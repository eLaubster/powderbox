
#include "particle.h"
#include "world.h"
#include "vec2.h"
#include <_types/_uint32_t.h>
#include <cmath>
#include <iostream>

Particle::Particle() {
    Particle(Vec2i(0,0));
}

Particle::Particle(Vec2i pos) {
    this->pos = pos;
    type = 0;
}

Particle::Particle(const Particle &p) {
    pos = p.pos;
    vel = p.vel;
    acc = p.acc;
    type = p.type;
}
