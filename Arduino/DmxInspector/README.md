# DMX Inspector
The DMX Inspector is an Arduino-based device that will read out the data being sent on a DMX512 universe. This provides a developer of DMX control software and/or the author of sequences with a desktop debugging display that confirms the data being sent to the lighting without having to actually be connected to a universe of DMX devices.

## Operation
The Inspector needs a 9v power adapter to operate. You should not attempt to power the device from USB--the Arduino may run, but the display is powered by a 5V regulator and the combination of the two devices may try to draw too much power from USB. One should  always plug in the power before connecting it to USB when making code changes. In normal operation, there is no need to connect the USB at all.

Use a 5-pin male DMX connector to 3-pin female XLR cable to connect the Inspector to the DMX universe. The DMX Shield has a termination resistor for terminating the line.

## Universe Display
When connected, the display will show all 512 values being sent by the DMX controller. For defined devices, the display will show RGB colors or levels of warm white, cool white, or amber. For all channels not defined, they will display as levels of grey (same as cool white).

Also, at the bottom of the screen a single device's information is shown. Pressing `Dev +` and `Dev -` buttons will sequence through the defined devices in channel order. A white box will highlight the channel(s) used by the device on the upper grid display.

## Devices Display
Pressing the `Devices` button changes to the Devices view where up to 6 devices are shown per page. Pressing `Page +` and `Page -` will cycle through pages of devices in the order defined in the Devices definitions tables.

The device displays--both on this page and on the Universe Display page--print the current numerical value of each channel above a box that displays the color or, in the case of generic devices, a linear representation of the value--like a slider filling the box proportionally to the value.

## Building the Code

Install Arduino v1.8.9 (1.8.10 has an annoying log spew during build when used with VS Code)
* I tend to install this at the root, (ex: c:\arduino)

Clone the DMX project to a directory of your choice from https://github.com/microsoft/DMXInspector:
```
git clone https://github.com/microsoft/DMXInspector.git
```

Install the two libraries. It is easiest to just clone them directly into the Arduino libraries cache in your home directory, `c:\users\<username>\Arduino\libraries`.
* DMX library from https://github.com/johnseghersmsft/Conceptinetics_RDM
```
git clone https://github.com/johnseghersmsft/Conceptinetics_RDM
```
* Nextion library https://github.com/johnseghersmsft/ITEADLIB_Arduino_Nextion
```
git clone https://github.com/johnseghersmsft/ITEADLIB_Arduino_Nextion
```
Then tell either the Arduino IDE or the VS Code IDE to include each library in the project. This will add a bunch of superfluous `#include` statements into DmxInspector.ino, so just remove them.

Suggestion: Setup VS Code to work as Arduino Editor
* Install the Arduino Extension: https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino
* Install the C/C++ Extension.
* In Edit->Preferences, In the Extensions section:
  * Set the Arduino: Path to the directory wherein you installed Arduino
  * Set the C_Cpp: IntelliSense Engine to "Tag Parser"
* Use `Show All Commands`--usually mapped to `F1` or `Ctrl-Shift-P` to bring up the command search. All Arduino commands are prefixed with "Arduino:"
  * Use `Arduino: Change Board Type` and select the Arduino Mega 2560
  * Use `Arduino: Select Serial Port` and choose the correct COM port
  * Use `Arduino: Change Baud Rate` to set 115200 if you're using the Serial Port Monitor in VS Code. I tend to use the Serial Monitor in the Arduino IDE (just have the IDE open to provide the button) because when you open that monitor it forces a reset on the Arduino. This allows you to get any debugging messages from the beginning.

## Defining Devices
The code contains the definitions for fixtures in the DMX Universe to be displayed. Of course, this needs to be configured for the actual universes with which you are working. This is accomplished by editing `DmxDeviceInfo.cpp`.

The devices present in a DMX universe are defined in tables such as this one. In the `DmxDeviceInfo.cpp` file there are six such universes defined.

