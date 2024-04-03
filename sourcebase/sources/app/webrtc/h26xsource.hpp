#ifndef __H26x_SOURCE_H__
#define __H26x_SOURCE_H__

#include <optional>
#include <string>
#include <vector>
#include <arpa/inet.h>

#include "stream.hpp"

#define H264SamplesDirectory "sources/unit_test/videoaudio/livestream/h264/"

class H26XSource : public StreamSource {
protected:
	uint64_t sampleDuration_us;
	uint64_t sampleTime_us									= 0;
	std::optional<std::vector<std::byte>> previousUnitType5 = std::nullopt;
	std::optional<std::vector<std::byte>> previousUnitType7 = std::nullopt;
	std::optional<std::vector<std::byte>> previousUnitType8 = std::nullopt;

	void appendSample(unsigned char *buffer, size_t length);
	void splitNALUS(unsigned char *buffer, size_t length);
	void extractNALUS(rtc::binary &samples);

public:
	H26XSource(uint32_t fps);
	~H26XSource();
	void start();
	void stop();
	void loadNextSample();
	void loadNextSample(uint8_t *, uint32_t);
	rtc::binary getSample();
	uint64_t getSampleTime_us();
	uint64_t getSampleDuration_us();
	std::vector<std::byte> initialNALUS();
};

class H26XSourceSD : public H26XSource {
private:
#define H264_MAX_FRAME_LOAD (2048)

	std::string mURL;
	size_t mTrack;
	size_t mFileSize;
	uint32_t mDurationInSec;
	uint8_t mCalib = 4;

	bool isNALU4(size_t startPos, std::byte *p);
	size_t loadSampleTrack(std::byte *p, uint32_t nbrOfBytes);

public:
	H26XSourceSD(uint32_t fps);
	~H26XSourceSD();

	void assignRecord(std::string pathToRecords, std::string dateTime, std::string desc);
	bool isStart();
	void stop();
	bool isDone();
	void loadNextSample();
	void setSeekPosInSec(uint32_t inSecs);
	void setCalibSample(uint8_t u8);
	uint32_t getSeekPosInSecs();
	uint32_t getDurationInSecs();
};

#endif /* __H26x_SOURCE_H__ */
