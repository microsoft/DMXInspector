//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#include "DmxUniversePage.h"
#include "DmxDevicesPage.h"
#include "DmxSettingsPage.h"


DmxDevicesPage::DmxDevicesPage(uint8_t* data)
    : DmxPageBase(kDevicePageId, "page1", data)
{
    btnToUniverse = new NexButton(kDevicePageId, 44, "b0");
    btnPagePlus = new NexButton(kDevicePageId, 43, "pagePlus");
    btnPageMinus = new NexButton(kDevicePageId, 42, "pageMinus");

    txtPageNum = new NexText(kDevicePageId, 41, "pageNumText");

    fnChannelColor = new NexHotspot(kDevicePageId, 27, "channelColor");
    fnChanLinear = new NexHotspot(kDevicePageId, 39, "chanLinear");

    vxSpan = new NexVariable(kDevicePageId, 30, "span");
    vxColor = new NexVariable(kDevicePageId, 31, "color");
    vxDevSlot = new NexVariable(kDevicePageId, 32, "devSlot");
    vxChanSlot = new NexVariable(kDevicePageId, 33, "chanSlot");
    vxValue = new NexVariable(kDevicePageId, 40, "value");
    vxPageNumber = new NexVariable(kDevicePageId, 45, "pageNumber");

    deviceSlots = new DmxDeviceSlots(kDevicePageId, kDeviceNameBaseId, kDevicesPerPage);
    deviceSlots->AssignMethods(fnChannelColor, fnChanLinear);
    deviceSlots->AssignVariables(vxSpan, vxColor, vxDevSlot, vxChanSlot, vxValue);


    btnToUniverse->attachPush(&OnToUniversePushed, this);
    btnPagePlus->attachPush(&OnPagePlusPushed, this);
    btnPageMinus->attachPush(&OnPageMinusPushed, this);

    ListenList = new NexTouch*[4] { btnToUniverse, btnPagePlus, btnPageMinus, NULL };
}

void DmxDevicesPage::OnLoop()
{
    DmxPageBase::OnLoop();
    deviceSlots->UpdateData(_data);
}

void DmxDevicesPage::OnNavigatedTo()
{
    nexTraceActive = true;

    topDevice = 0;
    numDevices = DmxDeviceInfo::CountDevicesInArray_P(DmxSettingsPage::currentUniverse.devices);

    PopulateDeviceSlots();
}

void DmxDevicesPage::PopulateDeviceSlots()
{
    int numVisible = min(numDevices - topDevice, kDevicesPerPage);

    DmxDeviceInfo* pInfo = DmxSettingsPage::currentUniverse.devices + topDevice;
    DmxDeviceSlot** pSlot = deviceSlots->deviceSlots;
    int i;
    for (i = 0; i < numVisible; i++)
    {
        (*pSlot++)->SetSlotInfo_P(pInfo++);
    }
    while (i++ < kDevicesPerPage)
    {
        (*pSlot++)->SetSlotEmpty();
    }
}
void DmxDevicesPage::OnToUniversePushed(void* p)
{
    SetCurrentPage(Pages[DmxUniversePage::kUniversePageId]);
}

void DmxDevicesPage::OnPagePlusPushed(void* p)
{
    DmxDevicesPage* pthis = (DmxDevicesPage*)p;
    pthis->PageChanged(1);
}
void DmxDevicesPage::OnPageMinusPushed(void* p)
{
    DmxDevicesPage* pthis = (DmxDevicesPage*)p;
    pthis->PageChanged(-1);
}
void DmxDevicesPage::PageChanged(int dir)
{
    int32_t page = 1;
    if (!vxPageNumber->getValue((uint32_t*)&page))
    {
        SetPageNumber(1);
        page = 1;
    }
    --page;     // convert to zero-based page number

    if (page < 0)
    {
        page = 0;
        SetPageNumber(1);
    }

    topDevice = page * kDevicesPerPage;
    if (topDevice >= numDevices)
    {

        SetPageNumber(page--);      // since Display uses 1-based page numbers
                                    // this will set the pre-decrement value as
                                    // the new page number
        topDevice -= kDevicesPerPage;
    }

    PopulateDeviceSlots();
}

void DmxDevicesPage::SetPageNumber(int page)
{
    vxPageNumber->setValue(page);
    SendClick(txtPageNum, ClickTouch);      // update display
}


