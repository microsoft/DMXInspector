//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#pragma once
#include "DmxPageBase.h"
#include "DmxDeviceInfo.h"

class DmxDeviceSlots;
class DmxDeviceSlot
{
    public:
        static const int kMaxChannelsPerSlot = 8;
        DmxDeviceSlots* pOwner;
        int          slotId;
        NexText*     txtDeviceName;
        NexText*     txtDeviceDesc;
        NexText*     txtDeviceChanValues;
        NexPicture*  picDeviceBoxes;

        DmxDeviceInfo* currentInfo;

        DmxDeviceSlot(DmxDeviceSlots* owner, int slot, int pageId, int baseId);

        static char* IdToComponentIndex(int id);

        void SetSlotInfo_P(DmxDeviceInfo* info);
        void SetSlotEmpty();

        void DisplayData(const uint8_t* data);
        void DisplayRGB(int chanSlot, const uint8_t* data);
        void DisplayWarmWhite(int chanSlot, uint8_t value);
        void DisplayAmber(int chanSlot, uint8_t value);
        void DisplayCoolWhite(int chanSlot, uint8_t value);
        void DisplayLinear(int chanSlot, uint8_t value);

        uint8_t _lastData[kMaxChannelsPerSlot];

};

class DmxDeviceSlots
{
    public:
        NexHotspot*  fnChannelColor;
        NexHotspot*  fnChanLinear;

        NexVariable* vxSpan;
        NexVariable* vxColor;
        NexVariable* vxDevSlot;
        NexVariable* vxChanSlot;
        NexVariable* vxValue;

        DmxDeviceSlot**  deviceSlots;
        int             numSlots;

        DmxDeviceSlots(int pageId, int baseId, int count);

        void UpdateData(const uint8_t* pData);

        void AssignMethods(NexHotspot* channelColor, NexHotspot* chanLinear);
        void AssignVariables(NexVariable* span, NexVariable* color, NexVariable* devSlot, NexVariable* chanSlot, NexVariable* value);

        void ChannelColor(int devSlot, int chanSlot, int span, uint16_t color);
        void ChannelLinear(int devSlot, int chanSlot, uint8_t value);
};