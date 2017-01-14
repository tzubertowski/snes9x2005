#include "../copyright"

#ifndef _memmap_h_
#define _memmap_h_

#include "snes9x.h"

#ifdef FAST_LSB_WORD_ACCESS
#define READ_WORD(s) (*(uint16_t *) (s))
#define READ_DWORD(s) (*(uint32_t *) (s))
#define WRITE_WORD(s, d) (*(uint16_t *) (s)) = (d)
#define WRITE_DWORD(s, d) (*(uint32_t *) (s)) = (d)

#define READ_3WORD(s) (0x00ffffff & *(uint32_t *) (s))
#define WRITE_3WORD(s, d) *(uint16_t *) (s) = (uint16_t)(d),\
                          *((uint8_t *) (s) + 2) = (uint8_t) ((d) >> 16)


#else
#define READ_WORD(s) ( *(uint8_t *) (s) |\
            (*((uint8_t *) (s) + 1) << 8))
#define READ_DWORD(s) ( *(uint8_t *) (s) |\
             (*((uint8_t *) (s) + 1) << 8) |\
             (*((uint8_t *) (s) + 2) << 16) |\
             (*((uint8_t *) (s) + 3) << 24))
#define WRITE_WORD(s, d) *(uint8_t *) (s) = (d), \
                         *((uint8_t *) (s) + 1) = (d) >> 8
#define WRITE_DWORD(s, d) *(uint8_t *) (s) = (uint8_t) (d), \
                          *((uint8_t *) (s) + 1) = (uint8_t) ((d) >> 8),\
                          *((uint8_t *) (s) + 2) = (uint8_t) ((d) >> 16),\
                          *((uint8_t *) (s) + 3) = (uint8_t) ((d) >> 24)
#define WRITE_3WORD(s, d) *(uint8_t *) (s) = (uint8_t) (d), \
                          *((uint8_t *) (s) + 1) = (uint8_t) ((d) >> 8),\
                          *((uint8_t *) (s) + 2) = (uint8_t) ((d) >> 16)
#define READ_3WORD(s) ( *(uint8_t *) (s) |\
                       (*((uint8_t *) (s) + 1) << 8) |\
                       (*((uint8_t *) (s) + 2) << 16))
#endif

#define MEMMAP_BLOCK_SIZE (0x1000)
#define MEMMAP_NUM_BLOCKS (0x1000000 / MEMMAP_BLOCK_SIZE)
#define MEMMAP_BLOCKS_PER_BANK (0x10000 / MEMMAP_BLOCK_SIZE)
#define MEMMAP_SHIFT 12
#define MEMMAP_MASK (MEMMAP_BLOCK_SIZE - 1)
#define MEMMAP_MAX_SDD1_LOGGED_ENTRIES (0x10000 / 8)

//Extended ROM Formats
#define NOPE 0
#define YEAH 1
#define BIGFIRST 2
#define SMALLFIRST 3

#ifdef LOAD_FROM_MEMORY_TEST
bool LoadROM(const struct retro_game_info* game);
#else
bool LoadROM(const char*);
#endif
void  InitROM(bool);
bool S9xInitMemory();
void  S9xDeinitMemory();
void  FreeSDD1Data();

void WriteProtectROM();
void FixROMSpeed();
void MapRAM();
void MapExtraRAM();

void BSLoROMMap();
void JumboLoROMMap(bool);
void LoROMMap();
void LoROM24MBSMap();
void SRAM512KLoROMMap();
//    void SRAM1024KLoROMMap ();
void SufamiTurboLoROMMap();
void HiROMMap();
void SuperFXROMMap();
void TalesROMMap(bool);
void AlphaROMMap();
void SA1ROMMap();
void BSHiROMMap();
void SPC7110HiROMMap();
void SPC7110Sram(uint8_t);
void SetaDSPMap();
void ApplyROMFixes();

const char* TVStandard();
const char* Speed();
const char* StaticRAMSize();
const char* MapType();
const char* MapMode();
const char* KartContents();
const char* Size();
const char* Headers();
const char* ROMID();
const char* CompanyID();
void ParseSNESHeader(uint8_t*);
enum
{
   MAP_PPU, MAP_CPU, MAP_DSP, MAP_LOROM_SRAM, MAP_HIROM_SRAM,
   MAP_NONE, MAP_DEBUG, MAP_C4, MAP_BWRAM, MAP_BWRAM_BITMAP,
   MAP_BWRAM_BITMAP2, MAP_SA1RAM, MAP_SPC7110_ROM, MAP_SPC7110_DRAM,
   MAP_RONLY_SRAM, MAP_OBC_RAM, MAP_SETA_DSP, MAP_SETA_RISC, MAP_LAST
};
enum { MAX_ROM_SIZE = 0x800000 };

typedef struct
{
   uint8_t* RAM;
   uint8_t* ROM;
   uint8_t* VRAM;
   uint8_t* SRAM;
   uint8_t* BWRAM;
   uint8_t* FillRAM;
   uint8_t* C4RAM;
   bool HiROM;
   bool LoROM;
   uint32_t SRAMMask;
   uint8_t SRAMSize;
   uint8_t* Map [MEMMAP_NUM_BLOCKS];
   uint8_t* WriteMap [MEMMAP_NUM_BLOCKS];
   uint8_t MemorySpeed [MEMMAP_NUM_BLOCKS];
   uint8_t BlockIsRAM [MEMMAP_NUM_BLOCKS];
   uint8_t BlockIsROM [MEMMAP_NUM_BLOCKS];
   char  ROMName [ROM_NAME_LEN];
   char  ROMId [5];
   char  CompanyId [3];
   uint8_t ROMSpeed;
   uint8_t ROMType;
   uint8_t ROMSize;
   int32_t ROMFramesPerSecond;
   int32_t HeaderCount;
   uint32_t CalculatedSize;
   uint32_t CalculatedChecksum;
   uint32_t ROMChecksum;
   uint32_t ROMComplementChecksum;
   uint8_t*  SDD1Index;
   uint8_t*  SDD1Data;
   uint32_t SDD1Entries;
   uint32_t SDD1LoggedDataCountPrev;
   uint32_t SDD1LoggedDataCount;
   uint8_t  SDD1LoggedData [MEMMAP_MAX_SDD1_LOGGED_ENTRIES];
   char ROMFilename [_MAX_PATH];
   uint8_t ROMRegion;
   uint32_t ROMCRC32;
   uint8_t ExtendedFormat;
   uint8_t* BSRAM;
#if 0
   bool LoadMulti(const char*, const char*, const char*);
#endif
} CMemory;

void ResetSpeedMap();

extern CMemory Memory;
void S9xDeinterleaveMode2();

#ifdef NO_INLINE_SET_GET
uint8_t S9xGetByte(uint32_t Address);
uint16_t S9xGetWord(uint32_t Address);
void S9xSetByte(uint8_t Byte, uint32_t Address);
void S9xSetWord(uint16_t Byte, uint32_t Address);
void S9xSetPCBase(uint32_t Address);
uint8_t* S9xGetMemPointer(uint32_t Address);
uint8_t* GetBasePointer(uint32_t Address);

extern uint8_t OpenBus;

#else
#ifndef INLINE
#define INLINE static inline
#endif
#include "getset.h"
#endif // NO_INLINE_SET_GET

#endif // _memmap_h_

