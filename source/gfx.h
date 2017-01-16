#include "../copyright"

#ifndef _GFX_H_
#define _GFX_H_

#include "port.h"
#include "snes9x.h"

void S9xStartScreenRefresh();
void S9xDrawScanLine(uint8_t Line);
void S9xEndScreenRefresh();
void S9xSetupOBJ();
void S9xUpdateScreen();
void RenderLine(uint8_t line);
void S9xBuildDirectColourMaps();

// External port interface which must be implemented or initialised for each
// port.
extern struct SGFX GFX;

bool S9xInitGFX();
void S9xDeinitGFX();
bool S9xInitUpdate(void);

struct SGFX
{
   // Initialize these variables
   uint8_t* Screen_buffer;
   uint8_t* SubScreen_buffer;
   uint8_t* ZBuffer_buffer;
   uint8_t* SubZBuffer_buffer;

   uint8_t* Screen;
   uint8_t* SubScreen;
   uint8_t* ZBuffer;
   uint8_t* SubZBuffer;
   uint32_t Pitch;

   // Setup in call to S9xInitGFX()
   int              Delta;
   uint16_t*        X2;
   uint16_t*        ZERO_OR_X2;
   uint16_t*        ZERO;
   uint32_t         RealPitch;           // True pitch of Screen buffer.
   uint32_t         Pitch2;              // Same as RealPitch except while using speed up hack for Glide.
   uint32_t         ZPitch;              // Pitch of ZBuffer
   uint32_t         PPL;                 // Number of pixels on each of Screen buffer
   uint32_t         PPLx2;
   uint32_t         PixSize;
   uint8_t          S_safety_margin[8];
   uint8_t*         S;
   uint8_t          DB_safety_margin[8];
   uint8_t*         DB;
   ptrdiff_t        DepthDelta;
   uint8_t          Z1;                  // Depth for comparison
   uint8_t          Z2;                  // Depth to save
   uint8_t          ZSprite;             // Used to ensure only 1st sprite is drawn per pixel
   uint32_t         FixedColour;
   const char*      InfoString;
   uint32_t         InfoStringTimeout;
   uint32_t         StartY;
   uint32_t         EndY;
   struct ClipData* pCurrentClip;
   uint32_t         Mode7Mask;
   uint32_t         Mode7PriorityMask;
   uint8_t          OBJWidths[128];
   uint8_t          OBJVisibleTiles[128];

   struct
   {
      uint8_t RTOFlags;
      int16_t Tiles;

      struct
      {
         int8_t Sprite;
         uint8_t Line;
      } OBJ[32];
   } OBJLines [SNES_HEIGHT_EXTENDED];

   uint8_t r212c;
   uint8_t r212d;
   uint8_t r2130;
   uint8_t r2131;
   bool    Pseudo;
};

struct SLineData
{
   struct
   {
      uint16_t VOffset;
      uint16_t HOffset;
   } BG [4];
};

#define H_FLIP 0x4000
#define V_FLIP 0x8000
#define BLANK_TILE 2

typedef struct
{
   uint32_t TileSize;
   uint32_t BitShift;
   uint32_t TileShift;
   uint32_t TileAddress;
   uint32_t NameSelect;
   uint32_t SCBase;

   uint32_t StartPalette;
   uint32_t PaletteShift;
   uint32_t PaletteMask;

   uint8_t* Buffer;
   uint8_t* Buffered;
   bool     DirectColourMode;
} SBG;

struct SLineMatrixData
{
   short MatrixA;
   short MatrixB;
   short MatrixC;
   short MatrixD;
   short CentreX;
   short CentreY;
};

extern uint32_t odd_high [4][16];
extern uint32_t odd_low [4][16];
extern uint32_t even_high [4][16];
extern uint32_t even_low [4][16];
extern SBG BG;
extern uint16_t DirectColourMaps [8][256];

extern uint8_t mul_brightness [16][32];

// Could use BSWAP instruction on Intel port...
#define SWAP_DWORD(dw) dw = ((dw & 0xff) << 24) | ((dw & 0xff00) << 8) | \
                  ((dw & 0xff0000) >> 8) | ((dw & 0xff000000) >> 24)

