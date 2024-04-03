/**
 * libdatachannel streamer example
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */
#include <unistd.h>

#include "stream.hpp"
#include "helpers.hpp"
#include "app_data.hpp"

pthread_mutex_t Stream::pubLicStreamMutex = PTHREAD_MUTEX_INITIALIZER;

Stream::Stream(std::shared_ptr<StreamSource> video, std::shared_ptr<StreamSource> audio) : std::enable_shared_from_this<Stream>(), video(video), audio(audio) {}

Stream::~Stream() {
	stopLive();
	stopPbSession();
}

std::pair<std::shared_ptr<StreamSource>, Stream::StreamSourceType> Stream::unsafePrepareForSample() {
	std::shared_ptr<StreamSource> ss;
	StreamSourceType sst;
	uint64_t nextTime;
	if (audioPb->getSampleTime_us() < videoPb->getSampleTime_us()) {
		ss		 = audioPb;
		sst		 = StreamSourceType::Audio;
		nextTime = audioPb->getSampleTime_us();
	}
	else {
		ss		 = videoPb;
		sst		 = StreamSourceType::Video;
		nextTime = videoPb->getSampleTime_us();
	}

	auto currentTime = currentTimeInMicroSeconds();

	auto elapsed = currentTime - startPbTime;

	if (nextTime > elapsed) {
		auto waitTime = nextTime - elapsed;
		mutex.unlock();
		usleep(waitTime);
		// std::this_thread::sleep_for(std::chrono::microseconds(waitTime));
		mutex.lock();
	}

	return {ss, sst};
}

void Stream::sendSample() {
	std::lock_guard lock(mutex);
	if (!mIsPbRunning) {
		return;
	}
	auto ssSST = unsafePrepareForSample();
	auto ss	   = ssSST.first;
	auto sst   = ssSST.second;
	sampleHandler(sst, ss->getSampleTime_us());
	ss->loadNextSample();

	dispatchQueue.dispatch([this]() { this->sendSample(); });
}

void Stream::onSample(std::function<void(StreamSourceType, uint64_t)> handler) {
	sampleHandler = handler;
}

void Stream::startLive() {
	std::lock_guard lock(mutex);
	if (isRunning) {
		return;
	}
	_isRunning	  = true;
	startLiveTime = currentTimeInMicroSeconds();
	audio->start();
	video->start();
}

void Stream::stopLive() {
	std::lock_guard lock(mutex);
	if (!isRunning) {
		return;
	}
	_isRunning = false;
	audio->stop();
	video->stop();
};

void Stream::startPbSession() {
	if (mIsPbRunning) {
		return;
	}
	audioPb->start();
	videoPb->start();
	startPbTime	 = currentTimeInMicroSeconds();
	mIsPbRunning = true;
	dispatchQueue.dispatch([this]() { this->sendSample(); });
}

void Stream::stopPbSession() {
	dispatchQueue.removePending();
	audioPb->stop();
	videoPb->stop();
	mIsPbRunning = false;
}

void Stream::syncLiveWaitTime(std::shared_ptr<Stream> AVStream) {
	uint64_t nextTime;

	if (AVStream->video->getSampleTime_us() > AVStream->audio->getSampleTime_us()) {
		nextTime = AVStream->audio->getSampleTime_us();
	}
	else {
		nextTime = AVStream->video->getSampleTime_us();
	}

	auto currentTime = currentTimeInMicroSeconds();
	auto elapsed	 = currentTime - AVStream->startLiveTime;

	if (nextTime > elapsed) {
		auto waitTime = nextTime - elapsed;
		pthread_mutex_unlock(&Stream::pubLicStreamMutex);
		usleep(waitTime);
		pthread_mutex_lock(&Stream::pubLicStreamMutex);
	}
}

