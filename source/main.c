#ifdef SWITCH

#include <switch.h>

#include "switchcraft.h"

int running = 1;

int main() {
    gfxInitDefault();
    initSwitchcraft();
    while(appletMainLoop() && running) {
        hidScanInput();
        update();

        gfxFlushBuffers();
        gfxSwapBuffers();
        gfxWaitForVsync();
    }

    gfxExit();
    return 0;
}

#endif