#ifdef FAST_LSB_WORD_ACCESS
#define READ_2BYTES(s) (*(uint16_t *) (s))
#define WRITE_2BYTES(s, d) *(uint16_t *) (s) = (d)
#else
#define READ_2BYTES(s) (*(uint8_t *) (s) | (*((uint8_t *) (s) + 1) << 8))
#define WRITE_2BYTES(s, d) *(uint8_t *) (s) = (d), \
            *((uint8_t *) (s) + 1) = (d) >> 8
#endif // i386

#define SUB_SCREEN_DEPTH 0
#define MAIN_SCREEN_DEPTH 32

#if defined(OLD_COLOUR_BLENDING)
#define COLOR_ADD(C1, C2) \
GFX.X2 [((((C1) & RGB_REMOVE_LOW_BITS_MASK) + \
     ((C2) & RGB_REMOVE_LOW_BITS_MASK)) >> 1) + \
   ((C1) & (C2) & RGB_LOW_BITS_MASK)]
#else
static inline uint16_t COLOR_ADD(uint16_t, uint16_t);

static inline uint16_t COLOR_ADD(uint16_t C1, uint16_t C2)
{
   if (C1 == 0)
      return C2;
   else if (C2 == 0)
      return C1;
   else
      return GFX.X2 [(((C1 & RGB_REMOVE_LOW_BITS_MASK) + (C2 &
                       RGB_REMOVE_LOW_BITS_MASK)) >> 1) + (C1 & C2 & RGB_LOW_BITS_MASK)] | ((
                                C1 ^ C2) & RGB_LOW_BITS_MASK);
}
#endif

#define COLOR_ADD1_2(C1, C2) \
(((((C1) & RGB_REMOVE_LOW_BITS_MASK) + \
          ((C2) & RGB_REMOVE_LOW_BITS_MASK)) >> 1) + \
         (((C1) & (C2) & RGB_LOW_BITS_MASK) | ALPHA_BITS_MASK))

#if defined(OLD_COLOUR_BLENDING)
#define COLOR_SUB(C1, C2) \
GFX.ZERO_OR_X2 [(((C1) | RGB_HI_BITS_MASKx2) - \
       ((C2) & RGB_REMOVE_LOW_BITS_MASK)) >> 1]
#elif !defined(NEW_COLOUR_BLENDING)
#define COLOR_SUB(C1, C2) \
(GFX.ZERO_OR_X2 [(((C1) | RGB_HI_BITS_MASKx2) - \
                  ((C2) & RGB_REMOVE_LOW_BITS_MASK)) >> 1] + \
((C1) & RGB_LOW_BITS_MASK) - ((C2) & RGB_LOW_BITS_MASK))
#else
inline uint16_t COLOR_SUB(uint16_t, uint16_t);

inline uint16_t COLOR_SUB(uint16_t C1, uint16_t C2)
{
   uint16_t   mC1, mC2, v = 0;

   mC1 = C1 & FIRST_COLOR_MASK;
   mC2 = C2 & FIRST_COLOR_MASK;
   if (mC1 > mC2) v += (mC1 - mC2);

   mC1 = C1 & SECOND_COLOR_MASK;
   mC2 = C2 & SECOND_COLOR_MASK;
   if (mC1 > mC2) v += (mC1 - mC2);

   mC1 = C1 & THIRD_COLOR_MASK;
   mC2 = C2 & THIRD_COLOR_MASK;
   if (mC1 > mC2) v += (mC1 - mC2);

   return v;
}
#endif

#define COLOR_SUB1_2(C1, C2) \
GFX.ZERO [(((C1) | RGB_HI_BITS_MASKx2) - \
      ((C2) & RGB_REMOVE_LOW_BITS_MASK)) >> 1]

typedef void (*NormalTileRenderer)(uint32_t Tile, int32_t Offset,
                                   uint32_t StartLine, uint32_t LineCount);
typedef void (*ClippedTileRenderer)(uint32_t Tile, int32_t Offset,
                                    uint32_t StartPixel, uint32_t Width,
                                    uint32_t StartLine, uint32_t LineCount);
typedef void (*LargePixelRenderer)(uint32_t Tile, int32_t Offset,
                                   uint32_t StartPixel, uint32_t Pixels,
                                   uint32_t StartLine, uint32_t LineCount);

#endif