``` C++
file DmxDeviceInfo.cpp:

const DmxDeviceInfo devices1[] PROGMEM
{
    //           123456789012345678901
    { "31045",  "Rec Desk RGBA CStrip",    DmxDeviceInfo::DeviceType::RGBA,           45 },
    { "31027",  "Lby Crvd Wall Flr Str",   DmxDeviceInfo::DeviceType::CW,             27 },
    { "31051",  "Lby Abv Ceil RGB Up 1",   DmxDeviceInfo::DeviceType::RGB,            51 },
    { "31054",  "Lby Abv Ceil RGB Up 2",   DmxDeviceInfo::DeviceType::RGB,            54 },
    { "31057",  "Lby Abv Ceil RGB Up 3",   DmxDeviceInfo::DeviceType::RGB,            57 },
    // ... More device definitions
    { "104099", "CC WallWash WW Flood",    DmxDeviceInfo::DeviceType::WW,             99 },
    { "104100", "CC WallWash CW Flood",    DmxDeviceInfo::DeviceType::CW,            100 },
    { "", "", DmxDeviceInfo::DeviceType::None, -1 }
};
```
I've abbreviated the table for clarity here. Each device, or fixture, is defined by one row of this array. The data within the braces initializes the four data members of the `DmxDeviceInfo` structure:
* Name: Up to 8 characters to be used as a short identifier
* Description: Up to 21 characters as a description of the device
* Type: The type of device. This defines the number of channels used by the device and how the channel data will be displayed.
* Channel: The DMX Channel number (1-based) of the first channel used by the device. For example, the first device is an LED light with Red, Green, Blue, and Amber on channels 45, 46, 47, and 48 respectively.

The devices can be listed in any order regardless of their channel numbers. The order listed is the order they will display on the Devices page. On the Universes page, they are sorted by channel number so the `Dev +` and `Dev -` buttons will scan through the devices in the order they appear on the grid.

Each of these array definitions must end with the Empty device: `{ "", "", DmxDeviceInfo::DeviceType::None, -1 }`. These are arrays of objects and the Empty device serves as an end marker in much the same way a `nullptr` acts as the end of an array of pointers.

## Defining Universes
The data in the current code defines devices for six DMX universes. The active universe can be selected from the settings page accessed by the gear icon on the Universe page.

The array of available universes is defined at the bottom of `DmxDeviceInfo.cpp`:
``` C++
file DmxDeviceInfo.cpp:

const DmxUniverse DmxUniverse::universes[] PROGMEM
{
    { 1, "Lobby Office Touchdown", devices1},
    { 2, "Plaza Cafe Makerspace", devices2},
    { 3, "Porch + Misc", devices3},
    { 4, "Family Room, Dining Room", devices4},
    { 5, "Kitchen", devices5},
    { 6, "Conversation Room", devices6}
};
```

This array defines the universes and links a description with the devices array for that universe. The universe description can be up to 29 characters long.

### Changing the Number of Universes
If you add or remove universes, you must update a constant, `kNumUniverses`, that informs the rest of the code of the number of universes:
``` C++
struct DmxUniverse
{
    static const int kNumUniverses = 6;
    static const int kMaxNameLength = 30;

    int number; 
    char desc[kMaxNameLength];
    DmxDeviceInfo * devices;

    static const DmxUniverse universes[kNumUniverses];
};
```

## Stored In Program Memory Address Space
These tables take up a lot of memory, and if they were defined in the normal data address space, they would quickly fill the available RAM. Thus, the use of the `PROGMEM` macro is required. This also means that reading this information requires special handling in the code, but we'll look at that later.

## Creating New Device Types
The `DeviceType` enum is defined in `DmxDeviceInfo.h` and can be extended to define new kinds of fixtures. Doing so requires several changes to be made.

### Defining the Number of Channels
The number of channels used for each kind of device is specified in the array `channelsForType`.

``` C++
file DmxDeviceInfo.cpp:

const uint8_t DmxDeviceInfo::channelsForType[] {0, 1, 1, 1, 1, 3, 4, 5, 4, 5, 4, 5, 6, 5, 6, 7, 8};
```
This has one entry per value of the `DeviceType` enum. Note that the Device display code can only show 8 channels of information, so if your new fixture uses more than 8 channels, you could break it up into multiple types and then enter multiple devices in the device table for that single fixture.

## Drawing On the Universe Page

Now that you've defined a new type and the number of channels it uses, you need to define how to draw it.

The grid display on the Universe page shows one color box per channel. Lights that use more than one channel to define a single color can draw that color across multiple boxes, merging them together. For example, the RGB type displays the combined color as a rectangle spanning three boxes.

