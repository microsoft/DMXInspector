//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#include "DmxUniversePage.h"
#include "DmxDevicesPage.h"
#include "DmxSettingsPage.h"


DmxUniversePage::DmxUniversePage(uint8_t* data)
    : DmxPageBase(kUniversePageId, "page0", data)
{
    memset(_lastData, 0, 512);

    btnToDevices = new NexButton(kUniversePageId, 16, "b0");
    btnToSettings = new NexButton(kUniversePageId, 23, "settings");
    btnDevMinus = new NexButton(kUniversePageId, 31, "devMinus");
    btnDevPlus = new NexButton(kUniversePageId, 32, "devPlus");


    fnDrawGrid = new NexHotspot(kUniversePageId, 7, "drawGrid");
    fnDrawColor = new NexHotspot(kUniversePageId, 14, "drawColor");
    fnDrawChanBox = new NexHotspot(kUniversePageId, 33, "drawChanBox");

    txtTitle = new NexText(kUniversePageId, 17, "title");

    vxChan = new NexVariable(kUniversePageId, 1, "chan");
    vxNum = new NexVariable(kUniversePageId, 2, "num");
    vxColor = new NexVariable(kUniversePageId, 3, "color");
    vxGridColor = new NexVariable(kUniversePageId, 9, "gridColor");

    fnChannelColor = new NexHotspot(kUniversePageId, 29, "channelColor");
    fnChanLinear = new NexHotspot(kUniversePageId, 30, "chanLinear");

    vxSpan = new NexVariable(kUniversePageId, 27, "span");
    vxDevSlot = new NexVariable(kUniversePageId, 24, "devSlot");
    vxChanSlot = new NexVariable(kUniversePageId, 25, "chanSlot");
    vxValue = new NexVariable(kUniversePageId, 26, "value");

    btnToDevices->attachPush(&OnToDevicesPushed, this);
    btnToSettings->attachPush(&OnToSettingsPushed, this);

    btnDevMinus->attachPush(&OnDevMinusPushed, this);
    btnDevPlus->attachPush(&OnDevPlusPushed, this);

    deviceSlot = new DmxDeviceSlots(kUniversePageId, 18, 1);
    deviceSlot->AssignMethods(fnChannelColor, fnChanLinear);
    deviceSlot->AssignVariables(vxSpan, vxColor, vxDevSlot, vxChanSlot, vxValue);

    ListenList = new NexTouch*[5] { btnToDevices, btnToSettings, btnDevMinus, btnDevPlus, NULL };
    _fullUpdateInProgress = true;
    _channelSortedDevices = nullptr;
    _ppDeviceIterator = nullptr;
    _deviceCount = 0;
}

void DmxUniversePage::OnToDevicesPushed(void* p)
{
    SetCurrentPage(Pages[DmxDevicesPage::kDevicePageId]);
}

void DmxUniversePage::OnToSettingsPushed(void* p)
{
    SetCurrentPage(Pages[DmxSettingsPage::kSettingsPageId]);
}

void DmxUniversePage::OnDevMinusPushed(void* p)
{
    ((DmxUniversePage*)p)->OnDevChange(-1);
}

void DmxUniversePage::OnDevPlusPushed(void* p)
{
    ((DmxUniversePage*)p)->OnDevChange(1);
}

void DmxUniversePage::OnDevChange(int plusOrMinus)
{
    SetCurrentDevice(_currentDevice + plusOrMinus);
}

void DmxUniversePage::DrawGrid()
{
    SendClick(fnDrawGrid, ClickTouch);
}
void DmxUniversePage::DrawColor(uint16_t channelNumber, uint16_t span, uint16_t color)
{
    vxChan->setValue(channelNumber);
    vxNum->setValue(span);
    vxColor->setValue(color);
    SendClick(fnDrawColor, ClickTouch);
}

void DmxUniversePage::DrawChanBox(uint16_t channelNumber, uint16_t span, uint16_t color)
{
    vxChan->setValue(channelNumber);
    vxNum->setValue(span);
    vxColor->setValue(color);
    SendClick(fnDrawChanBox, ClickTouch);
}

void DmxUniversePage::UpdateData()
{
    if (_ppDeviceIterator == nullptr)
        return;

    ReadMinMaxChannels_P(*_ppDeviceIterator);
    // iterate the data buffer sending any changes to the display
    int updateEnd = (_updateIterator + 20) % 512;

    for (int chan = _updateIterator; chan != updateEnd; chan = (chan + 1) % 512)
    {
        if (chan >= _maxChannel)
        {
            _ppDeviceIterator++;
            ReadMinMaxChannels_P(*_ppDeviceIterator);
        }
        if (chan < _minChannel)
        {
            uint8_t c = _data[chan];
            if (_fullUpdateInProgress || c != _lastData[chan])
            {
                DrawColor(chan, 1, RgbColorTo565(c, c, c));
                _lastData[chan] = c;
            }
        }
        else
        {
            chan = DrawDevice_P(*_ppDeviceIterator);
            if (updateEnd >= _minChannel && updateEnd < _maxChannel)
            {
                updateEnd = _maxChannel % 512;
            }
        }

        if (chan == 511)
        {
            _ppDeviceIterator = _channelSortedDevices;
            ReadMinMaxChannels_P(*_ppDeviceIterator);
        }
    }
    if (_fullUpdateInProgress && updateEnd < _updateIterator)
    {
        _fullUpdateInProgress = false;
        txtTitle->setText(DmxSettingsPage::currentUniverse.desc);
        txtTitle->Set_font_color_pco(kColorFontGreen);
        txtTitle->Set_background_color_bco(kColorBlack);
    }
    _updateIterator = updateEnd;
}

