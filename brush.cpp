#include <iostream>
#include <cmath>
#include "brush.h"
#include "world.h"
#include "powder.h"
#include "vec2.h"

Vec2i Brush::pos = Vec2i(0,0);
bool Brush::enabled = false;
float Brush::size = 1;

Brush::Brush() {
    type = 1;
}

void Brush::setPos(double x, double y) {
    setPos((int)x, (int)y);
}

void Brush::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;
}

void Brush::setEnabled(int state) {
    enabled = (bool)state;
}

void Brush::setScaleFactor(short scale) {
    scaleFactor = scale;
}

void Brush::changeSize(float amount) {
    size += amount;

    if(size < 1) {
        size = 1;
    }
}

Vec2i Brush::getPos() {
    return pos;
}

int Brush::getSize() {
    return size;
}

short Brush::getScaleFactor() {
    return scaleFactor;
}

void Brush::update() {
    if(enabled) {

        int r = round(size/2);

        for(int x = -r; x < r; x++) {
            for(int y = -r; y < r; y++) {

                if(x*x + y*y >= r * r) continue;

                int mx = pos.x*2/scaleFactor;
                int my = pos.y*2/scaleFactor;

                Vec2i position = Vec2i(mx+x, my+y);

                Powder *p = new Powder(position);
                p->type = type;
                world->insert(p);  
            }
        }

    }
}
