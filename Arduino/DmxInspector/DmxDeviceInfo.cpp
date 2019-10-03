//*********************************************************
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
//
//*********************************************************
#include "DmxDeviceInfo.h"

uint8_t DmxDeviceInfo::GetNumChannels_P()
{
    // this pointer is to Program Memory
    DeviceType t = pgm_read_byte(&this->type);
    return channelsForType[t];
}

bool DmxDeviceInfo::IsEmpty_P()
{
    // this pointer is to Program Memory
    DeviceType t = pgm_read_byte(&this->type);
    return t == DeviceType::None;
}

uint16_t DmxDeviceInfo::GetChannel_P()
{
    // this pointer is to Program Memory
    return pgm_read_word(&this->channel);
}

DmxDeviceInfo::DeviceType DmxDeviceInfo::GetType_P()
{
    // this pointer is to Program Memory
    return pgm_read_byte(&this->type);
}

int DmxDeviceInfo::CountDevicesInArray_P(DmxDeviceInfo* pArray)
{
    int count = 0;
    for (DmxDeviceInfo* pInfo = pArray; !pInfo->IsEmpty_P(); pInfo++)
        count++;   // don't count the end marker;
    return count;
}

const DmxDeviceInfo DmxDeviceInfo::Empty { "", "", DmxDeviceInfo::DeviceType::None, -1 };
const uint8_t DmxDeviceInfo::channelsForType[] {0, 1, 1, 1, 1, 3, 4, 5, 4, 5, 4, 5, 6, 5, 6, 7, 8};
 
