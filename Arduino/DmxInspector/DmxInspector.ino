//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#include <Nextion.h>
#include <Conceptinetics.h>
#include "DmxPageBase.h"
#include "DmxUniversePage.h"
#include "DmxDevicesPage.h"
#include "DmxSettingsPage.h"



DmxUniversePage* Page0;
DmxDevicesPage* Page1;
DmxSettingsPage* Page2;
DMX_Slave* DmxReader;

uint32_t timeLastDataReceived;
uint16_t lastChannelsReceived;
uint32_t timeLastDataProcessed;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  timeLastDataReceived = 0;
  timeLastDataProcessed = 0;
  lastChannelsReceived = 0;

  uint32_t timer = millis();


  DmxReader = new DMX_Slave(512);
  DMX_FrameBuffer frameBuffer = DmxReader->getBuffer();
  if (frameBuffer.getBufferSize() != 513)
  {
    dbSerialPrintf_P("Buffer size %d is not expected 513\n", frameBuffer.getBufferSize());
  }
  uint8_t* channelData = frameBuffer.getBufferZeroAligned();

  //memset(channelData, 0xff, 512);   // Set all channels to max for a quick test
  
  Page0 = new DmxUniversePage(channelData);
  Page1 = new DmxDevicesPage(channelData);
  Page2 = new DmxSettingsPage();
  
  // NOTE: Nextion baud rate is set in the pre-initialization event of Page0 in
  // the Nextion editor. The rate there must match the rate set here.
  
  //nexInit(115200L, false);  // was getting occasional missed draw calls at this speed
  nexInit(57600L, false);
  DmxPageBase::SetCurrentPage(Page0);

  DmxReader->onReceiveComplete(&OnDmxReceiveComplete);
  DmxReader->enable();

}

void loop() {
  // put your main code here, to run repeatedly:
  nexLoop(DmxPageBase::CurrentPage->ListenList);
  DmxPageBase::CurrentPage->OnLoop();

  bool newData = timeLastDataReceived > timeLastDataProcessed;
  uint16_t count = lastChannelsReceived;
  uint32_t currentTime = millis();

  if (newData)
  {
    dbSerialPrintf_P("New data received %d channels %ld ms ago", count, currentTime - timeLastDataReceived);
    timeLastDataProcessed = currentTime;
  }
}

void OnDmxReceiveComplete(uint16_t channelsReceived)
{
  timeLastDataReceived = millis();
  lastChannelsReceived = channelsReceived;
}
