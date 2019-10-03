//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#include "DmxPageBase.h"
#include "DmxDeviceInfo.h"

class DmxSettingsPage : public DmxPageBase
{
    public:
        static const int kSettingsPageId = 2;

        NexText* txtSelected;
        NexText* txtDesc;

        NexButton* btnSelections[DmxUniverse::kNumUniverses];
        NexButton* btnBack;

        NexHotspot* fnSetNumButtons;

        NexVariable* vxNumButtons;

        DmxSettingsPage();

        void OnNavigatedTo() override;

        static uint8_t selectedUniverse;
        static DmxUniverse currentUniverse;

    private:
        static void OnBackPushed(void* p);
        static void OnSelect(void* p);
        static char* ButtonNumberToName(int id);

        void ShowCurrentUniverse();

};