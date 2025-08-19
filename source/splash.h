/* SF2000 Splash Screen System */
#ifndef SPLASH_H
#define SPLASH_H

#include <stdint.h>
#include <stdbool.h>

#ifdef SF2000

/* Splash screen configuration */
#define SPLASH_DURATION_FRAMES 180  /* 3 seconds at 60fps */
#define SPLASH_BG_COLOR 0xF56D      /* #df6da9 in RGB565 format */
#define SPLASH_TEXT_COLOR 0xFFFF    /* White in RGB565 format */
#define SPLASH_BLACK_COLOR 0x0000   /* Black in RGB565 format */

/* Splash screen state */
typedef struct {
    bool active;
    int frame_counter;
    char build_date[20];  /* "BUILD DDMMYYHH" format - increased size */
} splash_screen_t;

extern splash_screen_t splash_screen;

/* Function declarations */
void splash_screen_init(void);
void splash_screen_render(uint16_t* screen, int width, int height, int pitch);
bool splash_screen_update(void);
void splash_screen_generate_build_date(void);

/* Font data for splash text (simple 8x8 bitmap font) */
extern const uint8_t splash_font_8x8[256][8];

#endif /* SF2000 */

#endif /* SPLASH_H */