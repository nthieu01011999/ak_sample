/**
 * libdatachannel streamer example
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "helpers.hpp"
#include "utils.h"

#include <ctime>
#include <arpa/inet.h>
#include <sys/time.h>
#include <chrono>

#include "app.h"
#include "app_data.hpp"
#include "task_list.h"

#include <chrono>

#include "app.h"
#include "app_data.hpp"
#include "task_list.h"

using namespace rtc;
using namespace std::chrono;

void Client::setState(State state) {
	std::unique_lock lock(_shrMutex);
	this->state = state;
}

Client::State Client::getState() {
	std::shared_lock lock(_shrMutex);
	return state;
}

bool Client::getDownloadFlag() {
	std::shared_lock lock(_shrMutex);
	return downloadFlag;
}

void Client::setDownloadFlag(bool newDownloadFlag) {
	std::unique_lock lock(_shrMutex);
	downloadFlag = newDownloadFlag;
}

fileDownloadInfo_t Client::getCurrentFileTransfer() {
	return currentFileTransfer;
}

void Client::setCurrentFileTransfer(const fileDownloadInfo_t &newCurrentFileTransfer) {
	currentFileTransfer = newCurrentFileTransfer;
}

uint32_t Client::getCursorFile() {
	return cursorFile;
}

void Client::setCursorFile(uint32_t newCursorFile) {
	cursorFile = newCursorFile;
}

void Client::startTimeoutDownload() {
	string id = getId();
	removeTimeoutDownload();
	setTimerDownloadId(systemTimer.add(milliseconds(GW_WEBRTC_WAIT_REQUEST_TIMEOUT_INTERVAL), [id](CppTime::timer_id) {
		task_post_dynamic_msg(GW_TASK_WEBRTC_ID, GW_WEBRTC_DATACHANNEL_DOWNLOAD_RELEASE_REQ, (uint8_t *)id.c_str(), id.length() + 1);
	}));
}

void Client::removeTimeoutDownload() {
	systemTimer.remove(getTimerDownloadId());
}

void Client::startTimeoutConnect() {
	string id = getId();
	removeTimeoutConnect();
	setTimerConnectId(systemTimer.add(milliseconds(GW_WEBRTC_ERASE_CLIENT_NO_ANSWER_TIMEOUT_INTERVAL), [id](CppTime::timer_id) {
		task_post_dynamic_msg(GW_TASK_WEBRTC_ID, GW_WEBRTC_CHECK_CLIENT_CONNECTED_REQ, (uint8_t *)id.c_str(), id.length() + 1);
	}));
}

void Client::removeTimeoutConnect() {
	systemTimer.remove(getTimerConnectId());
}

size_t Client::getTimerConnectId() {
	std::shared_lock lock(_shrMutex);
	return timerConnectId;
}

void Client::setTimerConnectId(size_t newTimerConnectId) {
	std::unique_lock lock(_shrMutex);
	timerConnectId = newTimerConnectId;
}

size_t Client::getTimerDownloadId() {
	return timerDownloadId;
}

void Client::setTimerDownloadId(size_t newTimerDownloadId) {
	timerDownloadId = newTimerDownloadId;
}

void Client::openPbChannel() {
	std::unique_lock lock(_shrMutex);

	auto video = make_shared<H26XSourceSD>(13);
	auto audio = make_shared<AudioSourceSD>(13);
	avPb	   = make_shared<PlayBack>(video, audio);
}

void Client::closePbChannel() {
	std::unique_lock lock(_shrMutex);

	if (avPb.use_count() != 0) {
		avPb.reset();
	}
}

bool Client::isPbChannelOpening() {
	std::shared_lock lock(_shrMutex);
	return ((avPb.use_count() != 0) && (mOptions == Client::eOptions::Playback));
}

void Client::setStreamOption(eOptions opt) {
	std::unique_lock lock(_shrMutex);

	mOptions = opt;
}

Client::eOptions Client::getStreamOption() {
	std::shared_lock lock(_shrMutex);

	return mOptions;
}

void Client::setResolution(eLiveResolution res) {
	std::unique_lock lock(_shrMutex);

	mLiveResolution = res;
}

Client::eLiveResolution Client::getResolution() {
	std::shared_lock lock(_shrMutex);

	return mLiveResolution;
}

PlayBack::ePbStatus Client::getPbState() {
	if (isPbChannelOpening()) {
		return avPb->getPbState();
	}

	return PlayBack::ePbStatus::Stopped;
}

string Client::getId() {
	return id;
}

void Client::setId(const string &newId) {
	id = newId;
}

bool Client::getSdpExist() const {
	return sdpExist;
}

void Client::setSdpExist(bool newSdpExist) {
	sdpExist = newSdpExist;
}

void Client::clMutexLock() {
	_mutex.lock();
}
void Client::clMutexUnlock() {
	_mutex.unlock();
}

uint64_t currentTimeInMicroSeconds() {
	struct timeval time;
	gettimeofday(&time, NULL);
	return uint64_t(time.tv_sec) * 1000 * 1000 + time.tv_usec;
}
