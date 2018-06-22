#include "buttons.h"

#include <cstdint>

extern "C" {

// --Global Variables--
#ifdef TTYD_US
  uint32_t r13 = 0x8041CF20;
  char *NextMap = reinterpret_cast<char *>(r13 + 0x16A8);
#elif defined TTYD_JP
  uint32_t r13 = 0x80417260;
  char *NextMap = reinterpret_cast<char *>(r13 + 0x1148);
#elif defined TTYD_EU
  uint32_t r13 = 0x80429760;
  char *NextMap = reinterpret_cast<char *>(r13 + 0x1788);
#endif

uint32_t GSWAddressesStart = r13 - 0x6F50;

bool GotReturnPostage = false;
bool IncrementCountFlag = false;
uint16_t Counter = 0;

// Warp Button Combo
uint16_t WarpCombo = PAD_L | PAD_B;

}