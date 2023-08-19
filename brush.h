#ifndef BRUSH_H
#define BRUSH_H

#include "world.h"
#include <sys/wait.h>

class Brush {
    public:
        Brush();

        void setPos(double x, double y);
        void setPos(int x, int y);
        void setEnabled(int state);
        void setScaleFactor(short scale);

        void changeSize(float amount);

        Vec2i getPos();
        int getSize();
        short getScaleFactor();

        void update();

        World* world;
    private:
        static Vec2i pos;
        static bool enabled;
        static float size;
        int scaleFactor;
        short type;
        

};

#endif // !BRUSH_H