This is handled by code in the method `DmxUniversePage::DrawDevice_P`. I've cut out a bunch of the cases for this document.
``` C++
file DmxUniversePage.cpp

int DmxUniversePage::DrawDevice_P(DmxDeviceInfo* pInfo)
{
    int chan = pInfo->GetChannel_P() - 1; // DMX channels are 1-based
    int channels = pInfo->GetNumChannels_P();

    uint8_t* pData = _data + chan;

// ... code to only update if there's a change in the data
    
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
// ...
        case DmxDeviceInfo::DeviceType::RGB:
            chan = DisplayRGB(chan, pData);
            break;
// ...
        case DmxDeviceInfo::DeviceType::Gen8Chan:
            for (int i = 0; i < 8; i++)
                chan = DisplayGrey(chan, pData[i]);
            break;
    }
    memcpy(pLast, pData, channels);
    return chan - 1;        // return last channel actually drawn
}
```

There are a few things to note in this method. First, the `_P` suffix to the method. This indicates that it is expecting a pointer to Program Memory. The `pInfo` pointer is a pointer into the first 64K of the program address space, not a pointer to data space. Trying to use `pInfo->type` to get the device type won't work. The methods of the `DmxDeviceInfo` instead treat the `this` pointer appropriately, turning `pInfo->GetType_P()` into `pgm_read_byte(&this->type)`.

Next, this code reads the channel number and then immediately converts it from DMX's 1-based channels to a zero-based index suitable for use with pointers, and pData is computed to point to the data for the first channel of the device.

The `switch` decides how to draw the data. This uses various Display*Type* methods. We'll examine a couple of those in a moment. Each of these methods takes the `chan` value as an input for *where* to display the data and returns an updated `chan` value for where the *next* channel should be drawn. The `DrawDevice_P` method returns the chan value for the last channel actually drawn by this code (or in the case of `None` the channel skipped).

It should be noted that the `pData` pointer should not be changed during drawing since it is used at the end to record the channel data drawn in the buffer that is used for detecting changes. Communications with the Nextion Display makes updating the display an expensive operation, so the drawing code only updates the display for channels whose data has changed.

### Drawing Primatives
Drawing on the grid makes use of a single primative drawing method on the Nextion display. Each UI component defined on a page (of which there can only be 255 per page) has two event handlers. One is called when the user pushes on the component, the other when the user releases the touch. Nextion calls these Push and Pop. I call them Touch and Release.

The drawing code makes use of the ability to call these event handers directly via the `click` Nextion instruction. There are `NexHotspot` components on the pages to implement various drawing methods in their event handlers. The hotspots have no visual representation and the page's initialization event disables them from user touches.

### Implementing Draw Methods
``` C++
file DmxUniversePage.cpp:

int DmxUniversePage::DisplayRGB(int chan, const uint8_t* pData)
{
    DrawColor(chan, 3, RgbColorTo565(pData[0], pData[1], pData[2]));
    return chan + 3;
}

int DmxUniversePage::DisplayAmber(int chan, uint8_t value)
{
    DrawColor(chan, 1, ScaledRgbColorTo565(kRgbBaseAmber, value));
    return chan + 1;
}
void DmxUniversePage::DrawColor(uint16_t channelNumber, uint16_t span, uint16_t color)
{
    vxChan->setValue(channelNumber);
    vxNum->setValue(span);
    vxColor->setValue(color);
    SendClick(fnDrawColor, ClickTouch);
}
```
Here we see two of the drawing methods `DisplayRGB` and `DisplayAmber`. The each call `DrawColor` to communicate the draw command to the event handler of the `drawColor` Hotspot object on the Nextion display.

`DrawColor` works by setting the channel number and span to the Nextion variables `chan` and `num` and sets the 16-bit RGB565 color to the `color` Nextion variable. It then sends the Touch event to the `drawColor` Hotspot component.

The code in this event hander knows how to align the drawing to the correct channel box, and how to span multiple boxes, even when it will wrap to the next row.

### Color Handling
Colors on the Nextion display use RGB565 encoding, using the most significant 5 bits for 32 shades of red, the next 6 bits for 64 shades of green, and the least significant 5 bits for 32 shades of blue.

The colors defined by a three-channel RGB light, however, use 8-bits of data for each of the three colors. The `DisplayRGB` method uses a utility method, `RgbColorTo565` to convert these three values into a color the Nextion can display.

