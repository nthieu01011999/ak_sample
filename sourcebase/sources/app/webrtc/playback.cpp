#include "playback.hpp"

PlayBack::PlayBack(std::shared_ptr<H26XSourceSD> video, std::shared_ptr<AudioSourceSD> audio) : video(video), audio(audio) {}

PlayBack::~PlayBack() {}

void PlayBack::assignRecord(std::string pathToRecords, std::string dateTime, std::string desc) {
	std::lock_guard lock(mMutex);

	video->assignRecord(pathToRecords, dateTime, desc);
	audio->assignRecord(pathToRecords, dateTime, desc);
}

void PlayBack::start() {
	if (video->isStart() && audio->isStart()) {
		mPbStatus	   = PlayBack::ePbStatus::Playing;
		mIsRunning	   = true;
		lastSecondTick = 0;
	}
	else {
		mPbStatus  = PlayBack::ePbStatus::Error;
		mIsRunning = false;
	}
}

void PlayBack::stop() {
	mIsRunning = false;
	video.reset();
	audio.reset();
}

void PlayBack::loadNextSample(Stream::StreamSourceType type) {
	std::lock_guard lock(mMutex);

	if (mPbStatus != ePbStatus::Playing) {
		return;
	}

	(type == Stream::StreamSourceType::Video) ? video->loadNextSample() : audio->loadNextSample();

	if (video->isDone()) {
		mPbStatus = ePbStatus::Done;
	}
}

rtc::binary PlayBack::getSample(Stream::StreamSourceType type) {
	std::lock_guard lock(mMutex);

	return (type == Stream::StreamSourceType::Video) ? video->getSample() : audio->getSample();
}

uint64_t PlayBack::getSampleTime_us(Stream::StreamSourceType type) {
	return (type == Stream::StreamSourceType::Video) ? video->getSampleTime_us() : audio->getSampleTime_us();
}

uint64_t PlayBack::getSampleDuration_us(Stream::StreamSourceType type) {
	return (type == Stream::StreamSourceType::Video) ? video->getSampleDuration_us() : audio->getSampleDuration_us();
}

void PlayBack::seekPosInSecs(uint32_t inSecs) {
	video->setSeekPosInSec(inSecs);
	audio->setSeekPosInSec(inSecs);
}

uint32_t PlayBack::getSeekInSecs() {
	std::lock_guard lock(mMutex);

	return video->getSeekPosInSecs();
}

void PlayBack::setPbControl(ePbControl pbc, uint32_t argv) {
	std::lock_guard lock(mMutex);

	switch (pbc) {
	case ePbControl::Stop: {
		mPbStatus = ePbStatus::Stopped;
		stop();
		mIsRunning = false;
	} break;

	case ePbControl::Play: {
		mPbStatus = ePbStatus::Playing;
		if (mIsRunning == false) {
			start();
		}
	} break;

	case ePbControl::Resume: {
		mPbStatus = ePbStatus::Playing;
	} break;

	case ePbControl::Pause: {
		mPbStatus = ePbStatus::Pausing;
	} break;

	case ePbControl::Seek: {
		mPbStatus = ePbStatus::Playing;
		PlayBack::seekPosInSecs(argv);
	} break;

	default:
		break;
	}
}

PlayBack::ePbStatus PlayBack::getPbState() {
	std::lock_guard lock(mMutex);

	return mPbStatus;
}