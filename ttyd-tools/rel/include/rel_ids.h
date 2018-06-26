#pragma once

#include <cstdio>

// Enumerations
enum REL_IDs : uint8_t
{
  #ifdef TTYD_US
    jon = 0xE,
  #elif defined TTYD_JP
    jon = 0xF,
  #elif defined TTYD_EU
    jon = 0xF,
  #endif
};