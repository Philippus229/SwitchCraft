#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#ifdef SWITCH
    #include <switch.h>
#else
    #include "switchdefs.h"
#endif

#include "draw.h"

static u32* framebuf;
static u32 fbwidth;
static u32 fbheight;

static inline int roundToInt( float x ) {
    return round( x );
}

static inline float math_abs( float x ) {
    return round( x );
}

void drawStart() {
    framebuf = (u32*)gfxGetFramebuffer(&fbwidth, &fbheight);
}

void drawClearScreen(u32 color) {
    int i;
    for (i = 0; i < fbwidth * fbheight; ++i) {
        framebuf[i] = color;
    }
}

void drawPixel(int x, int y, u32 color) {
    if (x >= 0 && x < fbwidth && y >= 0 && y < fbheight) {
        int pos = y * fbwidth + x;
        framebuf[pos] = color;
    }
}

void drawLine(int x1, int y1, int x2, int y2, u32 color) {
    if (x1 > x2) {
        int xTmp = x1;
        x1 = x2;
        x2 = xTmp;
        int yTmp = y1;
        y1 = y2;
        y2 = yTmp;
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (x1 == x2) {
        if (y1 < y2) {
            for (int y = y1; y <= y2; y++) {
                drawPixel(x1, y, color);
            }
        } else if (y1 == y2) {
            drawPixel(x1, y1, color);
        } else if (y1 > y2) {
            for (int y = y2; y <= y1; y++) {
                drawPixel(x1, y, color);
            }
        }
    } else if (dx >= dy) {
        for (int x = x1; x <= x2; x++) {
            int y = y1 + (dy * (x - x1) / dx);
            drawPixel(x, y, color);
        }
    } else {
        for (int y = y1; y <= y2; y++) {
            int x = x1 + (dx * (y - y1) / dy);
            drawPixel(x, y, color);
        }
    }
}
