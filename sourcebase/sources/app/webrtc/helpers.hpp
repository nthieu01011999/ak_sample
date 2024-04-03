/**
 * libdatachannel streamer example
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef helpers_hpp
#define helpers_hpp

#include <vector>
#include <shared_mutex>
#include <string>

#include "app_dbg.h"
#include "rtc/rtc.hpp"
#include "playback.hpp"

using namespace std;

typedef struct t_fileDownloadInfo {
	string type;
	string path;
	uint32_t size;
} fileDownloadInfo_t;

struct ClientTrackData {
	std::shared_ptr<rtc::Track> track;
	std::shared_ptr<rtc::RtcpSrReporter> sender;

	ClientTrackData(std::shared_ptr<rtc::Track> track, std::shared_ptr<rtc::RtcpSrReporter> sender) {
		this->track	 = track;
		this->sender = sender;
	}
};

struct ClientTrack {
	std::string id;
	std::shared_ptr<ClientTrackData> trackData;
	ClientTrack(std::string id, std::shared_ptr<ClientTrackData> trackData) {
		this->id		= id;
		this->trackData = trackData;
	}
};

class Client {
public:
	enum class State {
		Waiting,
		WaitingForVideo,
		WaitingForAudio,
		Ready
	};

	enum class eOptions {
		Idle,
		LiveStream,
		Playback,
	};

	enum class eLiveResolution {
		HD720p,
		FullHD1080p,
	};

	Client(std::shared_ptr<rtc::PeerConnection> pc) {
		_peerConnection = pc;
	}
	~Client() {}

	std::vector<fileDownloadInfo_t> arrFilesDownload;

	void setState(State state);
	State getState();

	bool getDownloadFlag();
	void setDownloadFlag(bool newIsDownloading);

	fileDownloadInfo_t getCurrentFileTransfer();
	void setCurrentFileTransfer(const fileDownloadInfo_t &newCurrentFileTransfer);
	uint32_t getCursorFile();
	void setCursorFile(uint32_t newCursorFile);
	void startTimeoutDownload();
	void removeTimeoutDownload();
	void startTimeoutConnect();
	void removeTimeoutConnect();

	void openPbChannel();
	void closePbChannel();
	bool isPbChannelOpening();
	void setStreamOption(eOptions opt);
	void setResolution(eLiveResolution res);
	eOptions getStreamOption();
	eLiveResolution getResolution();
	PlayBack::ePbStatus getPbState();

	std::string getId();
	void setId(const std::string &newId);
	bool getSdpExist() const;
	void setSdpExist(bool newSdpExist);
	void clMutexLock();
	void clMutexUnlock();

public:
	const std::shared_ptr<rtc::PeerConnection> &peerConnection = _peerConnection;
	std::optional<std::shared_ptr<ClientTrackData>> video;
	std::optional<std::shared_ptr<ClientTrackData>> audio;
	std::optional<std::shared_ptr<rtc::DataChannel>> dataChannel;
	uint32_t rtpStartTimestamp = 0;

	std::shared_ptr<PlayBack> avPb;

protected:
	size_t getTimerConnectId();
	void setTimerConnectId(size_t newTimerConnectId);
	size_t getTimerDownloadId();
	void setTimerDownloadId(size_t newTimerDownloadId);

private:
	std::shared_mutex _shrMutex;
	std::mutex _mutex;
	State state = State::Waiting;
	std::string id;
	std::shared_ptr<rtc::PeerConnection> _peerConnection;

	bool downloadFlag	   = false;
	size_t timerDownloadId = 0, timerConnectId = 0;
	fileDownloadInfo_t currentFileTransfer;
	uint32_t cursorFile;
	bool sdpExist = false;

	eOptions mOptions				= eOptions::LiveStream;
	eLiveResolution mLiveResolution = eLiveResolution::FullHD1080p;
};

typedef std::unordered_map<std::string, std::shared_ptr<Client>> ClientsGroup_t;

uint64_t currentTimeInMicroSeconds();

#endif /* helpers_hpp */
