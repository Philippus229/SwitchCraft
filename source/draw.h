#ifndef DRAW_H
#define DRAW_H

#ifdef SWITCH
    #include <switch/types.h>
#else
    #include "switchdefs.h"
#endif

typedef struct {
    u32 width;
    u32 height;
    const u32* buf;
} Bitmap;

void drawStart();
void drawClearScreen(u32 color);
void drawPixel(int x, int y, u32 color);
void drawLine(int x1, int y1, int x2, int y2, u32 color);

#endif /* DRAW_H */
