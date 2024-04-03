#ifndef __AUDIO_SOURCE_H__
#define __AUDIO_SOURCE_H__

#include <optional>
#include <string>
#include <vector>
#include <arpa/inet.h>

#include "stream.hpp"

#define OPUSSamplesDirectory "sources/unit_test/videoaudio/livestream/opus/"

class AudioSource : public StreamSource {
protected:
	uint64_t sampleDuration_us;
	uint64_t sampleTime_us = 0;

public:
	AudioSource(uint32_t sps);
	~AudioSource();
	void start();
	void stop();
	void loadNextSample();
	void loadNextSample(uint8_t *, uint32_t);
	rtc::binary getSample();
	uint64_t getSampleTime_us();
	uint64_t getSampleDuration_us();
};

class AudioSourceSD : public AudioSource {
private:
#define ALAW_MAX_FRAME_LOAD (550)

	std::string mURL;
	size_t mTrack;
	size_t mFileSize;
	uint32_t mDurationInSec = 0;

	size_t loadSampleTrack(std::byte *p, uint32_t nbrOfBytes);

public:
	AudioSourceSD(uint32_t sps);
	~AudioSourceSD();

	void assignRecord(std::string pathToRecords, std::string dateTime, std::string desc);
	bool isStart();
	void stop();
	void loadNextSample();
	void setSeekPosInSec(uint32_t inSecs);
	uint32_t getSeekPosInSecs();
	uint32_t getDurationInSecs();
};

#endif /* __AUDIO_SOURCE_H__ */




// class StreamSource {
// protected:
// 	rtc::binary sample;

// public:
// 	virtual void start()							 = 0;
// 	virtual void stop()								 = 0;
// 	virtual void loadNextSample()					 = 0;
// 	virtual void loadNextSample(uint8_t *, uint32_t) = 0;
// 	virtual uint64_t getSampleTime_us()				 = 0;
// 	virtual uint64_t getSampleDuration_us()			 = 0;
// 	virtual rtc::binary getSample()					 = 0;
// 	void rstSample() {
// 		sample.clear();
// 	}
// };