int DmxUniversePage::DrawDevice_P(DmxDeviceInfo* pInfo)
{
    int chan = pInfo->GetChannel_P() - 1; // DMX channels are 1-based
    int channels = pInfo->GetNumChannels_P();

    uint8_t* pData = _data + chan;
    uint8_t* pLast = _lastData + chan;
    bool anyChange = _fullUpdateInProgress;
    for (int i = 0; i < channels; i++)
    {
        anyChange |= pData[i] != pLast[i];
        //dbSerialPrintf_P("Change Check %d[%d] new %d old %d anyChange %d", chan, i, pData[i], pLast[i], anyChange);
    }
    
    if (!anyChange)
    {
        return chan + channels - 1;     // return last channel that would have been drawn
    }
    
    switch (pInfo->GetType_P())
    {
        case DmxDeviceInfo::DeviceType::None:
            chan++;
            break;
        case DmxDeviceInfo::DeviceType::Generic:
            chan = DisplayGrey(chan, *pData);
            break;
        case DmxDeviceInfo::DeviceType::WW:
            chan = DisplayWarmWhite(chan, *pData);
            break;
        case DmxDeviceInfo::DeviceType::CW:
            chan = DisplayCoolWhite(chan, *pData);
            break;
        case DmxDeviceInfo::DeviceType::Amber:
            chan = DisplayAmber(chan, *pData);
            break;
        case DmxDeviceInfo::DeviceType::RGB:
            chan = DisplayRGB(chan, pData);
            break;
        case DmxDeviceInfo::DeviceType::RGBW:
            chan = DisplayRGB(chan, pData);
            chan = DisplayWarmWhite(chan, pData[3]);
            break;
        case DmxDeviceInfo::DeviceType::RGBWC:
            chan = DisplayRGB(chan, pData);
            chan = DisplayWarmWhite(chan, pData[3]);
            chan = DisplayCoolWhite(chan, pData[4]);
            break;
        case DmxDeviceInfo::DeviceType::RGBA:
            chan = DisplayRGB(chan, pData);
            chan = DisplayAmber(chan, pData[3]);
            break;
        case DmxDeviceInfo::DeviceType::RGBAC:
            chan = DisplayRGB(chan, pData);
            chan = DisplayAmber(chan, pData[3]);
            chan = DisplayCoolWhite(chan, pData[4]);
            break;
        case DmxDeviceInfo::DeviceType::DRGB:
            chan = DisplayGrey(chan, *pData);
            chan = DisplayRGB(chan, pData + 1);
            break;
        case DmxDeviceInfo::DeviceType::DRGBW:
            chan = DisplayGrey(chan, *pData);
            chan = DisplayRGB(chan, pData + 1);
            chan = DisplayWarmWhite(chan, pData[4]);
            break;
        case DmxDeviceInfo::DeviceType::DRGBWC:
            chan = DisplayGrey(chan, *pData);
            chan = DisplayRGB(chan, pData + 1);
            chan = DisplayWarmWhite(chan, pData[4]);
            chan = DisplayCoolWhite(chan, pData[5]);
            break;
        case DmxDeviceInfo::DeviceType::DRGBA:
            chan = DisplayGrey(chan, *pData);
            chan = DisplayRGB(chan, pData + 1);
            chan = DisplayAmber(chan, pData[4]);
            break;
        case DmxDeviceInfo::DeviceType::DRGBAC:
            chan = DisplayGrey(chan, *pData);
            chan = DisplayRGB(chan, pData + 1);
            chan = DisplayAmber(chan, pData[4]);
            chan = DisplayCoolWhite(chan, pData[5]);
            break;
        case DmxDeviceInfo::DeviceType::Gen7Chan:
            for (int i = 0; i < 7; i++)
                chan = DisplayGrey(chan, pData[i]);
            break;
        case DmxDeviceInfo::DeviceType::Gen8Chan:
            for (int i = 0; i < 8; i++)
                chan = DisplayGrey(chan, pData[i]);
            break;
    }
    memcpy(pLast, pData, channels);
    return chan - 1;        // return last channel actually drawn
}

int DmxUniversePage::DisplayRGB(int chan, const uint8_t* pData)
{
    DrawColor(chan, 3, RgbColorTo565(pData[0], pData[1], pData[2]));
    return chan + 3;
}

