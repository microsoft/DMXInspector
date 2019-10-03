//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#include "DmxSettingsPage.h"
#include "DmxUniversePage.h"

struct ButtonThunk
{
    DmxSettingsPage* pPage;
    uint8_t buttonIndex;

    ButtonThunk(DmxSettingsPage* page, uint8_t index)
    {
        pPage = page;
        buttonIndex = index;
    }
};

DmxUniverse DmxSettingsPage::currentUniverse;
uint8_t DmxSettingsPage::selectedUniverse;

DmxSettingsPage::DmxSettingsPage()
    : DmxPageBase(kSettingsPageId, "page2", nullptr)
{
    btnBack = new NexButton(kSettingsPageId, 17, "btnBack");
    fnSetNumButtons = new NexHotspot(kSettingsPageId, 19, "setNumButtons");

    txtSelected = new NexText(kSettingsPageId, 21, "selected");
    txtDesc = new NexText(kSettingsPageId, 22, "desc");

    vxNumButtons = new NexVariable(kSettingsPageId, 20, "numButtons");

    ListenList = new NexTouch*[DmxUniverse::kNumUniverses + 2];
    NexTouch** pListen = ListenList;
    for (int i = 0; i < DmxUniverse::kNumUniverses; i++)
    {
        btnSelections[i] = new NexButton(kSettingsPageId, i + 1, ButtonNumberToName(i));
        btnSelections[i]->attachPush(&OnSelect, new ButtonThunk(this, i));
        *pListen++ = btnSelections[i];
    }
    btnBack->attachPush(&OnBackPushed, this);
    *pListen++ = btnBack;
    *pListen = nullptr;

    selectedUniverse = 0;

    FromPGM(&currentUniverse, &DmxUniverse::universes[0]);

}

char* DmxSettingsPage::ButtonNumberToName(int id)
{
    char buf[4] = "b";
    itoa(id, buf + 1, 10);
    return strdup(buf);
}

void DmxSettingsPage::OnBackPushed(void* p)
{
    SetCurrentPage(Pages[DmxUniversePage::kUniversePageId]);
}

void DmxSettingsPage::OnSelect(void* p)
{
    ButtonThunk* thunk = (ButtonThunk*)p;
    selectedUniverse = thunk->buttonIndex;

    FromPGM(&currentUniverse, &DmxUniverse::universes[selectedUniverse]);
    thunk->pPage->ShowCurrentUniverse();
}

void DmxSettingsPage::OnNavigatedTo()
{
    vxNumButtons->setValue(DmxUniverse::kNumUniverses);
    SendClick(fnSetNumButtons, ClickTouch);
    ShowCurrentUniverse();
}

void DmxSettingsPage::ShowCurrentUniverse()
{
    char buf[30];
    sprintf_P(buf, PSTR("Universe %d"), selectedUniverse + 1);
    txtSelected->setText(buf);
    txtDesc->setText(currentUniverse.desc);
}