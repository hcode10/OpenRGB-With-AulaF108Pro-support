/*---------------------------------------------------------*\
| RGBController_AulaF108Pro.cpp                             |
|                                                           |
|   RGBController for Aula F108Pro keyboard                |
|                                                           |
|   Converted from SignalRGB JavaScript code               |
\*---------------------------------------------------------*/

#include "RGBController_AulaF108Pro.h"

/**------------------------------------------------------------------*\
    @name Aula F108Pro
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectAulaF108ProControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_AulaF108Pro::RGBController_AulaF108Pro(AulaF108ProController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Aula F108Pro";
    vendor      = "Aula";
    type        = DEVICE_TYPE_KEYBOARD;
    description = "Aula F108Pro RGB Keyboard";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_AulaF108Pro::~RGBController_AulaF108Pro()
{
    delete controller;
}

void RGBController_AulaF108Pro::SetupZones()
{
    // Define LED indexes matching the JavaScript code
    const unsigned char led_indexes[] = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x70, 0x71, 0x73,
        0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x67, 0x74, 0x75, 0x76, 0x20, 0x21, 0x22, 0x7A,
        0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x43, 0x77, 0x78, 0x79, 0x32, 0x33, 0x34,
        0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x55, 0x44, 0x45, 0x46, 0x7B,
        0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x65, 0x56, 0x57, 0x58,
        0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x66, 0x68, 0x69, 0x6A
    };

    // LED names
    const char* led_names[] = {
        "Esc", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "PrintScrn", "ScrollLock", "Pause Break",
        "`", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "+", "BACKSPACE", "Insert", "Home", "Page Up", "Num Lock", "KEY/", "KEY*", "KEY-",
        "Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "\\", "Del", "End", "Pgdn", "KEY7", "KEY8", "KEY9",
        "CapsLock", "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "Enter", "KEY4", "KEY5", "KEY6", "KEY+",
        "Left Shift", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "Right Shift", "Up Arrow", "KEY1", "KEY2", "KEY3",
        "Ctrl", "Left Win", "Left Alt", "Space", "Right Alt", "Fn", "Menu", "Right Ctrl", "Left Arrow", "Down Arrow", "Right Arrow", "KEY0", "KEY.", "KEY Enter"
    };

    // LED positions (x, y)
    const unsigned int led_positions[][2] = {
        {0,0}, {2,0}, {3,0}, {4,0}, {5,0}, {7,0}, {8,0}, {9,0}, {10,0}, {12,0}, {13,0}, {14,0}, {15,0}, {17,0}, {18,0}, {19,0},
        {0,1}, {1,1}, {2,1}, {3,1}, {4,1}, {5,1}, {6,1}, {7,1}, {8,1}, {9,1}, {10,1}, {11,1}, {12,1}, {13,1}, {15,1}, {16,1}, {17,1}, {19,1}, {20,1}, {21,1}, {22,1},
        {0,2}, {1,2}, {2,2}, {3,2}, {4,2}, {5,2}, {6,2}, {7,2}, {8,2}, {9,2}, {10,2}, {11,2}, {12,2}, {13,2}, {15,2}, {16,2}, {17,2}, {19,2}, {20,2}, {21,2},
        {0,3}, {1,3}, {2,3}, {3,3}, {4,3}, {5,3}, {6,3}, {7,3}, {8,3}, {9,3}, {10,3}, {11,3}, {13,3}, {19,3}, {20,3}, {21,3}, {22,3},
        {0,4}, {1,4}, {2,4}, {3,4}, {4,4}, {5,4}, {6,4}, {7,4}, {8,4}, {9,4}, {10,4}, {11,4}, {14,4}, {19,4}, {20,4}, {21,4},
        {0,5}, {1,5}, {2,5}, {5,5}, {8,5}, {9,5}, {10,5}, {11,5}, {13,5}, {14,5}, {15,5}, {19,5}, {20,5}, {22,5}
    };

    zone keyboard_zone;
    keyboard_zone.name          = "Keyboard";
    keyboard_zone.type          = ZONE_TYPE_MATRIX;
    keyboard_zone.leds_min      = 98;
    keyboard_zone.leds_max      = 98;
    keyboard_zone.leds_count    = 98;

    keyboard_zone.matrix_map    = new matrix_map_type;
    keyboard_zone.matrix_map->height = 6;
    keyboard_zone.matrix_map->width  = 23;
    keyboard_zone.matrix_map->map = new unsigned int[6 * 23];

    // Initialize matrix to invalid
    for(unsigned int i = 0; i < 6 * 23; i++)
    {
        keyboard_zone.matrix_map->map[i] = 0xFFFFFFFF;
    }

    // Create LEDs and populate matrix
    for(unsigned int led_idx = 0; led_idx < 98; led_idx++)
    {
        led new_led;
        new_led.name  = led_names[led_idx];
        new_led.value = led_indexes[led_idx];

        unsigned int x = led_positions[led_idx][0];
        unsigned int y = led_positions[led_idx][1];

        keyboard_zone.matrix_map->map[y * 23 + x] = led_idx;

        leds.push_back(new_led);
    }

    zones.push_back(keyboard_zone);

    SetupColors();
}

void RGBController_AulaF108Pro::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // Not resizable
}

void RGBController_AulaF108Pro::DeviceUpdateLEDs()
{
    controller->SetLEDColors(colors, leds);
}

void RGBController_AulaF108Pro::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AulaF108Pro::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AulaF108Pro::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}
