//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#include "DmxPageBase.h"


DmxPageBase* DmxPageBase::CurrentPage;
DmxPageBase* DmxPageBase::Pages[kNumPages];

DmxPageBase::DmxPageBase(int pageId, const char* pageName, uint8_t* data)
{
    Page = new NexPage(pageId, 0, pageName);
    Pages[pageId] = this;
    _data = data;
}

void DmxPageBase::OnNavigatedTo()
{
    
}

void DmxPageBase::OnLoop()
{
    // const ChannelAnimatorData * pAD = AnimatorData;
    // uint8_t* pFrac = _animatorFracs;
    // for (int i = 0; i < kAnimatedChannels; i++)
    // {
    //     uint16_t data = _data[pAD->Channel-1] << 4;
    //     data += *pFrac;
    //     data += pAD->Rate;
    //     *pFrac = data & 0xf;
    //     _data[pAD->Channel-1] = (data >> 4) & 0xff;
    //     pFrac++;
    //     pAD++;
    // }
}

void DmxPageBase::SendClick(NexTouch* target, ClickType touchOrRelease)
{
    char buf[20] = "click ";
    itoa(target->getObjCid(), buf + 6, 10);
    strcat(buf, touchOrRelease == ClickTouch ? ",1" : ",0");
    sendCommandWait(buf, 1000);
}

// Form RGB565 from 8-bit components
uint16_t DmxPageBase::RgbColorTo565(uint8_t red, uint8_t green, uint8_t blue)
{
    return (( red & 0xf8) << 8) | 
           (( green & 0xfc) << 3) |
           (( blue >> 3) & 0x1f);
}

uint16_t DmxPageBase::RgbColorTo565(uint32_t rgbColor)
{
    return (( rgbColor & 0xF80000L) >> 8) |
           (( rgbColor & 0x00FC00L) >> 5) |
           (( rgbColor & 0x0000F8L) >> 3);
}

#define MULTDIV_8(a, m, d) ((uint8_t)((((uint16_t)a) * m) / d))
uint16_t DmxPageBase::ScaledRgbColorTo565(uint32_t rgbColor, uint8_t scale)
{
    uint8_t red = (rgbColor >> 16) & 0xFF;
    uint8_t green = (rgbColor >> 8) & 0xFF;
    uint8_t blue = rgbColor & 0xFF;
    
    red = MULTDIV_8(red, scale, 255);
    green = MULTDIV_8(green, scale, 255);
    blue = MULTDIV_8(blue, scale, 255);
    return RgbColorTo565(red, green, blue);
}


const DmxPageBase::ChannelAnimatorData DmxPageBase::AnimatorData[DmxPageBase::kAnimatedChannels] = 
{
    { 45, 0x08 },
    { 48, 0x0c },
    { 27, 0x10 },
    { 51, 0x20 },
    { 52, 0x30 },
    { 53, -0x08 },
    { 54, -0x0c },
    { 55, -0x10 },
    { 56, -0x20 },
    {511, -0x30 }
};

uint8_t DmxPageBase::_animatorFracs[DmxPageBase::kAnimatedChannels];
