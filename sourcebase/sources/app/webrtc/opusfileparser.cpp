/**
 * libdatachannel streamer example
 * Copyright (c) 2020 Filip Klembara (in2core)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "opusfileparser.hpp"
// #include "SDCard.h"

using namespace std;

OPUSFileParser::OPUSFileParser(string directory, bool loop, uint32_t samplesPerSecond): FileParser(directory, ".opus", samplesPerSecond, loop) { }

void OPUSFileParser::loadNextSample()  {
    sample = { };
    
    // if (testPLAYBACK.getPbState() == SDPlayback::ePbStatus::Stopped) {
    //     FileParser::loadNextSample();
    // }
    // else if (testPLAYBACK.getPbState() == SDPlayback::ePbStatus::Playing) {

    //     testPLAYBACK.loadNextSample(SDPlayback::ePbType::Audio);
    //     if (testPLAYBACK.g711Samples.size() <= 0) {
    //         return;
    //     }

    //     sample = testPLAYBACK.g711Samples;
    // }

    // if (sample.size() > 0) {
    //     FileParser::sampleTimeCounts();
    // }
    // else {
    //     return;
    // }
}