int DmxUniversePage::DisplayWarmWhite(int chan, uint8_t value)
{
    DrawColor(chan, 1, ScaledRgbColorTo565(kRgbBaseWarmWhite, value));
    return chan + 1;
}
int DmxUniversePage::DisplayAmber(int chan, uint8_t value)
{
    DrawColor(chan, 1, ScaledRgbColorTo565(kRgbBaseAmber, value));
    return chan + 1;
}

int DmxUniversePage::DisplayCoolWhite(int chan, uint8_t value)
{
    DrawColor(chan, 1, RgbColorTo565(value, value, value));
    return chan + 1;
}
int DmxUniversePage::DisplayGrey(int chan, uint8_t value)
{
    DrawColor(chan, 1, RgbColorTo565(value, value, value));
    return chan + 1;
}


void DmxUniversePage::ReadMinMaxChannels_P(DmxDeviceInfo* pInfo)
{
    if (pInfo != nullptr)
    {
        _minChannel = pInfo->GetChannel_P() - 1;
        _maxChannel = _minChannel + pInfo->GetNumChannels_P();
        return;
    }
    _minChannel = _maxChannel = 512;
}


void DmxUniversePage::OnNavigatedTo()
{
    nexTraceActive = false;
    memset(_lastData, 0, 512);

    SortDevices();

    Serial.println(F("Resetting deviceIterator"));
    _updateIterator = 0;
    _ppDeviceIterator = _channelSortedDevices;
    PrintDeviceInfo_P(*_ppDeviceIterator);

    _fullUpdateInProgress = true;
    SetCurrentDevice(0);
}
void DmxUniversePage::SetCurrentDevice(int device)
{
    if (device < 0)
        device = 0;
    else if (device >= _sortedCount)
        device = _sortedCount - 1;

    // Revert old device border to normal grid DisplayGrey
    DmxDeviceInfo* pInfo = _channelSortedDevices[_currentDevice];
    int chan = pInfo->GetChannel_P() - 1;
    int nChannels = pInfo->GetNumChannels_P();
    uint8_t* pData = _data + chan;
    uint8_t* pLast = _lastData + chan;
    for (int i = 0; i < nChannels; i++)
    {
        DrawChanBox(chan++, 1, kGridGrey);
        *pLast++ = *pData++ ^ 0xff;       // force a redraw next iteration
    }

    _currentDevice = device;
    pInfo = _channelSortedDevices[_currentDevice];
    (*deviceSlot->deviceSlots)->SetSlotInfo_P(pInfo);

    // Draw selection border
    chan = pInfo->GetChannel_P() - 1;
    nChannels = pInfo->GetNumChannels_P();
    DrawChanBox(chan, nChannels, kGridSelectedChannel);

}


void DmxUniversePage::OnLoop()
{
    DmxPageBase::OnLoop();
    UpdateData();
    deviceSlot->UpdateData(_data);
}

void DmxUniversePage::SortDevices()
{
    int count = DmxDeviceInfo::CountDevicesInArray_P(DmxSettingsPage::currentUniverse.devices);
    if (count > _deviceCount)
    {
        delete _channelSortedDevices;
        _channelSortedDevices = new DmxDeviceInfo*[count + 1];
        _deviceCount = count;
    }
    
    _sortedCount = 0;
    for (DmxDeviceInfo* pInfo = DmxSettingsPage::currentUniverse.devices; !pInfo->IsEmpty_P(); pInfo++)
    {
        InsertSorted(pInfo);
    }
    _channelSortedDevices[count] = nullptr;

}

void DmxUniversePage::PrintDeviceInfo_P(DmxDeviceInfo* pInfo)
{
    if (pInfo == nullptr)
    {
        Serial.println(F("DeviceInfo is null\n"));
        return;
    }

    char name[9];
    char desc[22];
    strcpy_P(name, pInfo->name);
    strcpy_P(desc, pInfo->desc);
    name[8] = 0;
    desc[21] = 0;
    int channel = pInfo->GetChannel_P();
    dbSerialPrintf_P("%3d[%d] %d: %9s %s\n", channel, pInfo->GetNumChannels_P(), pInfo->GetType_P(), name, desc);
}

void DmxUniversePage::InsertSorted(DmxDeviceInfo* pInfo)
{
    int channel = pgm_read_word_near(&pInfo->channel);
    DmxDeviceInfo** high = _channelSortedDevices + _sortedCount;
    DmxDeviceInfo** low = _channelSortedDevices;
 
    while (low < high)
    {
        DmxDeviceInfo** test = low + ((high - low) / 2);
        if (channel >= pgm_read_word_near(&(*test)->channel))
            low = test + 1;
        else
            high = test;
    }

    high = _channelSortedDevices + _sortedCount;
    // insert at low
    if (low < high)
    {
        memmove(low + 1, low, (high - low) * sizeof(DmxDeviceInfo*));
    }
    *low = pInfo;
    _sortedCount++;
}