const DmxDeviceInfo devices1[] PROGMEM
{
    //           123456789012345678901
    { "31045",  "Rec Desk RGBA CStrip",    DmxDeviceInfo::DeviceType::RGBA,           45 },
    { "31027",  "Lby Crvd Wall Flr Str",   DmxDeviceInfo::DeviceType::CW,             27 },
    { "31051",  "Lby Abv Ceil RGB Up 1",   DmxDeviceInfo::DeviceType::RGB,            51 },
    { "31054",  "Lby Abv Ceil RGB Up 2",   DmxDeviceInfo::DeviceType::RGB,            54 },
    { "31057",  "Lby Abv Ceil RGB Up 3",   DmxDeviceInfo::DeviceType::RGB,            57 },
    { "31060",  "Lby Abv Ceil RGB Up 4",   DmxDeviceInfo::DeviceType::RGB,            60 },
    { "31063",  "Lby Abv Ceil RGB Up 5",   DmxDeviceInfo::DeviceType::RGB,            63 },
    { "51012",  "Int West Downlights",     DmxDeviceInfo::DeviceType::WW,             12 },
    { "51028",  "Int Crvd Wall Flr Str",   DmxDeviceInfo::DeviceType::WW,             28 },
    { "51066",  "Int Abv Ceil RGB Up 1",   DmxDeviceInfo::DeviceType::RGB,            66 },
    { "51069",  "Int Abv Ceil RGB Up 2",   DmxDeviceInfo::DeviceType::RGB,            69 },
    { "51072",  "Int Abv Ceil RGB Up 3",   DmxDeviceInfo::DeviceType::RGB,            72 },
    { "51075",  "Int Abv Ceil RGB Up 4",   DmxDeviceInfo::DeviceType::RGB,            75 },
    { "51078",  "Int Abv Ceil RGB Up 5",   DmxDeviceInfo::DeviceType::RGB,            78 },
    { "10004",  "Ofc Volume",              DmxDeviceInfo::DeviceType::Generic,         4 },
    { "101014", "Ofc Center Downlights",   DmxDeviceInfo::DeviceType::WW,             14 },
    { "101019", "Ofc Niche Shelf RGBA",    DmxDeviceInfo::DeviceType::RGBA,          105 },
    { "102013", "NGM Center Downlight",    DmxDeviceInfo::DeviceType::WW,             13 },
    { "102014", "NGM Hex Notification",    DmxDeviceInfo::DeviceType::RGB,           109 },
    //           123456789012345678901
    { "103015", "Tdwn Exit Cove Downl",    DmxDeviceInfo::DeviceType::WW,             15 },
    { "103081", "Tdwn Ceil WW Flood 1",    DmxDeviceInfo::DeviceType::WW,             81 },
    { "103082", "Tdwn Ceil CW Flood 1",    DmxDeviceInfo::DeviceType::CW,             82 },
    { "103083", "Tdwn Ceil WW Flood 2",    DmxDeviceInfo::DeviceType::WW,             83 },
    { "103084", "Tdwn Ceil CW Flood 2",    DmxDeviceInfo::DeviceType::CW,             84 },
    { "103085", "Tdwn Ceil WW Flood 3",    DmxDeviceInfo::DeviceType::WW,             85 },
    { "103086", "Tdwn Ceil CW Flood 3",    DmxDeviceInfo::DeviceType::CW,             86 },
    { "103087", "Tdwn Ceil WW Flood 4",    DmxDeviceInfo::DeviceType::WW,             87 },
    { "103088", "Tdwn Ceil CW Flood 4",    DmxDeviceInfo::DeviceType::CW,             88 },
    { "103089", "Tdwn WallWash WW Fld",    DmxDeviceInfo::DeviceType::WW,             89 },
    { "103090", "Tdwn WallWash CW Fld",    DmxDeviceInfo::DeviceType::CW,             90 },
    { "103101", "Tdwn Ofc Chauvet Rly1",   DmxDeviceInfo::DeviceType::Generic,       101 },
    { "103102", "Tdwn Ofc Chauvet Rly2",   DmxDeviceInfo::DeviceType::Generic,       102 },
    { "103103", "Tdwn Ofc Chauvet Rly3",   DmxDeviceInfo::DeviceType::Generic,       103 },
    { "103104", "Tdwn Ofc Chauvet Rly4",   DmxDeviceInfo::DeviceType::Generic,       104 },
    { "104091", "CC Wboard WW Flood 1",    DmxDeviceInfo::DeviceType::WW,             91 },
    { "104092", "CC Wboard CW Flood 1",    DmxDeviceInfo::DeviceType::CW,             92 },
    { "104093", "CC Wboard WW Flood 2",    DmxDeviceInfo::DeviceType::WW,             93 },
    { "104094", "CC Wboard CW Flood 2",    DmxDeviceInfo::DeviceType::CW,             94 },
    { "104095", "CC Worktable WW Fld 3",   DmxDeviceInfo::DeviceType::WW,             95 },
    { "104096", "CC Worktable CW Fld 3",   DmxDeviceInfo::DeviceType::CW,             96 },
    { "104097", "CC Worktable WW Fld 4",   DmxDeviceInfo::DeviceType::WW,             97 },
    { "104098", "CC Worktable CW Fld 4",   DmxDeviceInfo::DeviceType::CW,             98 },
    { "104099", "CC WallWash WW Flood",    DmxDeviceInfo::DeviceType::WW,             99 },
    { "104100", "CC WallWash CW Flood",    DmxDeviceInfo::DeviceType::CW,            100 },
    { "", "", DmxDeviceInfo::DeviceType::None, -1 }
};
const DmxDeviceInfo devices2[] PROGMEM
{
    //           123456789012345678901
    { "301018", "Mkr Center Pendants",     DmxDeviceInfo::DeviceType::WW,             18 },
    { "301019", "Mkr Perim Wall Wash",     DmxDeviceInfo::DeviceType::WW,             19 },
    
    { "401011", "Plaza Cagelights",        DmxDeviceInfo::DeviceType::WW,             20 },
    { "401021", "Plaza Ceil RGBAC 2",      DmxDeviceInfo::DeviceType::RGBAC,          21 },
    { "401026", "Plaza Ceil RGBAC 1",      DmxDeviceInfo::DeviceType::RGBAC,          26 },
    { "401031", "Plaza Ceil RGBAC 3",      DmxDeviceInfo::DeviceType::RGBAC,          31 },
    { "401036", "Plaza Ceil RGBAC 6",      DmxDeviceInfo::DeviceType::RGBAC,          36 },
    { "401041", "Plaza Ceil RGBAC 4",      DmxDeviceInfo::DeviceType::RGBAC,          41 },
    { "401046", "Plaza Ceil RGBAC 5",      DmxDeviceInfo::DeviceType::RGBAC,          46 },
    { "401051", "Plaza Ceil RGBAC 12",     DmxDeviceInfo::DeviceType::RGBAC,          51 },
    { "401056", "Plaza Ceil RGBAC 11",     DmxDeviceInfo::DeviceType::RGBAC,          56 },
    { "401061", "Plaza Ceil RGBAC 10",     DmxDeviceInfo::DeviceType::RGBAC,          61 },
    { "401066", "Plaza Ceil RGBAC 9",      DmxDeviceInfo::DeviceType::RGBAC,          66 },
    { "401071", "Plaza Ceil RGBAC 13",     DmxDeviceInfo::DeviceType::RGBAC,          71 },
    { "401076", "Plaza Ceil RGBAC 14",     DmxDeviceInfo::DeviceType::RGBAC,          76 },
    { "401081", "Plaza Ceil RGBAC 8",      DmxDeviceInfo::DeviceType::RGBAC,          81 },
    { "401086", "Plaza Ceil RGBAC 7",      DmxDeviceInfo::DeviceType::RGBAC,          86 },
    { "501101", "Cafe N Bay RGBWC Fld",    DmxDeviceInfo::DeviceType::RGBWC,         101 },
    { "501106", "Cafe S Bay RGBWC Fld",    DmxDeviceInfo::DeviceType::RGBWC,         106 },
    { "501111", "Cafe N Bay WW Accent",    DmxDeviceInfo::DeviceType::Generic,       111 },
    { "501112", "Cafe N Bay CW Accent",    DmxDeviceInfo::DeviceType::Generic,       112 },
    { "501113", "Cafe S Bay WW Accent",    DmxDeviceInfo::DeviceType::Generic,       113 },
    { "501114", "Cafe S Bay CW Accent",    DmxDeviceInfo::DeviceType::Generic,       114 },
    { "", "", DmxDeviceInfo::DeviceType::None, -1 }
};
    
