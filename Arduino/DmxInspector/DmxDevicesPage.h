//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************

#pragma once
#include "DmxPageBase.h"
#include "DmxDeviceSlot.h"
#include "DmxDeviceInfo.h"

class DmxDevicesPage : public DmxPageBase
{
    public:
        static const int kDevicePageId = 1;
        static const int kDevicesPerPage = 6;
        static const int kDeviceNameBaseId = 3;
        static const int kDeviceDescBaseId = 4;
        static const int kDeviceValuesBaseId = 5;
        static const int kDeviceImageBaseId = 6;
        static const int kDeviceIdStride = 4;

        NexButton*   btnToUniverse;
        NexButton*   btnPagePlus;
        NexButton*   btnPageMinus;

        NexText*     txtPageNum;
        DmxDeviceSlots* deviceSlots;
 
        NexHotspot*  fnChannelColor;
        NexHotspot*  fnChanLinear;
        
        NexVariable* vxSpan;
        NexVariable* vxColor;
        NexVariable* vxDevSlot;
        NexVariable* vxChanSlot;
        NexVariable* vxValue;
        NexVariable* vxPageNumber;

        int topDevice;
        int numDevices;

        DmxDevicesPage(uint8_t* data);
        void OnNavigatedTo() override;
        void OnLoop() override;


    private:    // implementation
        static void OnToUniversePushed(void* p);
        static void OnPagePlusPushed(void* p);
        static void OnPageMinusPushed(void* p);
        void PageChanged(int dir);
        static char* IdToComponentIndex(int id, char* buf);
        void PopulateDeviceSlots();
        void SetPageNumber(int page);
};