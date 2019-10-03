//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************

#pragma once
#include <Nextion.h>

class DmxPageBase
{
    public:
        static const int kNumPages = 3;
        static const int kAnimatedChannels = 10;
        static const uint16_t kColorFontGreen = 2032;
        static const uint16_t kColorBlack = 0;
        static const uint16_t kColorWhite = 0xffff;

        static const uint32_t kRgbBaseAmber = 0xFFE100L;
        static const uint32_t kRgbBaseWarmWhite = 0xFFF5DDL;

        NexPage* Page;

        NexTouch** ListenList;

        DmxPageBase(int pageId, const char* pageName, uint8_t* data);
        virtual void OnNavigatedTo();

        virtual void OnLoop();

        enum ClickType
        {
            ClickRelease = 0,
            ClickTouch = 1
        };
        static void SendClick(NexTouch* target, ClickType touchOrRelease);
        static uint16_t RgbColorTo565(uint8_t red, uint8_t green, uint8_t blue);
        static uint16_t RgbColorTo565(uint32_t rgbColor);
        static uint16_t ScaledRgbColorTo565(uint32_t rgbColor, uint8_t scale);

        static void SetCurrentPage(DmxPageBase* page)
        {
            CurrentPage = page;
            page->OnNavigatedTo();
        }
        static DmxPageBase* CurrentPage;
        static DmxPageBase* Pages[kNumPages];

        struct ChannelAnimatorData
        {
            uint16_t Channel;
            int8_t Rate;
        };
        
        static const ChannelAnimatorData AnimatorData[kAnimatedChannels];

    protected:
        uint8_t* _data;
        static uint8_t  _animatorFracs[kAnimatedChannels];

};
