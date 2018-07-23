#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <tgmath.h>

#ifdef SWITCH
    #include <switch.h>
#else
    #include "switchdefs.h"
#endif

#include "draw.h"
#include "switchcraft.h"

static u32 color_black = 0xFF000000;
static u32 color_red = 0xFF0000FF;
static u32 color_green = 0xFF00FF00;
static u32 color_blue = 0xFFFF0000;
static u32 color_white = 0xFFFFFFFF;
static u32 color_yellow = 0xFF00FFFF;
static u32 color_orange = 0xFFE74116;
static u32 color_brown = 0xFF524138;
static u32 color_grey = 0xFF5C5C5A;
static u32 color_pink = 0xFFC51DB5;

bool mapInitDone = false;

const int fbheight = 720;
const int fbwidth = 1280;

float camRotX = 0;
float camRotY = 0;
float camPosX = 0;
float camPosY = 0;
float camPosZ = 0;

float editCubePosX;
float editCubePosY;
float editCubePosZ;
bool editCubeRemove = false;
int maxEditDist = 10;

int selectedBlockType = 4;

int worldSize = 256;
int worldHeight = 128;

int fov = 40;

int cubeMap[128][256][256]; //[worldHeight][worldSize][worldSize]

bool tCI = false;

int jumpFrame = 0;
int jumpHeight = 15;
bool jumping = false;
float gravity = 0.05;

float math_pi = 3.14159265359;

static inline int math_rint( float x ) {
    return rint( x );
}

static inline float math_sin( float x ) {
    return sinf( x );
}

static inline float math_cos( float x ) {
    return cosf( x );
}

