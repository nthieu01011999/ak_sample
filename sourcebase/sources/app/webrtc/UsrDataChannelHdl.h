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

#define APP_CONFIG_SUCCESS			  (0)
#define APP_CONFIG_ERROR_FILE_OPEN	  (-1)
#define APP_CONFIG_ERROR_DATA_MALLOC  (-2)
#define APP_CONFIG_ERROR_DATA_INVALID (-3)
#define APP_CONFIG_ERROR_DATA_DIFF	  (-4)
#define APP_CONFIG_ERROR_TIMEOUT	  (-5)
#define APP_CONFIG_ERROR_BUSY		  (-6)
#define APP_CONFIG_ERROR_ANOTHER	  (-7)

extern void UsrDataChannelHdl(std::string clId, std::string& req, std::string& resp);

#endif