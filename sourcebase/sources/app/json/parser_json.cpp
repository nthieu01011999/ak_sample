#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "app.h"
#include "app_data.hpp"
#include "app_dbg.h"
#include "parser_json.h"
#include "json.hpp" // Make sure to include the appropriate header for the JSON library you are using

using json = nlohmann::json; // Assuming you're using nlohmann::json, adjust if using a different JSON library
using namespace std;

// bool mtce_jsonGetRtcServers(json &json_in, rtcServersConfig_t *rtcSvCfg) {
// 	try {
// 		rtcSvCfg->clear();
// 		json arrJs = json_in["Connections"];
// 		for (auto &server : arrJs) {
// 			string type = server["Type"].get<string>();
// 			if (type == "stun") {
// 				rtcSvCfg->stunServerCfg = server["StunUrl"].get<string>();
// 			}
// 			else if (type == "turn") {
// 				rtcSvCfg->turnServerCfg = server["TurnUrl"].get<string>();
// 			}
// 		}
// 		return true;
// 	}
// 	catch (const exception &e) {
// 		APP_DBG("json error: %s\n", e.what());
// 	}
// 	return false;
// }




// bool mtce_jsonGetRtcServers(const json &json_in, rtcServersConfig_t *rtcSvCfg) {
//     try {
//         rtcSvCfg->clear();
//         json arrJs = json_in["Connections"];
//         for (auto &server : arrJs) {
//             // Check if "Type" exists and is a string before proceeding
//             if (server.contains("Type") && server["Type"].is_string()) {
//                 string type = server["Type"].get<string>();
//                 if (type == "stun" && server.contains("StunUrl") && server["StunUrl"].is_string()) {
//                     rtcSvCfg->stunServerCfg = server["StunUrl"].get<string>();
// 					APP_PRINT("mtce_jsonGetRtcServers STUN server is: %s\n", rtcSvCfg->stunServerCfg.c_str());
//                 }
//                 else if (type == "turn" && server.contains("TurnUrl") && server["TurnUrl"].is_string()) {
//                     rtcSvCfg->turnServerCfg = server["TurnUrl"].get<string>();
// 					APP_PRINT("mtce_jsonGetRtcServers TURN server is: %s\n", rtcSvCfg->stunServerCfg.c_str());
//                 }
//             } else {
//                 // Optionally log a warning or handle the case where "Type" is not a string
//                 APP_DBG("Warning: 'Type' field is missing or not a string\n");
//             }
//         }
//         return true;
//     }
//     catch (const std::exception &e) {
//         APP_DBG("json error: %s\n", e.what());
//     }
//     return false;
// }


bool mtce_jsonGetRtcServers(const json &json_in, rtcServersConfig_t *rtcSvCfg) {
    if (rtcSvCfg == nullptr) {
        APP_DBG("rtcSvCfg is null");
        return false;
    }

    try {
        rtcSvCfg->clear();
        json arrJs = json_in.at("Connections");
        
        for (const auto &server : arrJs) {
            if (server.contains("Type") && server["Type"].is_string()) {
                string type = server["Type"].get<string>();
                
                if (type == "stun") {
                    // Check if "StunUrl" is a string or an array and handle accordingly
                    if (server.contains("StunUrl")) {
                        if (server["StunUrl"].is_string()) {
                            rtcSvCfg->stunServerCfg = server["StunUrl"].get<string>();
                        } else if (server["StunUrl"].is_array() && !server["StunUrl"].empty()) {
                            rtcSvCfg->stunServerCfg = server["StunUrl"][0].get<string>();
                        }
                    }
                } else if (type == "turn") {
                    // Check if "TurnUrl" is a string or an array and handle accordingly
                    if (server.contains("TurnUrl")) {
                        if (server["TurnUrl"].is_string()) {
                            rtcSvCfg->turnServerCfg = server["TurnUrl"].get<string>();
                        } else if (server["TurnUrl"].is_array() && !server["TurnUrl"].empty()) {
                            rtcSvCfg->turnServerCfg = server["TurnUrl"][0].get<string>();
                        }
                    }
                }
            } else {
                APP_DBG("Warning: 'Type' field is missing or not a string");
            }
        }
        return true;
    }
    catch (const std::exception &e) {
        APP_DBG("json error: %s", e.what());
        return false;
    }
}
