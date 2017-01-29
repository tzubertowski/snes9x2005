#include "../copyright"

#ifndef _PPU_H_
#define _PPU_H_

#define FIRST_VISIBLE_LINE 1

extern uint8_t GetBank;
extern uint16_t SignExtend [2];

#define TILE_2BIT 0
#define TILE_4BIT 1
#define TILE_8BIT 2

#define MAX_2BIT_TILES 4096
#define MAX_4BIT_TILES 2048
#define MAX_8BIT_TILES 1024

#define PPU_H_BEAM_IRQ_SOURCE (1 << 0)
#define PPU_V_BEAM_IRQ_SOURCE (1 << 1)
#define GSU_IRQ_SOURCE     (1 << 2)
#define SA1_IRQ_SOURCE     (1 << 7)
#define SA1_DMA_IRQ_SOURCE (1 << 5)

struct ClipData
{
   uint32_t  Count [6];
   uint32_t  Left [6][6];
   uint32_t  Right [6][6];
};

typedef struct
{
   bool     ColorsChanged;
   uint8_t  HDMA;
   bool     HDMAStarted;
   uint8_t  MaxBrightness;
   bool     LatchedBlanking;
   bool     OBJChanged;
   bool     RenderThisFrame;
   bool     DirectColourMapsNeedRebuild;
   uint32_t FrameCount;
   uint32_t RenderedFramesCount;
   uint32_t DisplayedRenderedFrameCount;
   uint32_t SkippedFrames;
   uint32_t FrameSkip;
   uint8_t* TileCache [3];
   uint8_t* TileCached [3];
#ifdef CORRECT_VRAM_READS
   uint16_t VRAMReadBuffer;
#else
   bool  FirstVRAMRead;
#endif
   bool            DoubleHeightPixels;
   bool            Interlace;
   bool            InterlaceSprites;
   bool            DoubleWidthPixels;
   bool            HalfWidthPixels;
   int32_t         RenderedScreenHeight;
   int32_t         RenderedScreenWidth;
   uint32_t        Red [256];
   uint32_t        Green [256];
   uint32_t        Blue [256];
   uint8_t*        XB;
   uint16_t        ScreenColors [256];
   int32_t         PreviousLine;
   int32_t         CurrentLine;
   int32_t         Controller;
   uint32_t        Joypads[5];
   uint32_t        SuperScope;
   uint32_t        Mouse[2];
   int32_t         PrevMouseX[2];
   int32_t         PrevMouseY[2];
   struct ClipData Clip [2];
} InternalPPU;

struct SOBJ
{
   int16_t  HPos;
   uint16_t VPos;
   uint16_t Name;
   uint8_t  VFlip;
   uint8_t  HFlip;
   uint8_t  Priority;
   uint8_t  Palette;
   uint8_t  Size;
};

