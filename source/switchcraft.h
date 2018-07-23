#ifndef SWITCHCRAFT_H
#define SWITCHCRAFT_H

void update();
void initSwitchcraft();
void setCube(int x, int y, int z, int blockType);
void renderMap();
void renderCube(float posX, float posY, float posZ, int blockType);

extern int running;

#endif /* SWITCHCRAFT_H */
