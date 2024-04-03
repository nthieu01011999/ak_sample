#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "h26xsource.hpp"
#include "utils.h"
#include <unistd.h>
#include <Util.h>
using namespace std;

H26XSource::H26XSource(uint32_t fps) {
	this->sampleDuration_us = 1000 * 1000 / fps;
}

H26XSource::~H26XSource() {
	stop();
}

void H26XSource::start() {
	sampleTime_us = std::numeric_limits<uint64_t>::max() - sampleDuration_us + 1;
	loadNextSample();
}

void H26XSource::stop() {
	sample		  = {};
	sampleTime_us = 0;
}

void H26XSource::loadNextSample() {
	sampleTime_us += sampleDuration_us;
}

void H26XSource::loadNextSample(uint8_t *bytes, uint32_t len) {
	sampleTime_us += sampleDuration_us;

	sample.clear();
	splitNALUS((unsigned char *)bytes, len);
	extractNALUS(sample);
}

rtc::binary H26XSource::getSample() {
	return sample;
}

uint64_t H26XSource::getSampleTime_us() {
	return sampleTime_us;
}

uint64_t H26XSource::getSampleDuration_us() {
	return sampleDuration_us;
}

vector<byte> H26XSource::initialNALUS() {
	vector<byte> units{};
	if (previousUnitType7.has_value()) {
		auto nalu = previousUnitType7.value();
		units.insert(units.end(), nalu.begin(), nalu.end());
	}
	if (previousUnitType8.has_value()) {
		auto nalu = previousUnitType8.value();
		units.insert(units.end(), nalu.begin(), nalu.end());
	}
	if (previousUnitType5.has_value()) {
		auto nalu = previousUnitType5.value();
		units.insert(units.end(), nalu.begin(), nalu.end());
	}
	return units;
}

void H26XSource::appendSample(unsigned char *buffer, size_t length) {
	uint32_t nalu_nlen = htonl((uint32_t)length);
	sample.insert(sample.end(), reinterpret_cast<const byte *>(&nalu_nlen), reinterpret_cast<const byte *>(&nalu_nlen) + sizeof(nalu_nlen));
	sample.insert(sample.end(), reinterpret_cast<const byte *>(buffer), reinterpret_cast<const byte *>(buffer) + length);
}

void H26XSource::splitNALUS(unsigned char *buffer, size_t length) {
	const unsigned char zero_sequence_4[4] = {0, 0, 0, 1};
	unsigned char *begin				   = buffer;
	unsigned char *found				   = NULL;
	size_t remain_length				   = length;

	found = (unsigned char *)memmem(begin, remain_length, zero_sequence_4, sizeof(zero_sequence_4));
	while (found) {
		int nalu_hlen = found - begin;
		if (nalu_hlen) {
			appendSample(begin, nalu_hlen);
		}
		begin = found + sizeof(zero_sequence_4);
		remain_length -= (nalu_hlen + sizeof(zero_sequence_4));
		found = (unsigned char *)memmem(begin, remain_length, zero_sequence_4, sizeof(zero_sequence_4));
	}
	if (remain_length) {
		appendSample(begin, remain_length);
	}
}

void H26XSource::extractNALUS(rtc::binary &samples) {
	size_t i = 0;
	while (i < samples.size()) {
		assert(i + 4 < samples.size());
		auto lengthPtr		= (uint32_t *)(samples.data() + i);
		uint32_t length		= ntohl(*lengthPtr);
		auto naluStartIndex = i + 4;
		auto naluEndIndex	= naluStartIndex + length;
		assert(naluEndIndex <= samples.size());
		auto header = reinterpret_cast<rtc::NalUnitHeader *>(samples.data() + naluStartIndex);
		auto type	= header->unitType();
		switch (type) {
		case 7:
			previousUnitType7 = {samples.begin() + i, samples.begin() + naluEndIndex};
			break;
		case 8:
			previousUnitType8 = {samples.begin() + i, samples.begin() + naluEndIndex};
			;
			break;
		case 5:
			previousUnitType5 = {samples.begin() + i, samples.begin() + naluEndIndex};
			;
			break;
		}
		i = naluEndIndex;
	}
}

/*-----------------------------------------------------------------------------------------*/
H26XSourceSD::H26XSourceSD(uint32_t fps) : H26XSource(fps) {}
H26XSourceSD::~H26XSourceSD() {
	stop();
}

