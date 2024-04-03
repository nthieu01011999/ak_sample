#ifndef __PLAYBACK_H__
#define __PLAYBACK_H__

#include "h26xsource.hpp"
#include "audiosource.hpp"

class PlayBack {
public:
	enum class ePbControl {
		Play,
		Pause,
		Stop,
		Resume,
		Seek
	};

	enum class ePbStatus {
		Playing,
		Pausing,
		Stopped,
		Done,
		Error
	};

	PlayBack(std::shared_ptr<H26XSourceSD> video, std::shared_ptr<AudioSourceSD> audio);
	~PlayBack();

	void assignRecord(std::string pathToRecords, std::string dateTime, std::string desc);
	void loadNextSample(Stream::StreamSourceType type);
	rtc::binary getSample(Stream::StreamSourceType type);
	uint64_t getSampleTime_us(Stream::StreamSourceType type);
	uint64_t getSampleDuration_us(Stream::StreamSourceType type);
	void seekPosInSecs(uint32_t inSecs);
	uint32_t getSeekInSecs();
	void setPbControl(ePbControl pbc, uint32_t argv);
	ePbStatus getPbState();

private:
	std::mutex mMutex;

	std::shared_ptr<H26XSourceSD> video;
	std::shared_ptr<AudioSourceSD> audio;

	bool mIsRunning		= false;
	ePbStatus mPbStatus = ePbStatus::Stopped;

	void start();
	void stop();

public:
	uint32_t lastSecondTick;
};

#endif