void update() {
    u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
    u64 kHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
    if (mapInitDone == false) {
    	for (int y = 0; y < worldHeight; y++) {
            for (int x = 0; x < worldSize; x++) {
                for (int z = 0; z < worldSize; z++) {
                    cubeMap[y][x][z] = 0;
                }
            }
        }
        mapInitDone = true;
    }
    float speed = 0;
    if (kHeld & KEY_X) {
        speed = 0.05;
    }
    if (kHeld & KEY_B) {
        speed = -0.05;
    }
    int camPosXR = math_rint(camPosX);
    int camPosYR = math_rint(camPosY);
    int camPosZR = math_rint(camPosZ);
    if (speed != 0) {
        if (camRotY > 0 && camRotY <= 90) {
            if (cubeMap[(camPosYR - 1) + (worldHeight / 2)][math_rint(camPosX + (math_sin(camRotY) * 0.5)) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0 && cubeMap[(camPosYR) + (worldHeight / 2)][math_rint(camPosX + (math_sin(camRotY) * 0.5)) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0) {
                camPosX += math_sin(camRotY) * speed;
            }
            if (cubeMap[(camPosYR - 1) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][math_rint(camPosZ + (math_cos(camRotY) * 0.5)) + (worldSize / 2)] == 0 && cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][math_rint(camPosZ + (math_cos(camRotY) * 0.5)) + (worldSize / 2)] == 0) {
                camPosZ += math_cos(camRotY) * speed;
            }
        } else if (camRotY > 90 && camRotY <= 180) {
            if (cubeMap[(camPosYR - 1) + (worldHeight / 2)][math_rint(camPosX + (math_cos(camRotY) * 0.5)) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0 && cubeMap[(camPosYR) + (worldHeight / 2)][math_rint(camPosX + (math_cos(camRotY) * 0.5)) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0) {
                camPosX += math_cos(camRotY - 90) * speed;
            }
            if (cubeMap[(camPosYR - 1) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][math_rint(camPosZ - (math_sin(camRotY) * 0.5)) + (worldSize / 2)] == 0 && cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][math_rint(camPosZ - (math_sin(camRotY) * 0.5)) + (worldSize / 2)] == 0) {
                camPosZ -= math_sin(camRotY - 90) * speed;
            }
        } else if (camRotY > 180 && camRotY <= 270) {
            if (cubeMap[(camPosYR - 1) + (worldHeight / 2)][math_rint(camPosX - (math_sin(camRotY) * 0.5)) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0 && cubeMap[(camPosYR) + (worldHeight / 2)][math_rint(camPosX - (math_sin(camRotY) * 0.5)) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0) {
                camPosX -= math_sin(camRotY - 180) * speed;
            }
            if (cubeMap[(camPosYR - 1) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][math_rint(camPosZ - (math_cos(camRotY) * 0.5)) + (worldSize / 2)] == 0 && cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][math_rint(camPosZ - (math_cos(camRotY) * 0.5)) + (worldSize / 2)] == 0) {
                camPosZ -= math_cos(camRotY - 180) * speed;
            }
        } else if (camRotY > 270 && camRotY <= 360) {
            if (cubeMap[(camPosYR - 1) + (worldHeight / 2)][math_rint(camPosX - (math_cos(camRotY) * 0.5)) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0 && cubeMap[(camPosYR) + (worldHeight / 2)][math_rint(camPosX - (math_cos(camRotY) * 0.5)) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0) {
                camPosX -= math_cos(camRotY - 270) * speed;
            }
            if (cubeMap[(camPosYR - 1) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][math_rint(camPosZ + (math_sin(camRotY) * 0.5)) + (worldSize / 2)] == 0 && cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][math_rint(camPosZ + (math_sin(camRotY) * 0.5)) + (worldSize / 2)] == 0) {
                camPosZ += math_sin(camRotY - 270) * speed;
            }
        }
        speed = 0;
    }
    camPosXR = math_rint(camPosX);
    camPosYR = math_rint(camPosY);
    camPosZR = math_rint(camPosZ);
    if (cubeMap[(camPosYR - 2) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0 && jumping == false) {
        camPosY -= gravity;
    }
    if (jumping == false && kDown & KEY_A && cubeMap[(camPosYR - 2) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] != 0) {
        if (jumping == false) {
            jumping = true;
        }
    }
    if (jumping == true) {
        if (cubeMap[(camPosYR + 2) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] == 0) {
            if (jumpFrame < (jumpHeight * 2)) {
                camPosY += gravity;
                jumpFrame++;
            } else {
                camPosY += gravity;
                jumpFrame = 0;
                jumping = false;
            }
        } else {
            jumpFrame = 0;
            jumping = false;
        }
    }
    if (kDown & KEY_PLUS) {
        running = 0;
    }
    if (kDown & KEY_MINUS) {
        if (editCubeRemove == false) {
            editCubeRemove = true;
        } else {
            editCubeRemove = false;
        }
    }
    if (camRotY <= 0) {
        camRotY += 360;
    } else if (camRotY > 360) {
        camRotY -= 360;
    }
    if (kHeld & KEY_LEFT) {
        camRotY -= 0.05;
    }
    if (kHeld & KEY_RIGHT) {
        camRotY += 0.05;
    }
    if (camRotY <= 0) {
        camRotY += 360;
    } else if (camRotY > 360) {
        camRotY -= 360;
    }
    if (camRotX <= 0) {
        camRotX += 360;
    } else if (camRotX > 360) {
        camRotX -= 360;
    }
    if ((kHeld & KEY_UP) && ((camRotX <= 360 && camRotX > 270) || (camRotX >= 0 && camRotX < 85))) {
        camRotX += 0.05;
    }
    if ((kHeld & KEY_DOWN) && ((camRotX <= 360 && camRotX > 265) || (camRotX >= 0 && camRotX < 90))) {
        camRotX -= 0.05;
    }
    if (camRotX <= 0) {
        camRotX += 360;
    } else if (camRotX > 360) {
        camRotX -= 360;
    }
    if (tCI == false) {
        setCube(0, 0, 4, 2);
        setCube(1, -1, 3, 3);
        setCube(1, -1, 5, 4);
        setCube(-1, -1, 3, 4);
        setCube(-1, -1, 5, 3);
        setCube(1, 1, 3, 4);
        setCube(1, 1, 5, 3);
        setCube(-1, 1, 3, 3);
        setCube(-1, 1, 5, 4);
        int y = -2;
        for (int x = -(worldSize / 2); x <= (worldSize / 2); x++) {
            for (int z = -(worldSize / 2); z <= (worldSize / 2); z++) {
                setCube(x, y, z, 4);
            }
        }
        tCI = true;
    }
    editCubePosX = camPosX;
    editCubePosY = camPosY;
    editCubePosZ = camPosZ;
    for (int i = 0; i <= maxEditDist; i++) {
        if (camRotX >= 0 && camRotX <= 90) {
            if (camRotY > 0 && camRotY <= 90) {
                if ((editCubeRemove == false && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX + math_sin(camRotY) - (math_sin(camRotY) * math_sin(camRotX))) + (worldSize / 2)][math_rint(editCubePosZ + math_cos(camRotY) - (math_cos(camRotY) * math_sin(camRotX))) + (worldSize / 2)] == 0) || (editCubeRemove == true && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX) + (worldSize / 2)][math_rint(editCubePosZ) + (worldSize / 2)] == 0)) {
                    editCubePosX += math_sin(camRotY) - (math_sin(camRotY) * math_sin(camRotX));
                    editCubePosZ += math_cos(camRotY) - (math_cos(camRotY) * math_sin(camRotX));
                    editCubePosY += math_sin(camRotX);
                }
            } else if (camRotY > 90 && camRotY <= 180) {
                if ((editCubeRemove == false && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX + math_cos(camRotY) - (math_cos(camRotY - 90) * math_sin(camRotX))) + (worldSize / 2)][math_rint(editCubePosZ - math_sin(camRotY) + (math_sin(camRotY - 90) * math_sin(camRotX))) + (worldSize / 2)] == 0) || (editCubeRemove == true && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX) + (worldSize / 2)][math_rint(editCubePosZ) + (worldSize / 2)] == 0)) {
                    editCubePosX += math_cos(camRotY - 90) - (math_cos(camRotY - 90) * math_sin(camRotX));
                    editCubePosZ -= math_sin(camRotY - 90) + (math_sin(camRotY - 90) * math_sin(camRotX));
                    editCubePosY += math_sin(camRotX);
                }
            } else if (camRotY > 180 && camRotY <= 270) {
                if ((editCubeRemove == false && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX - math_sin(camRotY) + (math_sin(camRotY - 180) * math_sin(camRotX))) + (worldSize / 2)][math_rint(editCubePosZ - math_cos(camRotY) + (math_cos(camRotY - 180) * math_sin(camRotX))) + (worldSize / 2)] == 0) || (editCubeRemove == true && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX) + (worldSize / 2)][math_rint(editCubePosZ) + (worldSize / 2)] == 0)) {
                    editCubePosX -= math_sin(camRotY - 180) + (math_sin(camRotY - 180) * math_sin(camRotX));
                    editCubePosZ -= math_cos(camRotY - 180) + (math_cos(camRotY - 180) * math_sin(camRotX));
                    editCubePosY += math_sin(camRotX);
                }
            } else if (camRotY > 270 && camRotY <= 360) {
                if ((editCubeRemove == false && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX - math_cos(camRotY) + (math_cos(camRotY - 270) * math_sin(camRotX))) + (worldSize / 2)][math_rint(editCubePosZ + math_sin(camRotY) - (math_sin(camRotY - 270) * math_sin(camRotX))) + (worldSize / 2)] == 0) || (editCubeRemove == true && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX) + (worldSize / 2)][math_rint(editCubePosZ) + (worldSize / 2)] == 0)) {
                    editCubePosX -= math_cos(camRotY - 270) + (math_cos(camRotY - 270) * math_sin(camRotX));
                    editCubePosZ += math_sin(camRotY - 270) - (math_sin(camRotY - 270) * math_sin(camRotX));
                    editCubePosY += math_sin(camRotX);
                }
            }
        } else if (camRotX >= 270 && camRotX <= 360) {
            if (camRotY > 0 && camRotY <= 90) {
                if ((editCubeRemove == false && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX + math_sin(camRotY) - (math_sin(camRotY) * math_cos(camRotX))) + (worldSize / 2)][math_rint(editCubePosZ + math_cos(camRotY) - (math_cos(camRotY) * math_cos(camRotX))) + (worldSize / 2)] == 0) || (editCubeRemove == true && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX) + (worldSize / 2)][math_rint(editCubePosZ) + (worldSize / 2)] == 0)) {
                    editCubePosX += math_sin(camRotY) - (math_sin(camRotY) * math_cos(camRotX));
                    editCubePosZ += math_cos(camRotY) - (math_cos(camRotY) * math_cos(camRotX));
                    editCubePosY -= math_cos(camRotX - 270);
                }
            } else if (camRotY > 90 && camRotY <= 180) {
                if ((editCubeRemove == false && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX + math_cos(camRotY) - (math_cos(camRotY - 90) * math_cos(camRotX))) + (worldSize / 2)][math_rint(editCubePosZ - math_sin(camRotY) + (math_sin(camRotY - 90) * math_sin(camRotX))) + (worldSize / 2)] == 0) || (editCubeRemove == true && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX) + (worldSize / 2)][math_rint(editCubePosZ) + (worldSize / 2)] == 0)) {
                    editCubePosX += math_cos(camRotY - 90) - (math_cos(camRotY - 90) * math_cos(camRotX));
                    editCubePosZ -= math_sin(camRotY - 90) + (math_sin(camRotY - 90) * math_sin(camRotX));
                    editCubePosY -= math_cos(camRotX - 270);
                }
            } else if (camRotY > 180 && camRotY <= 270) {
                if ((editCubeRemove == false && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX - math_sin(camRotY) + (math_sin(camRotY - 180) * math_cos(camRotX))) + (worldSize / 2)][math_rint(editCubePosZ - math_cos(camRotY) + (math_cos(camRotY - 180) * math_cos(camRotX))) + (worldSize / 2)] == 0) || (editCubeRemove == true && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX) + (worldSize / 2)][math_rint(editCubePosZ) + (worldSize / 2)] == 0)) {
                    editCubePosX -= math_sin(camRotY - 180) + (math_sin(camRotY - 180) * math_cos(camRotX));
                    editCubePosZ -= math_cos(camRotY - 180) + (math_cos(camRotY - 180) * math_cos(camRotX));
                    editCubePosY -= math_cos(camRotX - 270);
                }
            } else if (camRotY > 270 && camRotY <= 360) {
                if ((editCubeRemove == false && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX - math_cos(camRotY) + (math_cos(camRotY - 270) * math_cos(camRotX))) + (worldSize / 2)][math_rint(editCubePosZ + math_sin(camRotY) - (math_sin(camRotY - 270) * math_cos(camRotX))) + (worldSize / 2)] == 0) || (editCubeRemove == true && cubeMap[math_rint(editCubePosY) + (worldHeight / 2)][math_rint(editCubePosX) + (worldSize / 2)][math_rint(editCubePosZ) + (worldSize / 2)] == 0)) {
                    editCubePosX -= math_cos(camRotY - 270) + (math_cos(camRotY - 270) * math_cos(camRotX));
                    editCubePosZ += math_sin(camRotY - 270) - (math_sin(camRotY - 270) * math_cos(camRotX));
                    editCubePosY -= math_cos(camRotX - 270);
                }
            }
        }
    }
    int editCubePosXR = math_rint(editCubePosX);
    int editCubePosYR = math_rint(editCubePosY);
    int editCubePosZR = math_rint(editCubePosZ);
    if (kDown & KEY_Y) {
        if (editCubeRemove == true) {
            setCube(editCubePosXR, editCubePosYR, editCubePosZR, 0);
        } else {
            setCube(editCubePosXR, editCubePosYR, editCubePosZR, selectedBlockType);
        }
    }
    drawClearScreen(color_black);
    renderMap();
    renderCube(editCubePosXR, editCubePosYR, editCubePosZR, 10);
}