`DisplayAmber` needs to display the channel value as different brightnesses of a color. `ScaledRgbColorTo565` converts a 24-bit RGB value (in the low 3 bytes of a 32-bit int) into a 565 representation of the color after being multiplied by the channel value. A channel value of 255 equates to multiplying by 1.

### Multiple Draw Calls
More complex devices will span multiple boxes drawing different colors into multiple positions. Here we see two such:
``` C++
        case DmxDeviceInfo::DeviceType::RGBAC:
            chan = DisplayRGB(chan, pData);
            chan = DisplayAmber(chan, pData[3]);
            chan = DisplayCoolWhite(chan, pData[4]);
            break;
        // ...
        case DmxDeviceInfo::DeviceType::DRGBAC:
            chan = DisplayGrey(chan, *pData);
            chan = DisplayRGB(chan, pData + 1);
            chan = DisplayAmber(chan, pData[4]);
            chan = DisplayCoolWhite(chan, pData[5]);
            break;
```
These are for two device types. Both have RGB, Amber, and Cool White components, but the `DRGBAC` adds a dimmer at the start of the channels. Notice how the `DisplayRGB`, `DisplayAmber`, and `DisplayCoolWhite` methods are almost identical, with the data being passed shifted by one channel.

And alternative implementation might use the dimmer value to scale the RGB color and display that across the 4 channels.

Note that the `chan` variable is updated after each Display*Type* call and used in the next call. If this variable is not updated correctly some channels may be skipped (chan set too high), or the drawing loop may stall in an endless loop if the chan value is too low.

## Drawing On the Device Page
Drawing on the Devices Page (and also on the single device drawn on the bottom of the Universe Page) is handled by the `DmxDeviceSlot` and `DmxDeviceSlots` classes. The code is very similar to that of the Universe Page, but there are some additional options available.

Drawing the channel data consists of two parts, both implemented in `DmxDeviceSlot::DisplayData`. `DmxDeviceSlot` represents the display of a single device. Which device that is varies as you scroll through devices with the `Page +` and `Page -` buttons (`Dev +` and `Dev -` on the Universe Page).

### Channel Values As Text
The first part of the method creates a string consisting of the numeric values of each channel. These are aligned to display above the channel boxes.
``` C++
File DmxDeviceSlot.cpp:

oid DmxDeviceSlot::DisplayData(const uint8_t* data)
{
//...
    char buf[32];
    char* pValues = buf;
    uint8_t* pData = data + currentInfo->GetChannel_P() - 1;
    
    int numChannels = currentInfo->GetNumChannels_P();
//...
    uint16_t chan = currentInfo->GetChannel_P() - 1;
//...
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
```
The string formatted into `buf` will have up to 8 numbers, each taking a constant 3 characters each, and up to 7 spaces between them for a maximum length of 31 characters plus the null character at the end. It would be possible to extend this to take into account devices that combine two channels into 16-bit values. Just be sure the resultant string fits within the buffer. The text component on the Nextion display is expecting this size as well.

### Filling the Channel Boxes
The same fill options are available as on the Universe Page, with the addition of another: `DisplayLinear` that fills the box from the bottom up with white proportionately to the value of the channel.

Comparing one of the drawing primitives from Universe Page to that of a Device Slot illustrates the differences:
``` C++
File DmxUniversePage.cpp:

int DmxUniversePage::DisplayRGB(int chan, const uint8_t* pData)
{
    DrawColor(chan, 3, RgbColorTo565(pData[0], pData[1], pData[2]));
    return chan + 3;
}

File DmxDeviceSlot.cpp:
void DmxDeviceSlot::DisplayRGB(int chanSlot, const uint8_t* pData)
{
    pOwner->ChannelColor(slotId, 0, 3, DmxPageBase::RgbColorTo565(pData[0], pData[1], pData[2]));
}
```

As mentioned above, each slot knows what device it is drawing, and is told what position on the page it holds. This position is the `slotId` with which it is constructed. On the Devices Page, this ranges from 0-5. On the Universe Page there is only one slot with an id of 0.

The `ChannelColor` method is implemented in the `DmxDeviceSlots` class that contains the slots. `DmxDeviceSlots` in turn gets the Nextion Variables and Hotspots for implementing the drawing from the host pages. The drawing methods in the Nextion event handlers convert a slot number and channel number into the appropriate positions on their respective pages.

