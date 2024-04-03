#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <vector>

#include <algorithm>
#include <future>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <chrono>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <atomic>

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_webrtc.h"
#include "rtc/rtc.hpp"
#include "h264fileparser.hpp"
#include "opusfileparser.hpp"

using namespace rtc;
using namespace std;
using namespace chrono_literals;
using namespace chrono;

using json = nlohmann::json;

static rtcServersConfig_t rtcServerCfg;

/// Audio and video stream
std::optional<std::shared_ptr<Stream>> avStream = std::nullopt;

const std::string defaultRootDirectory = "../../../examples/streamer/samples/";
const std::string defaultH264SamplesDirectory = defaultRootDirectory + "h264/";
std::string h264SamplesDirectory = defaultH264SamplesDirectory;
const std::string defaultOpusSamplesDirectory = defaultRootDirectory + "opus/";
std::string opusSamplesDirectory = defaultOpusSamplesDirectory;
const std::string defaultIPAddress = "127.0.0.1";
const uint16_t defaultPort = 8000;
std::string ip_address = defaultIPAddress;
uint16_t port = defaultPort;


static shared_ptr<Stream> createStream(void);
static void addToStream(shared_ptr<Client> client, bool isAddingVideo);
static void startStream();
static shared_ptr<ClientTrackData> addVideo(const shared_ptr<PeerConnection> pc, const uint8_t payloadType, const uint32_t ssrc, const string cname, const string msid,
                                            const function<void(void)> onOpen);
static shared_ptr<ClientTrackData> addAudio(const shared_ptr<PeerConnection> pc, const uint8_t payloadType, const uint32_t ssrc, const string cname, const string msid,
                                            const function<void(void)> onOpen);
void sendInitialNalus(std::shared_ptr<Stream> stream, std::shared_ptr<ClientTrackData> video);									
#ifdef BUILD_ARM_RTS
#endif

q_msg_t gw_task_webrtc_mailbox;

void *gw_task_webrtc_entry(void *) {
	ak_msg_t *msg = AK_MSG_NULL;

	wait_all_tasks_started();

	APP_DBG("[STARTED] gw_task_webrtc_entry\n");

	//Configure Network Settings, Including STUN/TURN Servers for WebRTC Connections
	Configuration config;
	string stunServer = "stun:42.116.138.35:3478";
	cout << "STUN server is " << stunServer << endl;
	config.iceServers.emplace_back(stunServer);


	while (1) {
		/* get messge */
		msg = ak_msg_rev(GW_TASK_WEBRTC_ID);

		switch (msg->header->sig) {
			
		default:
		break;
		}

		/* free message */
		ak_msg_free(msg);
	}

	return (void *)0;
}

/// Create stream
shared_ptr<Stream> createStream(const string h264Samples, const unsigned fps, const string opusSamples) {
    auto video = make_shared<H264FileParser>(h264Samples, fps, true); 
    auto audio = make_shared<OPUSFileParser>(opusSamples, true);

    auto stream = make_shared<Stream>(video, audio);
   
}

void startStream() {
    shared_ptr<Stream> stream;
    if (avStream.has_value()) {
        stream = avStream.value();
        if (stream->isRunning) {
            // stream is already running
            return;
        }
    } else {
        stream = createStream(h264SamplesDirectory, USER_FPS, opusSamplesDirectory);
        avStream = stream;
    }
    stream->start(); //thread run or not, not start();
}

/// Add client to stream
/// @param client Client
/// @param adding_video True if adding video
void addToStream(shared_ptr<Client> client, bool isAddingVideo) {
    if (client->getState() == Client::State::Waiting) {
        client->setState(isAddingVideo ? Client::State::WaitingForAudio : Client::State::WaitingForVideo);
    } else if ((client->getState() == Client::State::WaitingForAudio && !isAddingVideo)
               || (client->getState() == Client::State::WaitingForVideo && isAddingVideo)) {

        // Audio and video tracks are collected now
        assert(client->video.has_value() && client->audio.has_value());
        auto video = client->video.value();

        if (avStream.has_value()) {
            sendInitialNalus(avStream.value(), video);
        }

        client->setState(Client::State::Ready);
    }
    if (client->getState() == Client::State::Ready) {
        startStream();
    }
}