typedef struct
{
   uint8_t BGMode;
   uint8_t BG3Priority;
   uint8_t Brightness;

   struct
   {
      bool     High;
      uint8_t  Increment;
      uint16_t Address;
      uint16_t Mask1;
      uint16_t FullGraphicCount;
      uint16_t Shift;
   } VMA;

   struct
   {
      uint16_t SCBase;
      uint16_t VOffset;
      uint16_t HOffset;
      uint8_t  BGSize;
      uint16_t NameBase;
      uint16_t SCSize;
   } BG [4];

   bool        CGFLIP;
   uint16_t    CGDATA [256];
   uint8_t     FirstSprite;
   uint8_t     LastSprite;
   struct SOBJ OBJ [128];
   uint8_t     OAMPriorityRotation;
   uint16_t    OAMAddr;
   uint8_t     RangeTimeOver;

   uint8_t  OAMFlip;
   uint16_t OAMTileAddress;
   uint16_t IRQVBeamPos;
   uint16_t IRQHBeamPos;
   uint16_t VBeamPosLatched;
   uint16_t HBeamPosLatched;

   uint8_t HBeamFlip;
   uint8_t VBeamFlip;
   uint8_t HVBeamCounterLatched;

   int16_t MatrixA;
   int16_t MatrixB;
   int16_t MatrixC;
   int16_t MatrixD;
   int16_t CentreX;
   int16_t CentreY;
   uint8_t Joypad1ButtonReadPos;
   uint8_t Joypad2ButtonReadPos;

   uint8_t  CGADD;
   uint8_t  FixedColourRed;
   uint8_t  FixedColourGreen;
   uint8_t  FixedColourBlue;
   uint16_t SavedOAMAddr;
   uint16_t ScreenHeight;
   uint32_t WRAM;
   uint8_t  BG_Forced;
   bool     ForcedBlanking;
   bool     OBJThroughMain;
   bool     OBJThroughSub;
   uint8_t  OBJSizeSelect;
   uint16_t OBJNameBase;
   bool     OBJAddition;
   uint8_t  OAMReadFlip;
   uint8_t  OAMData [512 + 32];
   bool     VTimerEnabled;
   bool     HTimerEnabled;
   int16_t  HTimerPosition;
   uint8_t  Mosaic;
   bool     BGMosaic [4];
   bool     Mode7HFlip;
   bool     Mode7VFlip;
   uint8_t  Mode7Repeat;
   uint8_t  Window1Left;
   uint8_t  Window1Right;
   uint8_t  Window2Left;
   uint8_t  Window2Right;
   uint8_t  ClipCounts [6];
   uint8_t  ClipWindowOverlapLogic [6];
   uint8_t  ClipWindow1Enable [6];
   uint8_t  ClipWindow2Enable [6];
   bool     ClipWindow1Inside [6];
   bool     ClipWindow2Inside [6];
   bool     RecomputeClipWindows;
   uint8_t  CGFLIPRead;
   uint16_t OBJNameSelect;
   bool     Need16x8Mulitply;
   uint8_t  Joypad3ButtonReadPos;
   uint8_t  MouseSpeed[2];

   // XXX Do these need to be added to snapshot.cpp?
   uint16_t OAMWriteRegister;
   uint8_t  BGnxOFSbyte;
   uint8_t  OpenBus1;
   uint8_t  OpenBus2;
} SPPU;

#define CLIP_OR 0
#define CLIP_AND 1
#define CLIP_XOR 2
#define CLIP_XNOR 3

typedef struct
{
   bool    TransferDirection;
   bool    AAddressFixed;
   bool    AAddressDecrement;
   uint8_t TransferMode;

   uint8_t  ABank;
   uint16_t AAddress;
   uint16_t Address;
   uint8_t  BAddress;

   // General DMA only:
   uint16_t TransferBytes;

   // H-DMA only:
   bool     HDMAIndirectAddressing;
   uint16_t IndirectAddress;
   uint8_t  IndirectBank;
   uint8_t  Repeat;
   uint8_t  LineCount;
   uint8_t  FirstLine;
} SDMA;

void S9xUpdateScreen();
void S9xResetPPU();
void S9xSoftResetPPU();
void S9xFixColourBrightness();
void S9xUpdateJoypads();
void S9xProcessMouse(int32_t which1);
void S9xSuperFXExec();

void S9xSetPPU(uint8_t Byte, uint16_t Address);
uint8_t S9xGetPPU(uint16_t Address);
void S9xSetCPU(uint8_t Byte, uint16_t Address);
uint8_t S9xGetCPU(uint16_t Address);

void S9xInitC4();
void S9xSetC4(uint8_t Byte, uint16_t Address);
uint8_t S9xGetC4(uint16_t Address);
void S9xSetC4RAM(uint8_t Byte, uint16_t Address);
uint8_t S9xGetC4RAM(uint16_t Address);

extern SPPU PPU;
extern SDMA DMA [8];
extern InternalPPU IPPU;

#include "memmap.h"

typedef struct
{
   uint8_t _5C77;
   uint8_t _5C78;
   uint8_t _5A22;
} SnesModel;

extern SnesModel* Model;
extern SnesModel M1SNES;
extern SnesModel M2SNES;

#define MAX_5C77_VERSION 0x01
#define MAX_5C78_VERSION 0x03
#define MAX_5A22_VERSION 0x02

extern uint8_t REGISTER_4212();
extern void FLUSH_REDRAW();
extern void REGISTER_2104(uint8_t byte);
extern void REGISTER_2118(uint8_t Byte);
extern void REGISTER_2118_tile(uint8_t Byte);
extern void REGISTER_2118_linear(uint8_t Byte);
extern void REGISTER_2119(uint8_t Byte);
extern void REGISTER_2119_tile(uint8_t Byte);
extern void REGISTER_2119_linear(uint8_t Byte);
extern void REGISTER_2122(uint8_t Byte);
extern void REGISTER_2180(uint8_t Byte);

//Platform specific input functions used by PPU.CPP
void JustifierButtons(uint32_t*);
bool JustifierOffscreen();

#endif