void Stream::LiveVideo(bool isFullHD, uint8_t *samples, uint32_t totalSamples) {
	extern ClientsGroup_t clients;
	extern optional<shared_ptr<Stream>> avStream;

	if (!avStream.has_value()) {
		return;
	}

	auto avStreamValue = avStream.value();

	if (avStreamValue->video->getSampleTime_us() > avStreamValue->audio->getSampleTime_us()) {
		Stream::syncLiveWaitTime(avStreamValue);
	}

	avStreamValue->video->loadNextSample(samples, totalSamples);
	
	lock_guard lock(clientsMutex);
	for (auto it : clients) {
		auto id			  = it.first;
		auto client		  = it.second;
		auto optTrackData = client->video;

		if (client->getState() == Client::State::Ready && optTrackData.has_value()) {
			if (client->getStreamOption() == Client::eOptions::LiveStream) {
				bool isValid = false;

				if (isFullHD) {
					isValid = (client->getResolution() == Client::eLiveResolution::FullHD1080p) ? true : false;
				}
				else {
					isValid = (client->getResolution() == Client::eLiveResolution::HD720p) ? true : false;
				}
				if (isValid == false) {
					continue;
				}

				auto trackData = optTrackData.value();
				auto rtpConfig = trackData->sender->rtpConfig;

				auto elapsedSeconds			= double(avStreamValue->video->getSampleTime_us()) / (1000 * 1000);
				uint32_t elapsedTimestamp	= rtpConfig->secondsToTimestamp(elapsedSeconds);
				rtpConfig->timestamp		= rtpConfig->startTimestamp + elapsedTimestamp;
				auto reportElapsedTimestamp = rtpConfig->timestamp - trackData->sender->lastReportedTimestamp();
				if (rtpConfig->timestampToSeconds(reportElapsedTimestamp) > 1) {
					trackData->sender->setNeedsToReport();
				}

				try {
					trackData->track->send(avStreamValue->video->getSample());
				}
				catch (const std::exception &e) {
					// APP_DBG("[ERR] ClientId: %s unable to send audio packet, err: %s\n", id.c_str(), e.what());
				}
			}
		}
		else
			continue;
	}
}

void Stream::LiveAudio(uint8_t *samples, uint32_t totalSamples) {
	extern ClientsGroup_t clients;
	extern optional<shared_ptr<Stream>> avStream;

	if (!avStream.has_value()) {
		return;
	}
	auto avStreamValue = avStream.value();

	if (avStreamValue->video->getSampleTime_us() > avStreamValue->audio->getSampleTime_us()) {
		Stream::syncLiveWaitTime(avStreamValue);
	}

	avStreamValue->audio->loadNextSample(samples, totalSamples);

	lock_guard lock(clientsMutex);
	for (auto it : clients) {
		auto id			  = it.first;
		auto client		  = it.second;
		auto optTrackData = client->audio;

		if (client->getState() == Client::State::Ready && optTrackData.has_value()) {
			if (client->getStreamOption() == Client::eOptions::LiveStream) {
				auto trackData = optTrackData.value();
				auto rtpConfig = trackData->sender->rtpConfig;

				auto elapsedSeconds			= double(avStreamValue->audio->getSampleTime_us()) / (1000 * 1000);
				uint32_t elapsedTimestamp	= rtpConfig->secondsToTimestamp(elapsedSeconds);
				rtpConfig->timestamp		= rtpConfig->startTimestamp + elapsedTimestamp;
				auto reportElapsedTimestamp = rtpConfig->timestamp - trackData->sender->lastReportedTimestamp();
				if (rtpConfig->timestampToSeconds(reportElapsedTimestamp) > 1) {
					trackData->sender->setNeedsToReport();
				}

				try {
					trackData->track->send(avStreamValue->audio->getSample());
				}
				catch (const std::exception &e) {
					// APP_DBG("[ERR] ClientId: %s unable to send audio packet, err: %s\n", id.c_str(), e.what());
				}
			}
		}
		else
			continue;
	}
}
