//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#include "DmxDeviceSlot.h"

DmxDeviceSlot::DmxDeviceSlot(DmxDeviceSlots* owner, int slot, int pageId, int baseId)
{
    pOwner = owner;
    slotId = slot;
    txtDeviceName = new NexText(pageId, baseId, IdToComponentIndex(baseId));
    txtDeviceDesc = new NexText(pageId, baseId + 1, IdToComponentIndex(baseId + 1));
    txtDeviceChanValues = new NexText(pageId, baseId + 2, IdToComponentIndex(baseId + 2));
    picDeviceBoxes = new NexPicture(pageId, baseId + 3, IdToComponentIndex(baseId + 3));
    memset(_lastData, 0, sizeof(_lastData));
}

void DmxDeviceSlot::SetSlotEmpty()
{
    txtDeviceName->setVisible(false);
    txtDeviceDesc->setVisible(false);
    txtDeviceChanValues->setVisible(false);
    picDeviceBoxes->setVisible(false);
    currentInfo = nullptr;
}
void DmxDeviceSlot::SetSlotInfo_P(DmxDeviceInfo* info)
{
    currentInfo = info;
    if (info->IsEmpty_P())
        SetSlotEmpty();

    txtDeviceName->setVisible(true);
    txtDeviceDesc->setVisible(true);
    txtDeviceChanValues->setVisible(true);
    picDeviceBoxes->setVisible(true);

    DmxDeviceInfo copy;
    FromPGM(&copy, info);
    txtDeviceName->setText(copy.name);
    txtDeviceDesc->setText(copy.desc);
    txtDeviceChanValues->setText("");
    picDeviceBoxes->setPic(info->GetNumChannels_P() - 1);
    memset(_lastData, 0, sizeof(_lastData));
}

void DmxDeviceSlot::DisplayData(const uint8_t* data)
{
    if (currentInfo == nullptr || currentInfo->IsEmpty_P())
        return;

    char buf[32];
    char* pValues = buf;
    uint8_t* pData = data + currentInfo->GetChannel_P() - 1;
    
    int numChannels = currentInfo->GetNumChannels_P();
    if (numChannels < 1 || numChannels > 8)
    {
        dbSerialPrintf_P("Invalid Channel count %d for type %d\n", numChannels, currentInfo->GetType_P());
        return;
    }

    uint16_t chan = currentInfo->GetChannel_P() - 1;
    bool anyChange = false;
    for (int i = 0; i < numChannels; i++)
    {
        anyChange |= pData[i] != _lastData[i];
    }
    if (!anyChange)
        return;

    nexTraceActive = true;

//    picDeviceBoxes->refresh();
    for (int i = 0; i < numChannels; i++)
    {
        uint8_t v = *pData++;

        if (i > 0)      // prepend with space if not first
            *pValues++ = ' ';

        uint8_t* pEnd = pValues + 3;
        if (v < 100)    // prepend with space for hundreds
            *pValues++ = ' ';

        if (v < 10)    // prepend with space for tens
            *pValues++ = ' ';

        itoa(v, pValues, 10);        
        pValues = pEnd;
    }
    txtDeviceChanValues->setText(buf);      // Set the text values for the data

    pData = data + currentInfo->GetChannel_P() - 1;    // DMX channels are 1-based
    switch (currentInfo->GetType_P())
    {
        case DmxDeviceInfo::DeviceType::None:
            break;
        case DmxDeviceInfo::DeviceType::Generic:
            DisplayLinear(0, *pData);
            break;
        case DmxDeviceInfo::DeviceType::WW:
            DisplayWarmWhite(0, *pData);
            break;
        case DmxDeviceInfo::DeviceType::CW:
            DisplayCoolWhite(0, *pData);
            break;
        case DmxDeviceInfo::DeviceType::Amber:
            DisplayAmber(0, *pData);
            break;
        case DmxDeviceInfo::DeviceType::RGB:
            DisplayRGB(0, pData);
            break;
        case DmxDeviceInfo::DeviceType::RGBW:
            DisplayRGB(0, pData);
            DisplayWarmWhite(3, pData[3]);
            break;
        case DmxDeviceInfo::DeviceType::RGBWC:
            DisplayRGB(0, pData);
            DisplayWarmWhite(3, pData[3]);
            DisplayCoolWhite(4, pData[4]);
            break;
        case DmxDeviceInfo::DeviceType::RGBA:
            DisplayRGB(0, pData);
            DisplayAmber(3, pData[3]);
            break;
        case DmxDeviceInfo::DeviceType::RGBAC:
            DisplayRGB(0, pData);
            DisplayAmber(3, pData[3]);
            DisplayCoolWhite(4, pData[4]);
            break;
        case DmxDeviceInfo::DeviceType::DRGB:
            DisplayLinear(0, *pData);
            DisplayRGB(1, pData + 1);
            break;
        case DmxDeviceInfo::DeviceType::DRGBW:
            DisplayLinear(0, *pData);
            DisplayRGB(1, pData + 1);
            DisplayWarmWhite(4, pData[4]);
            break;
        case DmxDeviceInfo::DeviceType::DRGBWC:
            DisplayLinear(0, *pData);
            DisplayRGB(1, pData + 1);
            DisplayWarmWhite(4, pData[4]);
            DisplayCoolWhite(5, pData[5]);
            break;
        case DmxDeviceInfo::DeviceType::DRGBA:
            DisplayLinear(0, *pData);
            DisplayRGB(1, pData + 1);
            DisplayAmber(4, pData[4]);
            break;
        case DmxDeviceInfo::DeviceType::DRGBAC:
            DisplayLinear(0, *pData);
            DisplayRGB(1, pData + 1);
            DisplayAmber(4, pData[4]);
            DisplayCoolWhite(5, pData[5]);
            break;
        case DmxDeviceInfo::DeviceType::Gen7Chan:
            for (int i = 0; i < 7; i++)
                DisplayLinear(i, pData[i]);
            break;
        case DmxDeviceInfo::DeviceType::Gen8Chan:
            for (int i = 0; i < 8; i++)
                DisplayLinear(i, pData[i]);
            break;
    }

    memcpy(_lastData, pData, numChannels);
    nexTraceActive = false;

}

