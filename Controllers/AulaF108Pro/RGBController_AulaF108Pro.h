/*---------------------------------------------------------*\
| RGBController_AulaF108Pro.h                               |
|                                                           |
|   RGBController for Aula F108Pro keyboard                |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AulaF108ProController.h"

class RGBController_AulaF108Pro : public RGBController
{
public:
    RGBController_AulaF108Pro(AulaF108ProController* controller_ptr);
    ~RGBController_AulaF108Pro();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    AulaF108ProController* controller;
};
