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
#include "rtc/rtc.hpp"
#include "h264fileparser.hpp"
#include "opusfileparser.hpp"

using namespace rtc;
using namespace std;
using namespace chrono_literals;
using namespace chrono;

using json = nlohmann::json;

static rtcServersConfig_t rtcServerCfg;

#ifdef BUILD_ARM_RTS
static shared_ptr<Stream> createStream(void);
static void addToStream(shared_ptr<Client> client, bool isAddingVideo);
static void startStream();
static shared_ptr<ClientTrackData> addVideo(const shared_ptr<PeerConnection> pc, const uint8_t payloadType, const uint32_t ssrc, const string cname, const string msid,
                                            const function<void(void)> onOpen);
static shared_ptr<ClientTrackData> addAudio(const shared_ptr<PeerConnection> pc, const uint8_t payloadType, const uint32_t ssrc, const string cname, const string msid,
                                            const function<void(void)> onOpen);
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

shared_ptr<ClientTrackData> addVideo_(const shared_ptr<PeerConnection> pc, const uint8_t payloadType, const uint32_t ssrc, const string cname, const string msid,
									 const function<void(void)> onOpen) {
	auto video = Description::Video(cname, Description::Direction::SendOnly);
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