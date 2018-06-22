#include "mod.h"
#include "maps.h"

#include <ttyd/string.h>
#include <ttyd/seqdrv.h>
#include <ttyd/system.h>
#include <ttyd/mariost.h>
#include <ttyd/camdrv.h>
#include <ttyd/pmario_sound.h>
#include <ttyd/mario_cam.h>
#include <ttyd/fontmgr.h>

#include <cstdio>

extern uint32_t GSWAddressesStart;
extern uint16_t Counter;
extern uint16_t WarpCombo;
extern char *NextMap;
extern bool GotReturnPostage;
extern bool IncrementCountFlag;

namespace mod {
  
void Mod::initClearStuff()
{
  if ((ttyd::string::strcmp(NextMap, reinterpret_cast<char *>(jon_06)) != 0) || (ttyd::seqdrv::seqGetNextSeq() == static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kLoad)))
  {
    // Currently not in the Pit, can also be on the File Select screen
    // Checking for kLoad is necessary, as the strcmp will still succeed following a Game Over
    GotReturnPostage = false;
    Counter = 0;
    
    // Turn off GSWF(5084) and GSWF(5085)
    uint32_t GSWFAddressesStart = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
    uint32_t PitFlagsAddress = GSWFAddressesStart + 0x3F0;
    *reinterpret_cast<uint32_t *>(PitFlagsAddress) &= ~((1 << 29) | (1 << 28)); // Turn off the 29 and 28 bits
  }
}
  
void Mod::setPitFloorsBeaten()
{
  // Set Pit Floors Beaten to 99
  uint32_t PitFloorAddress = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
  PitFloorAddress += 0xAA1; // GSW(1321)
  *reinterpret_cast<uint8_t *>(PitFloorAddress) = 99;
}

void Mod::warpToBonetail()
{
  int32_t jon_06_comparison = ttyd::string::strcmp(NextMap, reinterpret_cast<char *>(jon_06));
  int32_t end_00_comparison = ttyd::string::strcmp(NextMap, reinterpret_cast<char *>(end_00));
  if ((jon_06_comparison != 0) && (end_00_comparison != 0))
  {
    // Not currently in the Pit nor the Credits
    if ((ttyd::system::keyGetButton(0) & WarpCombo) == WarpCombo)
    {
      uint32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
      uint32_t Game = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame);
      
      if (NextSeq == Game)
      { 
        // Set Bero and Map, and then Warp
        #ifdef TTYD_US
          uint32_t dokan_2 = 0x802E7EA8;
        #elif defined TTYD_JP
          uint32_t dokan_2 = 0x802E9834;
        #elif defined TTYD_EU
          uint32_t dokan_2 = 0x802F3B08;
        #endif
        
        ttyd::seqdrv::seqSetSeq(ttyd::seqdrv::SeqIndex::kMapChange, reinterpret_cast<char *>(jon_06), reinterpret_cast<char *>(dokan_2));
        
        uint32_t SystemLevel = ttyd::mariost::marioStGetSystemLevel();
        if (SystemLevel != 0)
        {
          if (SystemLevel == 15)
          {
            // Currently in pause menu, so re-enable the camera
            ttyd::camdrv::camDispOn(4);
          }
          
          // Enable sound effects, set the default camera id for Mario, and give back control to the player
          ttyd::pmario_sound::psndClearFlag(0x80);
          ttyd::mario_cam::marioSetCamId(4);
          ttyd::mariost::marioStSystemLevel(0);
        }
      }
    } 
  }
}

