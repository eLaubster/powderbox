#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <map>
#include <vector>
#include "particle.h"
#include "simConfig.h"

class Brush;

class World {
public:
    World();

    void setGravity(float x, float y);
    void setBrush(Brush *brush);

    Brush* getBrush();

    void insert(Particle *p);
    void flushInsertQueue();
    void update();
    void updateParticles();
    void updateParticle(Particle *p);
    void removeParticle(int i);
    void flushMap();

    bool isSettled(int x, int y);

    const uint16_t width = XRES;
    const uint16_t height = YRES;

    Particle particles[XRES*YRES];
    short particleMap[XRES][YRES];
    bool updateMap[XRES][YRES];

    std::vector<Particle> insertQueue;

    Vec2f gravity;

private:
    Brush *brush;
};

#endif // !WORLD_H
