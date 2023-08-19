#ifndef POWDER_H
#define POWDER_H

#include "vec2.h"
#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>

class World;

struct Powder {
    // Position, Velocity, and Acceleration Vectors
    Vec2i pos;
    Vec2f vel;
    Vec2f acc;
    uint8_t type;

    Powder();
    Powder(Vec2i pos);
    Powder(const Powder &p);

};


#endif // !POWDER_H
