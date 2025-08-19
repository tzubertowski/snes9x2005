/* SF2000 Splash Screen Implementation */
#include "splash.h"

#ifdef SF2000

#include <string.h>
#include <time.h>
#include "snes9x.h"

/* Global splash screen state */
splash_screen_t splash_screen = {0};

/* Simple 8x8 bitmap font (basic ASCII characters 32-127) */
const uint8_t splash_font_8x8[256][8] = {
    /* Space (32) */
    [32] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    /* A (65) */
    [65] = {0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x00},
    /* B (66) */
    [66] = {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00},
    /* C (67) */
    [67] = {0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00},
    /* D (68) */
    [68] = {0x78, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0x78, 0x00},
    /* E (69) */
    [69] = {0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x7E, 0x00},
    /* F (70) */
    [70] = {0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00},
    /* G (71) */
    [71] = {0x3C, 0x66, 0x60, 0x6E, 0x66, 0x66, 0x3C, 0x00},
    /* H (72) */
    [72] = {0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00},
    /* I (73) */
    [73] = {0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00},
    /* L (76) */
    [76] = {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00},
    /* M (77) */
    [77] = {0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x00},
    /* N (78) */
    [78] = {0x66, 0x76, 0x7E, 0x7E, 0x6E, 0x66, 0x66, 0x00},
    /* O (79) */
    [79] = {0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00},
    /* P (80) */
    [80] = {0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x00},
    /* R (82) */
    [82] = {0x7C, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x66, 0x00},
    /* S (83) */
    [83] = {0x3C, 0x66, 0x60, 0x3C, 0x06, 0x66, 0x3C, 0x00},
    /* T (84) */
    [84] = {0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00},
    /* U (85) */
    [85] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00},
    /* V (86) */
    [86] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00},
    /* X (88) */
    [88] = {0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00},
    /* Y (89) */
    [89] = {0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00},
    /* 0 (48) */
    [48] = {0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x3C, 0x00},
    /* 1 (49) */
    [49] = {0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00},
    /* 2 (50) */
    [50] = {0x3C, 0x66, 0x06, 0x1C, 0x30, 0x60, 0x7E, 0x00},
    /* 3 (51) */
    [51] = {0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0x00},
    /* 4 (52) */
    [52] = {0x0C, 0x1C, 0x3C, 0x6C, 0x7E, 0x0C, 0x0C, 0x00},
    /* 5 (53) */
    [53] = {0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00},
    /* 6 (54) */
    [54] = {0x1C, 0x30, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00},
    /* 7 (55) */
    [55] = {0x7E, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00},
    /* 8 (56) */
    [56] = {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00},
    /* 9 (57) */
    [57] = {0x3C, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0x00},
    /* + (43) */
    [43] = {0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00},
    /* = (61) */
    [61] = {0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00},
    /* : (58) */
    [58] = {0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00},
};

void splash_screen_init(void)
{
    splash_screen.active = true;
    splash_screen.frame_counter = 0;
    splash_screen_generate_build_date();
}

void splash_screen_generate_build_date(void)
{
    /* Generate build timestamp in DDMMYYHH format */
    /* For now, we'll use a fixed timestamp that can be updated via preprocessor */
#ifdef BUILD_TIMESTAMP
    strncpy(splash_screen.build_date, BUILD_TIMESTAMP, sizeof(splash_screen.build_date) - 1);
    splash_screen.build_date[sizeof(splash_screen.build_date) - 1] = '\0';
#else
    /* Default fallback */
    strcpy(splash_screen.build_date, "BUILD 2907250119");
#endif
}

bool splash_screen_update(void)
{
    if (!splash_screen.active) {
        return false;
    }
    
    splash_screen.frame_counter++;
    
    if (splash_screen.frame_counter >= SPLASH_DURATION_FRAMES) {
        splash_screen.active = false;
        return false;
    }
    
    return true;
}

static void draw_char(uint16_t* screen, int screen_width, int x, int y, char c, uint16_t color, int scale_x, int scale_y)
{
    const uint8_t* font_data = splash_font_8x8[(unsigned char)c];
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (font_data[row] & (0x80 >> col)) {
                /* Draw scaled pixel block */
                for (int sy = 0; sy < scale_y; sy++) {
                    for (int sx = 0; sx < scale_x; sx++) {
                        int px = x + col * scale_x + sx;
                        int py = y + row * scale_y + sy;
                        if (px >= 0 && px < screen_width && py >= 0 && py < 240) {
                            screen[py * screen_width + px] = color;
                        }
                    }
                }
            }
        }
    }
}