shared_ptr<ClientTrackData> addVideo(const shared_ptr<PeerConnection> pc, 
                                     const uint8_t payloadType, 
                                     const uint32_t ssrc, 
                                     const string cname, 
                                     const string msid, 
                                     const function<void (void)> onOpen)
    {
    auto video = Description::Video(cname);
    video.addH264Codec(payloadType);
    video.addSSRC(ssrc, cname, msid, cname);
    auto track = pc->addTrack(video);
    // create RTP configuration
    auto rtpConfig = make_shared<RtpPacketizationConfig>(ssrc, cname, payloadType, H264RtpPacketizer::defaultClockRate);
    // create packetizer
    auto packetizer = make_shared<H264RtpPacketizer>(NalUnit::Separator::Length, rtpConfig);
    // add RTCP SR handler
    auto srReporter = make_shared<RtcpSrReporter>(rtpConfig);
    packetizer->addToChain(srReporter);
    // add RTCP NACK handler
    auto nackResponder = make_shared<RtcpNackResponder>();
    packetizer->addToChain(nackResponder);
    // set handler
    track->setMediaHandler(packetizer);
    track->onOpen(onOpen);
    auto trackData = make_shared<ClientTrackData>(track, srReporter);
    return trackData;
}


shared_ptr<ClientTrackData> addAudio(const shared_ptr<PeerConnection> pc, 
                                     const uint8_t payloadType, 
                                     const uint32_t ssrc, 
                                     const string cname, 
                                     const string msid, 
                                     const function<void (void)> onOpen)
{
    auto audio = Description::Audio(cname);
    // audio.addOpusCodec(payloadType);
    audio.addPCMACodec(payloadType);
    // audio.addPCMUCodec(payloadType);
    audio.addSSRC(ssrc, cname, msid, cname);
    auto track = pc->addTrack(audio);
    // create RTP configuration
    auto rtpConfig = make_shared<RtpPacketizationConfig>(ssrc, cname, payloadType, OpusRtpPacketizer::DefaultClockRate);
    // create packetizer
    auto packetizer = make_shared<OpusRtpPacketizer>(rtpConfig);
    // add RTCP SR handler
    auto srReporter = make_shared<RtcpSrReporter>(rtpConfig);
    packetizer->addToChain(srReporter);
    // add RTCP NACK handler
    auto nackResponder = make_shared<RtcpNackResponder>();
    packetizer->addToChain(nackResponder);
    // set handler
    track->setMediaHandler(packetizer);
    track->onOpen(onOpen);
    auto trackData = make_shared<ClientTrackData>(track, srReporter);
    return trackData;
}

// shared_ptr<ClientTrackData> addVideo_(const shared_ptr<PeerConnection> pc, const uint8_t payloadType, const uint32_t ssrc, const string cname, const string msid,
// 									 const function<void(void)> onOpen) {
// 	auto video = Description::Video(cname, Description::Direction::SendOnly);
// 	video.addH264Codec(payloadType);
// 	video.addSSRC(ssrc, cname, msid, cname);
// 	auto track = pc->addTrack(video);
// 	// create RTP configuration
// 	auto rtpConfig = make_shared<RtpPacketizationConfig>(ssrc, cname, payloadType, H264RtpPacketizer::defaultClockRate);
// 	// create packetizer
// 	auto packetizer = make_shared<H264RtpPacketizer>(NalUnit::Separator::Length, rtpConfig);
// 	// add RTCP SR handler
// 	auto srReporter = make_shared<RtcpSrReporter>(rtpConfig);
// 	packetizer->addToChain(srReporter);
// 	// add RTCP NACK handler
// 	auto nackResponder = make_shared<RtcpNackResponder>();
// 	packetizer->addToChain(nackResponder);
// 	// set handler
// 	track->setMediaHandler(packetizer);
// 	track->onOpen(onOpen);
// 	auto trackData = make_shared<ClientTrackData>(track, srReporter);
// 	return trackData;
// }


/// Send previous key frame so browser can show something to user
/// @param stream Stream
/// @param video Video track data
void sendInitialNalus(shared_ptr<Stream> stream, shared_ptr<ClientTrackData> video) {
    auto h264 = dynamic_cast<H264FileParser *>(stream->video.get());
    auto initialNalus = h264->initialNALUS();

    // send previous NALU key frame so users don't have to wait to see stream works
    if (!initialNalus.empty()) {
        APP_PRINT("-> !initialNalus.empty()\n");

        const double frameDuration_s = double(h264->getSampleDuration_us()) / (1000 * 1000);
        const uint32_t frameTimestampDuration = video->sender->rtpConfig->secondsToTimestamp(frameDuration_s);
        video->sender->rtpConfig->timestamp = video->sender->rtpConfig->startTimestamp - frameTimestampDuration * 2;
        video->track->send(initialNalus);
        video->sender->rtpConfig->timestamp += frameTimestampDuration;
        // Send initial NAL units again to start stream in firefox browser
        video->track->send(initialNalus);
    }
    else {
        APP_PRINT("-> initialNalus.empty()\n");
    }
}