/**
 * libdatachannel streamer example
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "h264fileparser.hpp"
#include "rtc/rtc.hpp"

#include <fstream>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

using namespace std;

H264FileParser::H264FileParser(string directory, uint32_t fps, bool loop): FileParser(directory, ".h264", fps, loop) { }

void H264FileParser::loadNextSample() {
    sample = { };

    // if (testPLAYBACK.getPbState() == SDPlayback::ePbStatus::Stopped) {
    //     FileParser::loadNextSample();
    // }
    // else if (testPLAYBACK.getPbState() == SDPlayback::ePbStatus::Playing) {
    //     testPLAYBACK.loadNextSample(SDPlayback::ePbType::Video);
    //     if (testPLAYBACK.h264Samples.size() <= 0) {
    //         return;
    //     }
    //     sample = testPLAYBACK.h264Samples;
    // }

    if (sample.size() > 0) {
        FileParser::sampleTimeCounts();
    }
    else {
        return;
    }

    size_t i = 0;
    while (i < sample.size()) {
        assert(i + 4 < sample.size());
        auto lengthPtr = (uint32_t *) (sample.data() + i);
        uint32_t length = ntohl(*lengthPtr);
        auto naluStartIndex = i + 4;
        auto naluEndIndex = naluStartIndex + length;
        assert(naluEndIndex <= sample.size());
        auto header = reinterpret_cast<rtc::NalUnitHeader *>(sample.data() + naluStartIndex);
        auto type = header->unitType();
        switch (type) {
            case 7:
                previousUnitType7 = {sample.begin() + i, sample.begin() + naluEndIndex};
                break;
            case 8:
                previousUnitType8 = {sample.begin() + i, sample.begin() + naluEndIndex};;
                break;
            case 5:
                previousUnitType5 = {sample.begin() + i, sample.begin() + naluEndIndex};;
                break;
        }
        i = naluEndIndex;
    }
}

vector<byte> H264FileParser::initialNALUS() {
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

void H264FileParser::appendSample(unsigned char *buf, size_t length) {
	uint32_t naluNLen = htonl((uint32_t)length);
	sample.insert(sample.end(), 
                  reinterpret_cast<const std::byte *>(&naluNLen), 
                  reinterpret_cast<const std::byte *>(&naluNLen) + sizeof(naluNLen));
	sample.insert(sample.end(), 
                  reinterpret_cast<const std::byte *>(buf), 
                  reinterpret_cast<const std::byte *>(buf) + length);
}

void H264FileParser::splitNALUS(unsigned char *buf, size_t length) {
	const unsigned char forbiddenBits[4] = {0, 0, 0, 1};
	unsigned char *begin = buf;
	unsigned char *found = NULL;
	size_t remain = length;

	found = (unsigned char *)memmem(begin, remain, forbiddenBits, sizeof(forbiddenBits));
	while (found) {
		int nalu_hlen = found - begin;
		if (nalu_hlen) {
			appendSample(begin, nalu_hlen);
		}
		begin = found + sizeof(forbiddenBits);
		remain -= (nalu_hlen + sizeof(forbiddenBits));
		found = (unsigned char *)memmem(begin, remain, forbiddenBits, sizeof(forbiddenBits));
	}
	if (remain) {
		appendSample(begin, remain);
	}
}