void initSwitchcraft() {
    drawStart();
}

void setCube(int x, int y, int z, int blockType) {
    if ((y + (worldHeight / 2)) >= 0 && (y + (worldHeight / 2)) < worldHeight && (x + (worldSize /  2)) >= 0 && (x + (worldSize / 2)) < worldSize && (z + (worldSize / 2)) >= 0 && (z + (worldSize / 2)) < worldSize) {
        cubeMap[y + (worldHeight / 2)][x + (worldSize / 2)][z + (worldSize / 2)] = blockType;
    }
}

void renderMap() {
    int camPosXR = math_rint(camPosX);
    int camPosYR = math_rint(camPosY);
    int camPosZR = math_rint(camPosZ);
    for (int bR = 0; bR < 8; bR++) {
        for (int y = -(fov / 2); y < 0; y++) {
            for (int x = -(fov / 2); x < 0; x++) {
                for (int z = -(fov / 2); z < 0; z++) {
                    if ((camPosXR + x) >= -(worldSize / 2) && (camPosXR + x) <= (worldSize / 2) && (camPosYR + y) >= -(worldHeight / 2) && (camPosYR + y) <= (worldHeight / 2) && (camPosZR + z) >= -(worldSize / 2) && (camPosZR + z) <= (worldSize / 2)) {
                        if (bR == 0) {
                            if (cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)] != 0) {
                                renderCube((camPosXR + x), (camPosYR + y), (camPosZR + z), cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)]);
                            }
                        } else if (bR == 1) {
                            if (cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)] != 0) {
                                renderCube((camPosXR + x), (camPosYR + y), (camPosZR - z), cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)]);
                            }
                        } else if (bR == 2) {
                            if (cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)] != 0) {
                                renderCube((camPosXR - x), (camPosYR + y), (camPosZR - z), cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)]);
                            }
                        } else if (bR == 3) {
                            if (cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)] != 0) {
                                renderCube((camPosXR - x), (camPosYR + y), (camPosZR + z), cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)]);
                            }
                        } else if (bR == 4) {
                            if (cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)] != 0) {
                                renderCube((camPosXR + x), (camPosYR - y), (camPosZR + z), cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)]);
                            }
                        } else if (bR == 5) {
                            if (cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)] != 0) {
                                renderCube((camPosXR + x), (camPosYR - y), (camPosZR - z), cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)]);
                            }
                        } else if (bR == 6) {
                            if (cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)] != 0) {
                                renderCube((camPosXR - x), (camPosYR - y), (camPosZR - z), cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)]);
                            }
                        } else if (bR == 7) {
                            if (cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)] != 0) {
                                renderCube((camPosXR - x), (camPosYR - y), (camPosZR + z), cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)]);
                            }
                        }
                    }
                }
            }
        }
    }
    for (int bR = 0; bR < 4; bR++) {
        for (int y = -(fov / 2); y < 0; y++) {
            for (int x = -(fov / 2); x < 0; x++) {
                if ((camPosXR + x) >= -(worldSize / 2) && (camPosXR + x) <= (worldSize / 2) && (camPosYR + y) >= -(worldHeight / 2) && (camPosYR + y) <= (worldHeight / 2) && (camPosZR) >= -(worldSize / 2) && (camPosZR) <= (worldSize / 2)) {
                    if (bR == 0) {
                        if (cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR - x), (camPosYR - y), (camPosZR), cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)]);
                        }
                    } else if (bR == 1) {
                        if (cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR - x), (camPosYR + y), (camPosZR), cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)]);
                        }
                    } else if (bR == 2) {
                        if (cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR + x), (camPosYR + y), (camPosZR), cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)]);
                        }
                    } else if (bR == 3) {
                        if (cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR + x), (camPosYR - y), (camPosZR), cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)]);
                        }
                    }
                }
            }
        }
    }
    for (int bR = 0; bR < 4; bR++) {
        for (int y = -(fov / 2); y < 0; y++) {
            for (int z = -(fov / 2); z < 0; z++) {
                if ((camPosXR) >= -(worldSize / 2) && (camPosXR) <= (worldSize / 2) && (camPosYR + y) >= -(worldHeight / 2) && (camPosYR + y) <= (worldHeight / 2) && (camPosZR + z) >= -(worldSize / 2) && (camPosZR + z) <= (worldSize / 2)) {
                    if (bR == 0) {
                        if (cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR), (camPosYR - y), (camPosZR - z), cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)]);
                        }
                    } else if (bR == 1) {
                        if (cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR), (camPosYR + y), (camPosZR - z), cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)]);
                        }
                    } else if (bR == 2) {
                        if (cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR), (camPosYR + y), (camPosZR + z), cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)]);
                        }
                    } else if (bR == 3) {
                        if (cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR), (camPosYR - y), (camPosZR + z), cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)]);
                        }
                    }
                }
            }
        }
    }
    for (int bR = 0; bR < 4; bR++) {
        for (int x = -(fov / 2); x < 0; x++) {
            for (int z = -(fov / 2); z < 0; z++) {
                if ((camPosXR + x) >= -(worldSize / 2) && (camPosXR + x) <= (worldSize / 2) && (camPosYR) >= -(worldHeight / 2) && (camPosYR) <= (worldHeight / 2) && (camPosZR + z) >= -(worldSize / 2) && (camPosZR + z) <= (worldSize / 2)) {
                    if (bR == 0) {
                        if (cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR - x), (camPosYR), (camPosZR - z), cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)]);
                        }
                    } else if (bR == 1) {
                        if (cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR + x), (camPosYR), (camPosZR - z), cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)]);
                        }
                    } else if (bR == 2) {
                        if (cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR + x), (camPosYR), (camPosZR + z), cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)]);
                        }
                    } else if (bR == 3) {
                        if (cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)] != 0) {
                            renderCube((camPosXR - x), (camPosYR), (camPosZR + z), cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)]);
                        }
                    }
                }
            }
        }
    }
    for (int bR = 0; bR < 2; bR++) {
        for (int x = -(fov / 2); x < 0; x++) {
            if ((camPosXR + x) >= -(worldSize / 2) && (camPosXR + x) <= (worldSize / 2) && (camPosYR) >= -(worldHeight / 2) && (camPosYR) <= (worldHeight / 2) && (camPosZR) >= -(worldSize / 2) && (camPosZR) <= (worldSize / 2)) {
                if (bR == 0) {
                    if (cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] != 0) {
                        renderCube((camPosXR + x), (camPosYR), (camPosZR), cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR + x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)]);
                    }
                } else if (bR == 1) {
                    if (cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] != 0) {
                        renderCube((camPosXR - x), (camPosYR), (camPosZR), cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR - x) + (worldSize / 2)][(camPosZR) + (worldSize / 2)]);
                    }
                }
            }
        }
    }
    for (int bR = 0; bR < 2; bR++) {
        for (int y = -(fov / 2); y < 0; y++) {
            if ((camPosXR) >= -(worldSize / 2) && (camPosXR) <= (worldSize / 2) && (camPosYR + y) >= -(worldHeight / 2) && (camPosYR + y) <= (worldHeight / 2) && (camPosZR) >= -(worldSize / 2) && (camPosZR) <= (worldSize / 2)) {
                if (bR == 0) {
                    if (cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] != 0) {
                        renderCube((camPosXR), (camPosYR + y), (camPosZR), cubeMap[(camPosYR + y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR) + (worldSize / 2)]);
                    }
                } else if (bR == 1) {
                    if (cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR) + (worldSize / 2)] != 0) {
                        renderCube((camPosXR), (camPosYR - y), (camPosZR), cubeMap[(camPosYR - y) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR) + (worldSize / 2)]);
                    }
                }
            }
        }
    }
    for (int bR = 0; bR < 2; bR++) {
        for (int z = -(fov / 2); z < 0; z++) {
            if ((camPosXR) >= -(worldSize / 2) && (camPosXR) <= (worldSize / 2) && (camPosYR) >= -(worldHeight / 2) && (camPosYR) <= (worldHeight / 2) && (camPosZR + z) >= -(worldSize / 2) && (camPosZR + z) <= (worldSize / 2)) {
                if (bR == 0) {
                    if (cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)] != 0) {
                        renderCube((camPosXR), (camPosYR), (camPosZR + z), cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR + z) + (worldSize / 2)]);
                    }
                } else if (bR == 1) {
                    if (cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)] != 0) {
                        renderCube((camPosXR), (camPosYR), (camPosZR - z), cubeMap[(camPosYR) + (worldHeight / 2)][(camPosXR) + (worldSize / 2)][(camPosZR - z) + (worldSize / 2)]);
                    }
                }
            }
        }
    }
}

