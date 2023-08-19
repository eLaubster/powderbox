
#include "powder.h"
#include "world.h"
#include "vec2.h"
#include <_types/_uint32_t.h>
#include <cmath>
#include <iostream>

Powder::Powder() {
    Powder(Vec2i(0,0));
}

Powder::Powder(Vec2i pos) {
    this->pos = pos;
    type = 0;
}

Powder::Powder(const Powder &p) {
    pos = p.pos;
    vel = p.vel;
    acc = p.acc;
    type = p.type;
}