void DmxDeviceSlot::DisplayRGB(int chanSlot, const uint8_t* pData)
{
    pOwner->ChannelColor(slotId, 0, 3, DmxPageBase::RgbColorTo565(pData[0], pData[1], pData[2]));
}

void DmxDeviceSlot::DisplayWarmWhite(int chanSlot, uint8_t value)
{
    pOwner->ChannelColor(slotId, chanSlot, 1, DmxPageBase::ScaledRgbColorTo565(DmxPageBase::kRgbBaseWarmWhite, value));
}

void DmxDeviceSlot::DisplayAmber(int chanSlot, uint8_t value)
{
    pOwner->ChannelColor(slotId, chanSlot, 1, DmxPageBase::ScaledRgbColorTo565(DmxPageBase::kRgbBaseAmber, value));
}

void DmxDeviceSlot::DisplayCoolWhite(int chanSlot, uint8_t value)
{
    pOwner->ChannelColor(slotId, chanSlot, 1, DmxPageBase::RgbColorTo565(value, value, value));
}
void DmxDeviceSlot::DisplayLinear(int chanSlot, uint8_t value)
{
    pOwner->ChannelLinear(slotId, chanSlot, value);
}

char* DmxDeviceSlot::IdToComponentIndex(int id)
{
    char buf[10];
    strcpy(buf, "b[");
    itoa(id, buf+2, 10);
    strcat(buf,"]");

    return strdup(buf);
}

DmxDeviceSlots::DmxDeviceSlots(int pageId, int baseId, int count)
{
    deviceSlots = new DmxDeviceSlot*[count];
    numSlots = count;
    int id = baseId;
    for (int i = 0; i < count; i++)
    {
        deviceSlots[i] = new DmxDeviceSlot(this, i, pageId, baseId);
        baseId += 4;
    }

}

void DmxDeviceSlots::UpdateData(const uint8_t* pData)
{
    for (int i = 0; i < numSlots; i++)
    {
        deviceSlots[i]->DisplayData(pData);
    }
}

void DmxDeviceSlots::AssignMethods(NexHotspot* channelColor, NexHotspot* chanLinear)
{
    fnChannelColor = channelColor;
    fnChanLinear = chanLinear;
}
void DmxDeviceSlots::AssignVariables(NexVariable* span, NexVariable* color, NexVariable* devSlot, 
        NexVariable* chanSlot, NexVariable* value)
{
    vxSpan = span;
    vxColor = color;
    vxDevSlot = devSlot;
    vxChanSlot = chanSlot;
    vxValue = value;
}

void DmxDeviceSlots::ChannelColor(int devSlot, int chanSlot, int span, uint16_t color)
{
    vxDevSlot->setValue(devSlot);
    vxChanSlot->setValue(chanSlot);
    vxSpan->setValue(span);
    vxColor->setValue(color);
    DmxPageBase::SendClick(fnChannelColor, DmxPageBase::ClickType::ClickTouch);
}
void DmxDeviceSlots::ChannelLinear(int devSlot, int chanSlot, uint8_t value)
{
    vxDevSlot->setValue(devSlot);
    vxChanSlot->setValue(chanSlot);
    vxValue->setValue(value);
    DmxPageBase::SendClick(fnChanLinear, DmxPageBase::ClickType::ClickTouch);
}