const DmxDeviceInfo devices3[] PROGMEM
{
    //           123456789012345678901
    { "2013013","FR South Receptacle",     DmxDeviceInfo::DeviceType::Generic,        13 },
    { "22004",  "Porch Volume",            DmxDeviceInfo::DeviceType::Generic,         4 },
    { "22011",  "Wall Sconce & OverEnt",   DmxDeviceInfo::DeviceType::CW,             11 },
    { "22012",  "Porch Downlights",        DmxDeviceInfo::DeviceType::WW,             12 },
    { "22014",  "OLD Display Cabinet",     DmxDeviceInfo::DeviceType::Generic,        14 },
    { "22015",  "Porch Artificial Sun",    DmxDeviceInfo::DeviceType::WW,             17 },
    { "22016",  "Porch Chauvet Rly 2",     DmxDeviceInfo::DeviceType::Generic,        18 },
    { "22017",  "Porch Chauvet Rly 3",     DmxDeviceInfo::DeviceType::Generic,        19 },
    { "22018",  "Porch Window Light",      DmxDeviceInfo::DeviceType::WW,             20 },
    { "", "", DmxDeviceInfo::DeviceType::None, -1 }
};
    
const DmxDeviceInfo devices4[] PROGMEM
{
    //           123456789012345678901
    { "1303",   "Family Room Volume",      DmxDeviceInfo::DeviceType::Generic,         4 },
    { "201011", "Hm West Downlights",      DmxDeviceInfo::DeviceType::WW,             11 },
    { "201012", "Hm East Downlights",      DmxDeviceInfo::DeviceType::WW,             12 },
    { "201013", "Hm Hallway",              DmxDeviceInfo::DeviceType::WW,             13 },
    { "201015", "Chalkboard WallWash",     DmxDeviceInfo::DeviceType::WW,             15 },
    { "201021", "FRoom Pend W Inner",      DmxDeviceInfo::DeviceType::CW,             45 },
    { "201022", "FRoom Pend W Outer",      DmxDeviceInfo::DeviceType::CW,             46 },
    { "201023", "FRoom Pend N Inner",      DmxDeviceInfo::DeviceType::CW,             47 },
    { "201024", "FRoom Pend N Outer",      DmxDeviceInfo::DeviceType::CW,             49 },
    { "201025", "FRoom Pend E Inner",      DmxDeviceInfo::DeviceType::CW,             50 },
    { "201026", "FRoom Pend E Outer",      DmxDeviceInfo::DeviceType::CW,             51 },
    { "201014", "Kitchen Downlights",      DmxDeviceInfo::DeviceType::WW,             14 },
    { "201016", "Dining Table Pends",      DmxDeviceInfo::DeviceType::CW,             16 },
    { "", "", DmxDeviceInfo::DeviceType::None, -1 }
};
    