void H26XSourceSD::assignRecord(std::string pathToRecords, std::string dateTime, std::string desc) {
	mURL = pathToRecords + std::string("/video/") + dateTime + std::string("/") + desc + std::string("_13.h264");
	APP_DBG("record path: %s\n", mURL.c_str());
	auto stSplit   = string_split(desc, "_");
	mDurationInSec = std::stoi(stSplit[2]) - std::stoi(stSplit[1]);
}

bool H26XSourceSD::isStart() {
	if (!mURL.empty()) {
		mTrack	  = 0;
		mFileSize = sizeOfFile(mURL.c_str());
		mCalib	  = 5;
	}

	return (mFileSize > 0 && mDurationInSec != 0);
}

void H26XSourceSD::stop() {
	mURL.clear();
	mTrack = mFileSize = 0;
	mDurationInSec	   = 0;
}

bool H26XSourceSD::isDone() {
	if (mTrack >= mFileSize) {
		return true;
	}

	return false;
}

void H26XSourceSD::loadNextSample() {
	H26XSource::loadNextSample();

	enum {
		NALU_NOTFOUND = 0,
		NALU_BEGIN,
		NALU_END,
	};

	size_t beginNalu = NALU_NOTFOUND;
	bool isFound	 = false;
	std::byte *bytes = (std::byte *)malloc(sizeof(std::byte) * H264_MAX_FRAME_LOAD);

	if (bytes == NULL) {
		return;
	}

	sample.clear();
	std::vector<std::byte> tmp;

	for (uint16_t cnts = 0; cnts < mCalib; ++cnts) {
		beginNalu = NALU_NOTFOUND;
		isFound	  = false;

		do {
			memset(bytes, 0, H264_MAX_FRAME_LOAD);
			size_t totalSamples = loadSampleTrack(bytes, H264_MAX_FRAME_LOAD);

			if (totalSamples <= 4) {
				mTrack += totalSamples;
				break;
			}

			for (size_t id = 0; id < (totalSamples - 4); ++id) {
				if (isNALU4(id, bytes) == true) {
					++beginNalu;
				}

				if (beginNalu == NALU_NOTFOUND) {
					++mTrack;
					continue;
				}
				else if (beginNalu == NALU_BEGIN) {
					tmp.emplace_back(reinterpret_cast<const std::byte>(bytes[id]));
					++mTrack;
				}
				else {
					isFound = true;
					break;
				}
			}
		} while (!isFound);

		uint32_t naluLen = htonl(static_cast<uint32_t>(tmp.size() - 4));
		std::copy(reinterpret_cast<const std::byte *>(&naluLen), reinterpret_cast<const std::byte *>(&naluLen) + sizeof(naluLen), tmp.begin());
		sample.insert(sample.end(), tmp.begin(), tmp.end());
		tmp.clear();

		if (mCalib != 1) {
			--mCalib;
		}
	}

	free(bytes);

	if (sample.size()) {
		H26XSource::extractNALUS(sample);
	}
}

void H26XSourceSD::setSeekPosInSec(uint32_t inSecs) {
	if (inSecs < mDurationInSec) {
		mTrack = (inSecs / 2) * (mFileSize / (mDurationInSec / 2));
	}
}

void H26XSourceSD::setCalibSample(uint8_t u8) {
	mCalib = u8;
}

uint32_t H26XSourceSD::getSeekPosInSecs() {
	return (uint32_t)(((double)mTrack / mFileSize) * mDurationInSec);
}

uint32_t H26XSourceSD::getDurationInSecs() {
	return mDurationInSec;
}

size_t H26XSourceSD::loadSampleTrack(std::byte *p, uint32_t nbrOfBytes) {
	int fd = -1;

	if (mTrack >= mFileSize) {
		return 0;
	}

	fd = open(mURL.c_str(), O_RDONLY);
	if (fd < 0) {
		return 0;
	}

	size_t remain	 = mFileSize - mTrack;
	size_t validSize = (remain > nbrOfBytes ? nbrOfBytes : remain);
	size_t ret		 = pread(fd, p, validSize, mTrack);

	close(fd);

	return ret;
}

bool H26XSourceSD::isNALU4(size_t startPos, std::byte *p) {
	return (p[startPos + 0] == std::byte{0} && p[startPos + 1] == std::byte{0} && p[startPos + 2] == std::byte{0} && p[startPos + 3] == std::byte{1});
}