void Mod::overwritePipeBeroMap()
{
  uint32_t REL_Location = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
  REL_Location = *reinterpret_cast<uint32_t *>(REL_Location + 0x15C);
  if (REL_Location != 0)
  {
    // REL is currently loaded
    #ifdef TTYD_US
      uint8_t jon = 0xE;
    #elif defined TTYD_JP
      uint8_t jon = 0xF;
    #elif defined TTYD_EU
      uint8_t jon = 0xF;
    #endif
    
    uint32_t REL_id = *reinterpret_cast<uint32_t *>(REL_Location);
    if (REL_id == jon)
    {
      // Pit REL is currently loaded
      #ifdef TTYD_US
        uint32_t PipeNextMap = REL_Location + 0x754C;
        uint32_t PipeNextBero = REL_Location + 0x7554;
      #elif defined TTYD_JP
        uint32_t PipeNextMap = REL_Location + 0x755C;
        uint32_t PipeNextBero = REL_Location + 0x7564;
      #elif defined TTYD_EU
        uint32_t PipeNextMap = REL_Location + 0x7584;
        uint32_t PipeNextBero = REL_Location + 0x758C;
      #endif
      
      if (Counter < 100)
      {
        #ifdef TTYD_US
          uint32_t dokan_2 = 0x802E7EA8;
        #elif defined TTYD_JP
          uint32_t dokan_2 = 0x802E9834;
        #elif defined TTYD_EU
          uint32_t dokan_2 = 0x802F3B08;
        #endif
        
        ttyd::string::strcpy(reinterpret_cast<char *>(PipeNextMap), reinterpret_cast<char *>(jon_06));
        ttyd::string::strcpy(reinterpret_cast<char *>(PipeNextBero), reinterpret_cast<char *>(dokan_2));
      }
      else
      {
        ttyd::string::strcpy(reinterpret_cast<char *>(PipeNextMap), reinterpret_cast<char *>(end_00));
      }
    }
  }
}

void Mod::overwriteBonetailChestItem()
{
  if (GotReturnPostage)
  {
    uint32_t REL_Location = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
    REL_Location = *reinterpret_cast<uint32_t *>(REL_Location + 0x15C);
    if (REL_Location != 0)
    {
      // REL is currently loaded
      #ifdef TTYD_US
        uint8_t jon = 0xE;
      #elif defined TTYD_JP
        uint8_t jon = 0xF;
      #elif defined TTYD_EU
        uint8_t jon = 0xF;
      #endif
      
      uint32_t REL_id = *reinterpret_cast<uint32_t *>(REL_Location);
      if (REL_id == jon)
      {
        uint32_t BonetailChestItemAddress;
        
        #ifdef TTYD_US
          BonetailChestItemAddress = REL_Location + 0x11344;
        #elif defined TTYD_JP
          BonetailChestItemAddress = REL_Location + 0x11318;
        #elif defined TTYD_EU
          BonetailChestItemAddress = REL_Location + 0x1137C;
        #endif
        
        *reinterpret_cast<uint32_t *>(BonetailChestItemAddress) = 121; // 121 = Coin
      }
    }
  }
}

void Mod::clearBonetailGSWFs()
{
  uint32_t GSWFAddressesStart = *reinterpret_cast<uint32_t *>(GSWAddressesStart);
  uint32_t PitFlagsAddress = GSWFAddressesStart + 0x3F0;
  
  // Check if GSWF(5084) or GSWF(5085) are on
  uint32_t PitFlagsBits = *reinterpret_cast<uint32_t *>(PitFlagsAddress) & 0x30000000;
  if (PitFlagsBits)
  {
    if ((PitFlagsBits & 0x20000000) == 0x20000000)
    {
      // GSWF(5085) is on
      IncrementCountFlag = true;
    }
    
    if ((PitFlagsBits & 0x10000000) == 0x10000000)
    {
      // GSWF(5084) is on
      GotReturnPostage = true;
    }
    
    // Turn off GSWF(5084) and GSWF(5085)
    *reinterpret_cast<uint32_t *>(PitFlagsAddress) &= ~((1 << 29) | (1 << 28)); // Turn off the 29 and 28 bits
  }
}

void Mod::incrementCount()
{
  if (IncrementCountFlag)
  {
    ++Counter;
    IncrementCountFlag = false;
  }
}

void Mod::displayRoundNumber()
{
  if (ttyd::string::strcmp(NextMap, reinterpret_cast<char *>(jon_06)) == 0)
  {
    // Currently in the Pit
    uint32_t NextSeq = ttyd::seqdrv::seqGetNextSeq();
    uint32_t Game = static_cast<uint32_t>(ttyd::seqdrv::SeqIndex::kGame);
    
    if (NextSeq == Game)
    {
      sprintf(mDisplayBuffer,
        "Bonetail Count: %d",
        Counter);
      
      ttyd::fontmgr::FontDrawStart();
      uint32_t color = 0xFFFFFFFF;
      ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
      ttyd::fontmgr::FontDrawEdge();
      ttyd::fontmgr::FontDrawMessage(-252, -153, mDisplayBuffer);
    }
  }
}

}