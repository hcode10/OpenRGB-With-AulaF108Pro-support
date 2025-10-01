/*---------------------------------------------------------*\
| AulaF108ProDetect.cpp                                     |
|                                                           |
|   Detector for Aula F108Pro keyboard                     |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "AulaF108ProController.h"
#include "RGBController.h"
#include "RGBController_AulaF108Pro.h"
#include <hidapi.h>

// Aula F108Pro VID/PID - YOU NEED TO FILL THESE!
#define AULA_VID                    0x0C45  // Replace with actual VID
#define AULA_F108PRO_PID            0x800A  // Replace with actual PID

void DetectAulaF108ProControllers(hid_device_info* info, const std::string& name)
{

#ifdef USE_HID_USAGE
    if(info->usage_page != 0xFF13 || info->usage != 0x0001 || info->interface_number != 3)
    {
        return;
    }
#else
    if(info->interface_number != 3)
    {
        return;
    }
#endif

    hid_device* dev = hid_open_path(info->path);

    if(dev)
    {
        AulaF108ProController*     controller     = new AulaF108ProController(dev, info->path);
        RGBController_AulaF108Pro* rgb_controller = new RGBController_AulaF108Pro(controller);
        rgb_controller->name = name;
        
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR("Aula F108Pro", DetectAulaF108ProControllers, AULA_VID, AULA_F108PRO_PID);
