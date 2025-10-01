/*---------------------------------------------------------*\
| AulaF108ProController.h                                   |
|                                                           |
|   Driver for Aula F108Pro keyboard                       |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <string>
#include <thread>
#include <atomic>
#include <hidapi.h>

class AulaF108ProController
{
public:
    AulaF108ProController(hid_device* dev_handle, const char* path);
    ~AulaF108ProController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void SetLEDColors(std::vector<RGBColor> colors, std::vector<led> leds);

private:
    hid_device* dev;
    std::string location;

    std::thread keepalive_thread;
    std::atomic<bool> keepalive_running;

    void Refresh();
    void StartRefresh();
    void KeepaliveThreadFunction();
};
