#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <map>
#include <vector>
#include "powder.h"
#include "simConfig.h"

class Brush;

class World {
public:
    World();

    void DiscountDoubleCheck();

    void setGravity(float x, float y);
    void setBrush(Brush *brush);

    Brush* getBrush();

    void insert(Powder *p);
    void flushInsertQueue();
    void update();
    void updateParticles();
    void removeParticle(int i);
    void flushMap();

    bool checkSettled(int x, int y);

    const uint16_t width = XRES;
    const uint16_t height = YRES;

    Powder particles[XRES*YRES];
    short particleMap[XRES][YRES];

    std::vector<Powder> insertQueue;

    Vec2f gravity;

private:
    Brush *brush;
};

#endif // !WORLD_H
