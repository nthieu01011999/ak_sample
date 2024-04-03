#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "audiosource.hpp"
#include <unistd.h>
#include <Util.h>
#include <utils.h>

using namespace std;

AudioSource::AudioSource(uint32_t sps) {
	this->sampleDuration_us = 1000 * 1000 / sps;
}

AudioSource::~AudioSource() {
	stop();
}

void AudioSource::start() {
	sampleTime_us = std::numeric_limits<uint64_t>::max() - sampleDuration_us + 1;
	loadNextSample();
}

void AudioSource::stop() {
	sample		  = {};
	sampleTime_us = 0;
}

void AudioSource::loadNextSample() {
	sampleTime_us += sampleDuration_us;
}

void AudioSource::loadNextSample(uint8_t *bytes, uint32_t len) {
	sampleTime_us += sampleDuration_us;

	sample.clear();
	sample.insert(sample.end(), reinterpret_cast<std::byte *>(bytes), reinterpret_cast<std::byte *>(bytes) + len);
}

rtc::binary AudioSource::getSample() {
	return sample;
}

uint64_t AudioSource::getSampleTime_us() {
	return sampleTime_us;
}

uint64_t AudioSource::getSampleDuration_us() {
	return sampleDuration_us;
}

/*-----------------------------------------------------------------------------------------*/
AudioSourceSD::AudioSourceSD(uint32_t sps) : AudioSource(sps) {}

AudioSourceSD::~AudioSourceSD() {}

void AudioSourceSD::assignRecord(std::string pathToRecords, std::string dateTime, std::string desc) {
	mURL		   = pathToRecords + std::string("/audio/") + dateTime + std::string("/") + desc + std::string(".g711");
	auto stSplit   = string_split(desc, "_");
	mDurationInSec = std::stoi(stSplit[2]) - std::stoi(stSplit[1]);
}

bool AudioSourceSD::isStart() {
	if (!mURL.empty()) {
		mTrack	  = 0;
		mFileSize = sizeOfFile(mURL.c_str());
	}

	return (mFileSize > 0 && mDurationInSec != 0);
}

void AudioSourceSD::stop() {
	mURL.clear();
	mTrack = mFileSize = 0;
	mDurationInSec	   = 0;
}

void AudioSourceSD::loadNextSample() {
	AudioSource::loadNextSample();

	std::byte *bytes = (std::byte *)malloc(sizeof(std::byte) * ALAW_MAX_FRAME_LOAD);

	if (bytes == NULL) {
		return;
	}

	sample.clear();
	memset(bytes, 0, ALAW_MAX_FRAME_LOAD);
	int totalSamples = loadSampleTrack(bytes, ALAW_MAX_FRAME_LOAD);
	if (totalSamples != 0) {
		sample.insert(sample.end(), bytes, bytes + totalSamples);
		mTrack += totalSamples;
	}
	free(bytes);
}

void AudioSourceSD::setSeekPosInSec(uint32_t inSecs) {
	if (inSecs < mDurationInSec) {
		mTrack = (inSecs / 2) * (mFileSize / (mDurationInSec / 2));
	}
}

uint32_t AudioSourceSD::getSeekPosInSecs() {
	return ((mTrack * mDurationInSec) / mFileSize);
}

uint32_t AudioSourceSD::getDurationInSecs() {
	return mDurationInSec;
}

size_t AudioSourceSD::loadSampleTrack(std::byte *p, uint32_t nbrOfBytes) {
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