void renderCube(float posX, float posY, float posZ, int blockType) {
    u32 color;
    int pXint = math_rint(posX);
    int pYint = math_rint(posY);
    int pZint = math_rint(posZ);
    if (blockType == 1) {
	color = color_black;
    } else if (blockType == 2) {
    	color = color_red;
    } else if (blockType == 3) {
    	color = color_blue;
    } else if (blockType == 4) {
    	color = color_green;
    } else if (blockType == 5) {
    	color = color_yellow;
    } else if (blockType == 6) {
    	color = color_orange;
    } else if (blockType == 7) {
    	color = color_brown;
    } else if (blockType == 8) {
    	color = color_grey;
    } else if (blockType == 9) {
    	color = color_pink;
    } else if (blockType == 10) {
    	color = color_white;
    } else {
    	color = color_white;
    }
    bool fTR[6];
    for (int i = 0; i < 6; i++) {
        fTR[i] = true;
    }
    if (posX < (worldSize / 2)) {
        if (cubeMap[pYint + (worldHeight / 2)][pXint + (worldSize / 2) + 1][pZint + (worldSize / 2)] != 0) {
            fTR[3] = false;
        }
    }
    if (posX < (worldSize / 2)) {
        if (cubeMap[pYint + (worldHeight / 2)][pXint + (worldSize / 2) - 1][pZint + (worldSize / 2)] != 0) {
            fTR[1] = false;
        }
    }
    if (posY < (worldSize / 2)) {
        if (cubeMap[pYint + (worldHeight / 2) + 1][pXint + (worldSize / 2)][pZint + (worldSize / 2)] != 0) {
            fTR[4] = false;
        }
    }
    if (posY < (worldSize / 2)) {
        if (cubeMap[pYint + (worldHeight / 2) - 1][pXint + (worldSize / 2)][pZint + (worldSize / 2)] != 0) {
            fTR[5] = false;
        }
    }
    if (posZ < (worldSize / 2)) {
        if (cubeMap[pYint + (worldHeight / 2)][pXint + (worldSize / 2)][pZint + (worldSize / 2) + 1] != 0) {
            fTR[2] = false;
        }
    }
    if (posZ < (worldSize / 2)) {
        if (cubeMap[pYint + (worldHeight / 2)][pXint + (worldSize / 2)][pZint + (worldSize / 2) - 1] != 0) {
            fTR[0] = false;
        }
    }
    if (posX > camPosX) {
        fTR[3] = false;
    }
    if (posX < camPosX) {
        fTR[1] = false;
    }
    if (posY > camPosY) {
        fTR[4] = false;
    }
    if (posY < camPosY) {
        fTR[5] = false;
    }
    if (posZ > camPosZ) {
        fTR[2] = false;
    }
    if (posZ < camPosZ) {
        fTR[0] = false;
    }
    posX -= camPosX;
    posY -= camPosY;
    posZ -= camPosZ;
    float p0x = posX - 0.5;
    float p0y = posY + 0.5;
    float p0z = posZ - 0.5;
    float p1x = posX - 0.5;
    float p1y = posY - 0.5;
    float p1z = posZ - 0.5;
    float p2x = posX + 0.5;
    float p2y = posY + 0.5;
    float p2z = posZ - 0.5;
    float p3x = posX + 0.5;
    float p3y = posY - 0.5;
    float p3z = posZ - 0.5;
    float p4x = posX + 0.5;
    float p4y = posY + 0.5;
    float p4z = posZ + 0.5;
    float p5x = posX + 0.5;
    float p5y = posY - 0.5;
    float p5z = posZ + 0.5;
    float p6x = posX - 0.5;
    float p6y = posY + 0.5;
    float p6z = posZ + 0.5;
    float p7x = posX - 0.5;
    float p7y = posY - 0.5;
    float p7z = posZ + 0.5;
    float cubePosX = posX;
    float cubePosY = posY;
    float cubePosZ = posZ;
    bool p0set = false;
    bool p1set = false;
    bool p2set = false;
    bool p3set = false;
    bool p4set = false;
    bool p5set = false;
    bool p6set = false;
    bool p7set = false;
    for (int i = 0; i < 9; i++) {
        float pX;
        float pY;
        float pZ;
        bool rTP = true;
        if (i == 0 && (fTR[0] || fTR[1] || fTR[4])) {
            pX = p0x;
            pY = p0y;
            pZ = p0z;
            p0set = true;
        } else if (i == 1 && (fTR[0] || fTR[1] || fTR[5])) {
            pX = p1x;
            pY = p1y;
            pZ = p1z;
            p1set = true;
        } else if (i == 2 && (fTR[3] || fTR[0] || fTR[4])) {
            pX = p2x;
            pY = p2y;
            pZ = p2z;
            p2set = true;
        } else if (i == 3 && (fTR[3] || fTR[0] || fTR[5])) {
            pX = p3x;
            pY = p3y;
            pZ = p3z;
            p3set = true;
        } else if (i == 4 && (fTR[2] || fTR[3] || fTR[4])) {
            pX = p4x;
            pY = p4y;
            pZ = p4z;
            p4set = true;
        } else if (i == 5 && (fTR[2] || fTR[3] || fTR[5])) {
            pX = p5x;
            pY = p5y;
            pZ = p5z;
            p5set = true;
        } else if (i == 6 && (fTR[1] || fTR[2] || fTR[4])) {
            pX = p6x;
            pY = p6y;
            pZ = p6z;
            p6set = true;
        } else if (i == 7 && (fTR[1] || fTR[2] || fTR[5])) {
            pX = p7x;
            pY = p7y;
            pZ = p7z;
            p7set = true;
        } else if (i == 8) {
            pX = cubePosX;
            pY = cubePosY;
            pZ = cubePosZ;
        } else {
            rTP = false;
        }
        float yPosRTC;
        float xPosRTC;
        float zPosRTC;
        float zPosRTC2;
        if (rTP == true) {
            if (camRotY >= 0 && camRotY < 90) {
                xPosRTC = (math_cos(camRotY) * pX) - (math_sin(camRotY) * pZ);
                zPosRTC = (math_cos(camRotY) * pZ) + (math_sin(camRotY) * pX);
            } else if (camRotY >= 90 && camRotY < 180) {
                xPosRTC = (-math_cos(camRotY - 90) * pZ) - (math_sin(camRotY - 90) * pX);
                zPosRTC = (math_cos(camRotY - 90) * pX) - (math_sin(camRotY - 90) * pZ);
            } else if (camRotY >= 180 && camRotY < 270) {
                xPosRTC = (-math_cos(camRotY - 180) * pX) + (math_sin(camRotY - 180) * pZ);
                zPosRTC = (-math_cos(camRotY - 180) * pZ) - (math_sin(camRotY - 180) * pX);
            } else if (camRotY >= 270 && camRotY <= 360) {
                xPosRTC = (math_cos(camRotY - 270) * pZ) + (math_sin(camRotY - 270) * pX);
                zPosRTC = (-math_cos(camRotY - 270) * pX) + (math_sin(camRotY - 270) * pZ);
            }
            if (camRotX >= 0 && camRotX <= 90) {
                yPosRTC = -((math_cos(camRotX) * pY) - (math_sin(camRotX) * zPosRTC));
                zPosRTC2 = (math_cos(camRotX) * zPosRTC) + (math_sin(camRotX) * pY);
            } else if (camRotX >= 270 && camRotX <= 360) {
                yPosRTC = -((math_cos(camRotX - 270) * zPosRTC) + (math_sin(camRotX - 270) * pY));
                zPosRTC2 = (-math_cos(camRotX - 270) * pY) + (math_sin(camRotX - 270) * zPosRTC);
            }
            if (i == 0) {
                p0x = xPosRTC;
                p0y = yPosRTC;
                p0z = zPosRTC2;
            } else if (i == 1) {
                p1x = xPosRTC;
                p1y = yPosRTC;
                p1z = zPosRTC2;
            } else if (i == 2) {
                p2x = xPosRTC;
                p2y = yPosRTC;
                p2z = zPosRTC2;
            } else if (i == 3) {
                p3x = xPosRTC;
                p3y = yPosRTC;
                p3z = zPosRTC2;
            } else if (i == 4) {
                p4x = xPosRTC;
                p4y = yPosRTC;
                p4z = zPosRTC2;
            } else if (i == 5) {
                p5x = xPosRTC;
                p5y = yPosRTC;
                p5z = zPosRTC2;
            } else if (i == 6) {
                p6x = xPosRTC;
                p6y = yPosRTC;
                p6z = zPosRTC2;
            } else if (i == 7) {
                p7x = xPosRTC;
                p7y = yPosRTC;
                p7z = zPosRTC2;
            } else if (i == 8) {
                cubePosX = xPosRTC;
                cubePosY = yPosRTC;
                cubePosZ = zPosRTC2;
            }
        }
    }
    bool renderIt = false;
    if (cubePosZ > 0) {
        if ((cubePosX / cubePosZ) < 1.75 && (cubePosX / cubePosZ) > -1.75 && (cubePosY / cubePosZ) < 1 && (cubePosY / cubePosZ) > -1) {
            renderIt = true;
        }
    }
    if (renderIt == true) {
        float x0 = (fbwidth / 2) + ((p0x / p0z) * (fbheight / 2));
        float x1 = (fbwidth / 2) + ((p1x / p1z) * (fbheight / 2));;
        float x2 = (fbwidth / 2) + ((p2x / p2z) * (fbheight / 2));;
        float x3 = (fbwidth / 2) + ((p3x / p3z) * (fbheight / 2));;
        float x4 = (fbwidth / 2) + ((p4x / p4z) * (fbheight / 2));;
        float x5 = (fbwidth / 2) + ((p5x / p5z) * (fbheight / 2));;
        float x6 = (fbwidth / 2) + ((p6x / p6z) * (fbheight / 2));;
        float x7 = (fbwidth / 2) + ((p7x / p7z) * (fbheight / 2));;
        float y0 = (fbheight / 2) + ((p0y / p0z) * (fbheight / 2));;
        float y1 = (fbheight / 2) + ((p1y / p1z) * (fbheight / 2));;
        float y2 = (fbheight / 2) + ((p2y / p2z) * (fbheight / 2));;
        float y3 = (fbheight / 2) + ((p3y / p3z) * (fbheight / 2));;
        float y4 = (fbheight / 2) + ((p4y / p4z) * (fbheight / 2));;
        float y5 = (fbheight / 2) + ((p5y / p5z) * (fbheight / 2));;
        float y6 = (fbheight / 2) + ((p6y / p6z) * (fbheight / 2));;
        float y7 = (fbheight / 2) + ((p7y / p7z) * (fbheight / 2));;
        int x00 = math_rint(x0);
        int x01 = math_rint(x1);
        int x02 = math_rint(x2);
        int x03 = math_rint(x3);
        int x04 = math_rint(x4);
        int x05 = math_rint(x5);
        int x06 = math_rint(x6);
        int x07 = math_rint(x7);
        int y00 = math_rint(y0);
        int y01 = math_rint(y1);
        int y02 = math_rint(y2);
        int y03 = math_rint(y3);
        int y04 = math_rint(y4);
        int y05 = math_rint(y5);
        int y06 = math_rint(y6);
        int y07 = math_rint(y7);
        if (p0set && p1set) {
            drawLine(x00, y00, x01, y01, color);
        }
        if (p0set && p2set) {
            drawLine(x00, y00, x02, y02, color);
        }
        if (p1set && p3set) {
            drawLine(x01, y01, x03, y03, color);
        }
        if (p2set && p3set) {
            drawLine(x02, y02, x03, y03, color);
        }
        if (p2set && p4set) {
            drawLine(x02, y02, x04, y04, color);
        }
        if (p3set && p5set) {
            drawLine(x03, y03, x05, y05, color);
        }
        if (p4set && p5set) {
            drawLine(x04, y04, x05, y05, color);
        }
        if (p4set && p6set) {
            drawLine(x04, y04, x06, y06, color);
        }
        if (p5set && p7set) {
            drawLine(x05, y05, x07, y07, color);
        }
        if (p6set && p7set) {
            drawLine(x06, y06, x07, y07, color);
        }
        if (p6set && p0set) {
            drawLine(x06, y06, x00, y00, color);
        }
        if (p7set && p1set) {
            drawLine(x07, y07, x01, y01, color);
        }
        if (blockType == 10) {
            if (p0set && p3set) {
                drawLine(x00, y00, x03, y03, color);
            }
            if (p3set && p4set) {
                drawLine(x03, y03, x04, y04, color);
            }
            if (p4set && p7set) {
                drawLine(x04, y04, x07, y07, color);
            }
            if (p7set && p0set) {
                drawLine(x07, y07, x00, y00, color);
            }
            if (p0set && p4set) {
                drawLine(x00, y00, x04, y04, color);
            }
            if (p3set && p7set) {
                drawLine(x03, y03, x07, y07, color);
            }
        }
    }
}
