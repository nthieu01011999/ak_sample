#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <mntent.h>
#include <unordered_map>
#include <cstring>
#include <fcntl.h>

#include "UsrDataChannelHdl.h"
#include "json.hpp"
// #include "SDCard.h"

using namespace std;
using json = nlohmann::json;

static void panTiltHdl(json& content);
static void playListHdl(json& content);
// static void playBackHdl(json& content);
static void downloadHdl(json& content);

static unordered_map<string, function<void(json&)>> dcCmdTblMaps = {
    { DC_CMD_PANTILT    , panTiltHdl    },
    { DC_CMD_PLAYLIST   , playListHdl   },
    // { DC_CMD_PLAYBACK   , playBackHdl   },
    { DC_CMD_DOWNLOADS3 , downloadHdl },
};

// Function to handle user data channel requests
void UsrDataChannelHdl(string clId, string& req, string& resp) {
    // Explicitly cast client ID to void to avoid unused variable warning
    (void)clId;

    // Declare a JSON object to parse the request into
    json JSON;
    
    // Try-Catch block to attempt parsing the request string into a JSON object
    try {
        // Parse the JSON request string
        JSON = json::parse(req);
    }
    catch (...) { // Catch any parsing exceptions
        // Log the error if JSON parsing fails
        // APP_DBG("JSON Parser failed %s\n", req.c_str());
        // Exit the function early in case of parsing failure
        return;
    }
    //cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<JSON << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<< endl;
    // Check if the ID in the JSON object matches the expected local camera ID
    if (JSON["Id"].get<string>().compare(LOCAL_CAM_ID) != 0) {
        // Log an error if the ID does not match
        // APP_DBG("The ID %s is not correct\n", JSON["Id"].get<string>().c_str());
        // Return early if the ID is incorrect
        return;
    }

    // Look for the command in the command table map using the 'Command' field from the JSON object
    auto it = dcCmdTblMaps.find(JSON["Command"].get<string>());

    // Determine if the type of the request is 'Request'
    bool boolean = (JSON["Type"].get<string>().compare("Request") == 0) ? true : false;
    
    // Check if the command is found in the map and the type is 'Request'
    if (it != dcCmdTblMaps.end() && boolean) {
        // Execute the command handler function with the JSON content
        it->second(JSON["Content"]);

        // Set the JSON type field to 'Respond' to indicate a response
        JSON["Type"] = "Respond";

        // Serialize the JSON object and assign it to the response string
        resp.assign(JSON.dump());
    }
    else { // If the command is not found or the type is not 'Request'
        // Log an error indicating an invalid command
        // APP_DBG("Invalid command %s\n", JSON["Command"].get<string>().c_str());
    }
}


void panTiltHdl(json& content) {
    // APP_DBG("panTiltHdl() -> %s\n", content.dump(4).c_str());

    bool boolean = false;

    if (content["Direction"].get<string>().compare("Up") == 0) {
        // TODO: Call API Control camera pan up

        boolean = true;
    }
    else if (content["Direction"].get<string>().compare("Down") == 0) {
        // TODO: Call API Control camera pan down

        boolean = true;
    }
    else if (content["Direction"].get<string>().compare("Left") == 0) {
        // TODO: Call API Control camera tilt left

        boolean = true;
    }
    else if (content["Direction"].get<string>().compare("Right") == 0) {
        // TODO: Call API Control camera tilt right

        boolean = true;
    }

    content.clear();
    content["Success"] = boolean;
}

void playListHdl(json& content) { 
    // APP_DBG("playListHdl() -> %s\n", content.dump(4).c_str());
    
    string qrTime = "";
    qrTime += to_string(content["Datetime"]["Year"].get<int>()) + string(".");
    qrTime += to_string(content["Datetime"]["Month"].get<int>()) + string(".");
    qrTime += to_string(content["Datetime"]["Date"].get<int>());

    // APP_DBG("SD Card path: %s\n", qrTime.c_str());
    content.clear();
    content["Total"] = 0;
    content["Playlists"].clear();

    // auto listRecords = testSDCARD.getAllPlaylists(qrTime);
    
    // content["Total"] = listRecords.size();
    // cout << "Total records: " << listRecords.size() << endl;
    // for (int id = 0; id < listRecords.size(); ++id) {
    //     json JS;
    //     try {
    //         JS["OrderNumber"]    = id;
    //         JS["Type"]           = listRecords[id].type;
    //         JS["FileName"]       = string(listRecords[id].fileName);
    //         JS["DurationInSecs"] = listRecords[id].durationInSecs;
    //         JS["BeginTime"]      = string(listRecords[id].beginTime);
    //         JS["EndTime"]        = string(listRecords[id].endTime);
    //         content["Playlists"].push_back(JS);
    //     }
    //     // catch (...) { APP_DBG("ERROR\n"); }
    // }
}

// void playBackHdl(json& content) {
//     // APP_DBG("playBackHdl() -> %s\n", content.dump(4).c_str());

//     bool boolean = false;
    
//     /*  Go to folder datetime: 
//         2023.11.24 13:00:28 -> Just take 2023.11.24
//     */
//     string dateTime(content["BeginTime"].get<string>());
//     string fileName = content["FileName"].get<string>();
//     dateTime = dateTime.substr(0, dateTime.find(' '));

//     if (content["Status"].get<string>().compare("Play") == 0) {
//         if (testPLAYBACK.getPbState() != SDPlayback::ePbStatus::Playing) {
//             testPLAYBACK.assignRecord(testSDCARD.getURL(), dateTime, fileName);
//             testPLAYBACK.setSpeed(1);
//             testPLAYBACK.setPbControl(SDPlayback::ePbControl::Play);
//             testPLAYBACK.start();
//         }
//     }
//     else if (content["Status"].get<string>().compare("Pause") == 0) {
//         testPLAYBACK.setPbControl(SDPlayback::ePbControl::Pause);
//     }
//     else if (content["Status"].get<string>().compare("Resume") == 0) {
//         testPLAYBACK.setPbControl(SDPlayback::ePbControl::Play);
//     }
//     else if (content["Status"].get<string>().compare("Stop") == 0) {
//         testPLAYBACK.setPbControl(SDPlayback::ePbControl::Stop);
//         testPLAYBACK.stop();
//     }
//     else if (content["Status"].get<string>().compare("Seek") == 0) {
//         testPLAYBACK.setPbControl(SDPlayback::ePbControl::Seek);
//         int seekPos = content["SeekPos"].get<int>();
        
//         testPLAYBACK.seekPosInSecs(seekPos);
//     }

//     content.clear();
//     content["FileName"] = fileName;
//     content["BeginTime"] = "";
//     content["EndTime"] = "";
//     content["SeekPos"] = 0;
//     content["PlayStatus"] = (uint8_t)testPLAYBACK.getPbState();
// }

void downloadHdl(json& content) {
    // APP_DBG("downloadHdl() -> %s\n", content.dump(4).c_str());
    
    // TODO: Download
}