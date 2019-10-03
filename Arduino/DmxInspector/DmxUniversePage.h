//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************

#pragma once
#include "DmxPageBase.h"
#include "DmxDeviceInfo.h"
#include "DmxDeviceSlot.h"

class DmxUniversePage : public DmxPageBase
{
    public:
        static const int kUniversePageId = 0;
        static const uint16_t kGridGrey = 16904;
        static const uint16_t kGridSelectedChannel = 0xffff;

        NexButton*   btnToDevices;
        NexButton*   btnToSettings;
        NexButton*   btnDevMinus;
        NexButton*   btnDevPlus;

        NexHotspot*  fnDrawGrid;
        NexHotspot*  fnDrawColor;
        NexHotspot*  fnDrawChanBox;
        NexHotspot*  fnChannelColor;
        NexHotspot*  fnChanLinear;

        NexText* txtTitle;
        
        NexVariable* vxChan;
        NexVariable* vxNum;
        NexVariable* vxColor;
        NexVariable* vxGridColor;
        NexVariable* vxSpan;
        NexVariable* vxDevSlot;
        NexVariable* vxChanSlot;
        NexVariable* vxValue;


        DmxUniversePage(uint8_t* data);

        void UpdateData();

        void OnNavigatedTo() override;
        void OnLoop() override;

        void SetCurrentDevice(int device);

        void DrawGrid();
        void DrawColor(uint16_t channelNumber, uint16_t span, uint16_t color);
        void DrawChanBox(uint16_t channelNumber, uint16_t span, uint16_t color);

    private: // implementation
        static void OnToDevicesPushed(void* p);
        static void OnToSettingsPushed(void* p);
        static void OnDevMinusPushed(void* p);
        static void OnDevPlusPushed(void* p);
        void OnDevChange(int plusOrMinus);

        uint8_t  _lastData[512];

        DmxDeviceInfo** _channelSortedDevices;
        int _deviceCount;
        int _sortedCount;
        int _currentDevice;

        void SortDevices();
        void InsertSorted(DmxDeviceInfo* pInfo);
        void ReadMinMaxChannels_P(DmxDeviceInfo* pInfo);
        int DrawDevice_P(DmxDeviceInfo* pInfo);

        int DisplayRGB(int chan, const uint8_t* data);
        int DisplayWarmWhite(int chan, uint8_t value);
        int DisplayCoolWhite(int chan, uint8_t value);
        int DisplayAmber(int chan, uint8_t value);
        int DisplayGrey(int chan, uint8_t value);

        static void PrintDeviceInfo_P(DmxDeviceInfo* pInfo);
        int _updateIterator;
        DmxDeviceInfo** _ppDeviceIterator;
        int _minChannel;
        int _maxChannel;

        bool _fullUpdateInProgress;

        DmxDeviceSlots* deviceSlot;


};