//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#pragma once
#include <Arduino.h>

template <typename T>
T* FromPGM(T* dest, const T* src)
{
    return reinterpret_cast<T*>(memcpy_P(dest, src, sizeof(T)));
}

struct DmxDeviceInfo
{
    enum DeviceType : uint8_t
    {
        // NOTE: if this enum is changed, the array channelsForType must also be updated
        None,
        Generic,    // Any single-channel non-color
        WW,         // Warm White
        CW,         // Cool White
        Amber,      // Amber 
        RGB,        // three channel RGB
        RGBW,       // four channel RGB + White
        RGBWC,      // five chennel RGB + White + Cool White
        RGBA,       // four channel RGB + Amber
        RGBAC,      // five chennel RGB + Amber + Cool White
        DRGB,       // 4C Dimmer + RGB
        DRGBW,      // 5C Dimmer + RGBW
        DRGBWC,     // 6C Dimmer + RGBWC
        DRGBA,      // 5C Dimmer + RGBA
        DRGBAC,      // 6C Dimmer + RGBAC
        Gen7Chan,   // A Generic 7-channel
        Gen8Chan    // A Generic 8-channel
    };
    char name[9];
    char desc[22];
    DeviceType type;
    uint16_t channel;

    DeviceType GetType_P();
    uint16_t GetChannel_P();
    uint8_t GetNumChannels_P();
    bool IsEmpty_P();

    static int CountDevicesInArray_P(DmxDeviceInfo* pArray);

    // index by DeviceType enum
    static const uint8_t channelsForType[];
    static const DmxDeviceInfo Empty;
};

struct DmxUniverse
{
    static const int kNumUniverses = 6;
    static const int kMaxNameLength = 30;

    int number; 
    char desc[kMaxNameLength];
    DmxDeviceInfo * devices;

    static const DmxUniverse universes[kNumUniverses];
};