#include "mod.h"

#include <ttyd/system.h>
#include <ttyd/fontmgr.h>
#include <ttyd/dispdrv.h>

#include "patch.h"

#include <cstdio>

namespace mod {

Mod *gMod = nullptr;

void main()
{
  Mod *mod = new Mod();
  mod->init();
}

Mod::Mod()
{
  
}

void Mod::init()
{
  gMod = this;
  
  mPFN_makeKey_trampoline = patch::hookFunction(ttyd::system::makeKey, []()
  {
    gMod->updateEarly();
  });
  
  // Initialize typesetting early
  ttyd::fontmgr::fontmgrTexSetup();
  patch::hookFunction(ttyd::fontmgr::fontmgrTexSetup, [](){});
}

void Mod::updateEarly()
{
  Mod::initClearStuff();
  Mod::setPitFloorsBeaten();
  Mod::warpToBonetail();
  Mod::overwritePipeBeroMap();
  Mod::overwriteBonetailChestItem();
  Mod::clearBonetailGSWFs();
  Mod::incrementCount();
  
  // Check for font load
  ttyd::dispdrv::dispEntry(ttyd::dispdrv::DisplayLayer::k2d, 0, [](ttyd::dispdrv::DisplayLayer layerId, void *user)
  {
    // --Tricks (With Display)--
    reinterpret_cast<Mod *>(user)->displayRoundNumber();
  }, this);
  
  // Call original function
  mPFN_makeKey_trampoline();
}

}