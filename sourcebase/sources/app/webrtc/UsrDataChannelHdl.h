#ifndef __DATACHANNEL_HDL_H
#define __DATACHANNEL_HDL_H

#include <stdio.h>
#include <stdint.h>

#include <iostream>

#define LOCAL_CAM_ID        "camerainfake0134"

#define DC_CMD_PANTILT      "PanTilt"
#define DC_CMD_PLAYLIST     "Playlist"
#define DC_CMD_PLAYBACK     "Playback"
#define DC_CMD_DOWNLOADS3   "DownloadS3"

extern void UsrDataChannelHdl(std::string clId, std::string& req, std::string& resp);

#endif