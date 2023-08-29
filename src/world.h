#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <map>
#include <vector>
#include "particle.h"
#include "particleType.h"
#include "simConfig.h"

class Brush;

class World {
public:
    World();

    void loadTypes(); 

    void setGravity(float x, float y);
    void setBrush(Brush *brush);

    void createWalls(int thickness);

    Brush* getBrush();

    void insert(Particle *p);
    void flushInsertQueue();
    void update();
    void updateParticles();
    void updateParticle(Particle *p);
    void removeParticle(int i);
    void flushMap();

    bool isSettled(int x, int y, uint16_t props);
    bool isSettled(int x, int y);

    const uint16_t width = XRES;
    const uint16_t height = YRES;

    // Just allocate space for 32 types for now
    std::array<ParticleType, 32> types;

    Particle particles[XRES*YRES];
    short particleMap[XRES][YRES];
    bool updateMap[XRES][YRES];

    std::vector<Particle> insertQueue;

    Vec2f gravity;

private:
    Brush *brush;
};

#endif // !WORLD_H
