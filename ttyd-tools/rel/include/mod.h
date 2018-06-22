#pragma once

#include <cstdint>

namespace mod {

class Mod
{
public:
  Mod();
  void init();
  
private:
  void updateEarly();
  
private:
  void initClearStuff();
  void setPitFloorsBeaten();
  void warpToBonetail();
  void overwritePipeBeroMap();
  void overwriteBonetailChestItem();
  void clearBonetailGSWFs();
  void incrementCount();
  void displayRoundNumber();
  
private:
  void (*mPFN_makeKey_trampoline)() = nullptr;
  char mDisplayBuffer[256];
};

}