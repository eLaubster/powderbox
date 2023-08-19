#ifndef VEC2_H
#define VEC2_H

struct Vec2i {
    int y,x;

    Vec2i() {
        x = 0;
        y = 0;
    }
    
    Vec2i(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

struct Vec2f {
    float x,y;

    Vec2f() {
        x = 0;
        y = 0;
    }

    Vec2f(float x, float y) {
        this->x = x;
        this->y = y;
    }
};

#endif // !VEC2_H