const DmxDeviceInfo devices5[] PROGMEM
{
    //           123456789012345678901
    { "23004",  "Kitchen Volume",          DmxDeviceInfo::DeviceType::Generic,         4 },
    { "23011",  "Kitchen Wall Graze",      DmxDeviceInfo::DeviceType::WW,              2 },
    { "23012",  "Kitchen Ceiling Graze",   DmxDeviceInfo::DeviceType::Generic,         3 },
    { "203017", "Kitch N Und Cab RGBA",    DmxDeviceInfo::DeviceType::RGBA,            5 },
    { "203022", "Kitch E Und Cab RGBA",    DmxDeviceInfo::DeviceType::RGBA,            9 },
    { "203031", "Kitch N Ovr Cab RGBA",    DmxDeviceInfo::DeviceType::RGBA,           17 },
    { "203027", "Kitch Cab Scr Backlt",    DmxDeviceInfo::DeviceType::WW,             13 },
    { "203028", "Kitch Cab Scr Frost",     DmxDeviceInfo::DeviceType::CW,             14 },
    { "203029", "Kitch Plant Lights 1",    DmxDeviceInfo::DeviceType::CW,             15 },
    { "203030", "Kitch Plant Lights 2",    DmxDeviceInfo::DeviceType::CW,             16 },
    { "", "", DmxDeviceInfo::DeviceType::None, -1 }
};
    
const DmxDeviceInfo devices6[] PROGMEM
{
    //           123456789012345678901
    { "61004",  "Conv Room Volume",        DmxDeviceInfo::DeviceType::Generic,         4 },
    { "61045",  "Conv Ceil RGBA SL",       DmxDeviceInfo::DeviceType::RGBA,           45 },
    { "61049",  "Conv RGBA Floor SL",      DmxDeviceInfo::DeviceType::RGBA,           49 },
    { "61021",  "Conv Ceiling Globes",     DmxDeviceInfo::DeviceType::WW,             21 },
    { "", "", DmxDeviceInfo::DeviceType::None, -1 }
};

const DmxUniverse DmxUniverse::universes[] PROGMEM
{
    { 1, "Lobby Office Touchdown", devices1},
    { 2, "Plaza Cafe Makerspace", devices2},
    { 3, "Porch + Misc", devices3},
    { 4, "Family Room, Dining Room", devices4},
    { 5, "Kitchen", devices5},
    { 6, "Conversation Room", devices6}
};