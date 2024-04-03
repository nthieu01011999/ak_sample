/**
 * libdatachannel streamer example
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef stream_hpp
#define stream_hpp

#include <pthread.h>

#include "dispatchqueue.hpp"
#include "rtc/rtc.hpp"

class StreamSource {
protected:
	rtc::binary sample;

public:
	virtual void start()							 = 0;
	virtual void stop()								 = 0;
	virtual void loadNextSample()					 = 0;
	virtual void loadNextSample(uint8_t *, uint32_t) = 0;
	virtual uint64_t getSampleTime_us()				 = 0;
	virtual uint64_t getSampleDuration_us()			 = 0;
	virtual rtc::binary getSample()					 = 0;
	void rstSample() {
		sample.clear();
	}
};

class Stream : public std::enable_shared_from_this<Stream> {
public:
	enum class StreamSourceType {
		Audio,
		Video
	};

	Stream(std::shared_ptr<StreamSource> video, std::shared_ptr<StreamSource> audio);
	~Stream();
	void onSample(std::function<void(StreamSourceType, uint64_t)> handler);
	void startLive();
	void stopLive();
	void startPbSession();
	void stopPbSession();

	static void syncLiveWaitTime(std::shared_ptr<Stream> AVStream);
	static void LiveVideo(bool isFullHD, uint8_t *, uint32_t);
	static void LiveAudio(uint8_t *, uint32_t);

private:
	std::mutex mutex;
	DispatchQueue dispatchQueue = DispatchQueue("StreamQueue");
	bool _isRunning				= false;
	bool mIsPbRunning			= false;
	rtc::synchronized_callback<StreamSourceType, uint64_t> sampleHandler;

	std::pair<std::shared_ptr<StreamSource>, StreamSourceType> unsafePrepareForSample();
	void sendSample();

public:
	const bool &isRunning = _isRunning;
	std::shared_ptr<StreamSource> audio;
	std::shared_ptr<StreamSource> video;

	std::shared_ptr<StreamSource> audioPb;
	std::shared_ptr<StreamSource> videoPb;

	uint64_t startLiveTime = 0;
	uint64_t startPbTime   = 0;
	static pthread_mutex_t pubLicStreamMutex;
};

#endif /* stream_hpp */