static void draw_string(uint16_t* screen, int screen_width, int x, int y, const char* str, uint16_t color, int scale_x, int scale_y)
{
    int char_x = x;
    while (*str) {
        draw_char(screen, screen_width, char_x, y, *str, color, scale_x, scale_y);
        char_x += 8 * scale_x;
        str++;
    }
}

void splash_screen_render(uint16_t* screen, int width, int height, int pitch)
{
    if (!splash_screen.active) {
        return;
    }
    
    /* Fill background with splash color */
    uint16_t* pixel = screen;
#ifdef SF2000_ARITHMETIC_OPTS
    int pixels_per_line = pitch >> 1; /* pitch is in bytes, we need 16-bit pixels (/ 2 → >>1) */
#else
    int pixels_per_line = pitch / 2; /* pitch is in bytes, we need 16-bit pixels */
#endif
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            pixel[x] = SPLASH_BG_COLOR;
        }
        pixel += pixels_per_line;
    }
    
    /* Calculate text positions (centered) with larger fonts */
#ifdef SF2000_ARITHMETIC_OPTS
    int center_x = width >> 1;  /* /2 → >>1 */
#else
    int center_x = width / 2;
#endif
    int scale_big_x = 1;   /* 1x horizontal scale for main text to fix stretching */
    int scale_big_y = 2;   /* 2x vertical scale for main text */
    int scale_small_x = 1; /* 1x horizontal scale for build version */
    int scale_small_y = 1; /* 1x vertical scale for build version */
    
    /* Line 1: "SNES9X2005" - Large font, first part */
    const char* line1 = "SNES9X2005";
    int line1_width = strlen(line1) * 8 * scale_big_x;
    draw_string(screen, width, center_x - line1_width/2, 50, line1, SPLASH_TEXT_COLOR, scale_big_x, scale_big_y);
    
    /* Line 2: "DASH V" - Large font, second part */
    const char* line2 = "DASH V";
    int line2_width = strlen(line2) * 8 * scale_big_x;
    draw_string(screen, width, center_x - line2_width/2, 70, line2, SPLASH_TEXT_COLOR, scale_big_x, scale_big_y);
    
    /* Line 3: "TEST BUILD" - Large font (changed from "EXPERIMENTAL BUILD") */
    const char* line3 = "TEST BUILD";
    int line3_width = strlen(line3) * 8 * scale_big_x;
    draw_string(screen, width, center_x - line3_width/2, 100, line3, SPLASH_TEXT_COLOR, scale_big_x, scale_big_y);
    
    /* Line 4: "BY PROSTY" - Large font */
    const char* line4 = "BY PROSTY";
    int line4_width = strlen(line4) * 8 * scale_big_x;
    draw_string(screen, width, center_x - line4_width/2, 130, line4, SPLASH_TEXT_COLOR, scale_big_x, scale_big_y);
    
    /* Line 5: Hotkey information - Large font, black color */
    const char* line5 = "SEL + R = TRANSPARENCY";
    int line5_width = strlen(line5) * 8 * scale_big_x;
    draw_string(screen, width, center_x - line5_width/2, 145, line5, SPLASH_BLACK_COLOR, scale_big_x, scale_big_y);
    
    /* Line 6: Second hotkey - Large font, black color */
    const char* line6 = "SEL + L = AUDIO";
    int line6_width = strlen(line6) * 8 * scale_big_x;
    draw_string(screen, width, center_x - line6_width/2, 160, line6, SPLASH_BLACK_COLOR, scale_big_x, scale_big_y);
    
    /* Line 7: Third hotkey - Large font, black color */
    const char* line7 = "SEL + A = FRAMESKIP";
    int line7_width = strlen(line7) * 8 * scale_big_x;
    draw_string(screen, width, center_x - line7_width/2, 175, line7, SPLASH_BLACK_COLOR, scale_big_x, scale_big_y);
    
    /* SPACER - create gap between hotkeys and version */
    
    /* Version number - large font, at actual screen bottom */
    const char* version = "V1.43 SF2000 OPTIMIZED";
    int version_width = strlen(version) * 8 * scale_big_x;
    draw_string(screen, width, center_x - version_width/2, height - 32, version, SPLASH_TEXT_COLOR, scale_big_x, scale_big_y);
    
    /* Build timestamp - large font, at very bottom */
    int line8_width = strlen(splash_screen.build_date) * 8 * scale_big_x;
    draw_string(screen, width, center_x - line8_width/2, height - 16, splash_screen.build_date, SPLASH_TEXT_COLOR, scale_big_x, scale_big_y);
}

#endif /* SF2000 */