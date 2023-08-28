#ifndef POWDER_H
#define POWDER_H

#include "vec2.h"
#include <_types/_uint8_t.h>

struct Particle {
    // Position, Velocity, and Acceleration Vectors
    Vec2i pos;
    Vec2f vel;
    Vec2f acc;
    uint8_t type;

    Particle();
    Particle(Vec2i pos);
    Particle(const Particle &p);
};


#endif // !POWDER_H
