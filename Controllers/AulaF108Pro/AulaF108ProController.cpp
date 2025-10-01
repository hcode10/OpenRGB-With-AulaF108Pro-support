/*---------------------------------------------------------*\
| AulaF108ProController.cpp                                 |
|                                                           |
|   Driver for Aula F108Pro keyboard                       |
|                                                           |
|   Converted from SignalRGB JavaScript code               |
\*---------------------------------------------------------*/

#include "AulaF108ProController.h"
#include <cstring>
#include <chrono>
#include <thread>

AulaF108ProController::AulaF108ProController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    // Start keepalive thread
    keepalive_running = true;
    keepalive_thread = std::thread(&AulaF108ProController::KeepaliveThreadFunction, this);
}

AulaF108ProController::~AulaF108ProController()
{
    // Stop keepalive thread
    keepalive_running = false;
    if(keepalive_thread.joinable())
    {
        keepalive_thread.join();
    }

    hid_close(dev);
}

std::string AulaF108ProController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AulaF108ProController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void AulaF108ProController::Refresh()
{
    unsigned char packet[65];
    memset(packet, 0, sizeof(packet));

    // Send first empty packet
    hid_send_feature_report(dev, packet, 65);

    // Send second empty packet (from packet capture in original code)
    hid_send_feature_report(dev, packet, 65);

    // Send command packet
    memset(packet, 0, sizeof(packet));
    packet[1] = 0x04;
    packet[2] = 0x02;
    hid_send_feature_report(dev, packet, 65);

    // Get response
    unsigned char data[65];
    hid_get_feature_report(dev, data, 65);
}

void AulaF108ProController::StartRefresh()
{
    unsigned char packet[65];
    memset(packet, 0, sizeof(packet));

    packet[1] = 0x04;
    packet[2] = 0x20;
    packet[9] = 0x08;

    hid_send_feature_report(dev, packet, 65);

    unsigned char data[65];
    hid_get_feature_report(dev, data, 65);
}

void AulaF108ProController::SetLEDColors(std::vector<RGBColor> colors, std::vector<led> leds)
{
    // Prepare RGB data with LED indexes
    std::vector<unsigned char> send_data;

    for(size_t i = 0; i < leds.size(); i++)
    {
        unsigned char led_index = leds[i].value;
        RGBColor color = colors[i];

        send_data.push_back(led_index);
        send_data.push_back(RGBGetRValue(color));
        send_data.push_back(RGBGetGValue(color));
        send_data.push_back(RGBGetBValue(color));
    }

    // Start refresh sequence
    StartRefresh();

    // Send data in 7 packets (64 bytes of data per packet)
    for(int packet_idx = 0; packet_idx <= 6; packet_idx++)
    {
        unsigned char packet[65];
        memset(packet, 0, sizeof(packet));
        packet[0] = 0x00;

        // Calculate how much data to copy
        size_t offset = packet_idx * 64;
        size_t remaining = send_data.size() - offset;
        size_t to_copy = (remaining > 64) ? 64 : remaining;

        if(to_copy > 0)
        {
            memcpy(&packet[1], &send_data[offset], to_copy);
        }

        hid_send_feature_report(dev, packet, 65);
    }

    // Final refresh
    Refresh();
}

void AulaF108ProController::KeepaliveThreadFunction()
{
    while(keepalive_running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        if(keepalive_running)
        {
            StartRefresh();
        }
    }
}
