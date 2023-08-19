#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include "world.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGl/glext.h>
#include "brush.h"

class WorldRenderer {
    public:
        World *world;
        bool close = false;

        GLuint fbo;
        GLuint texture;

        GLFWwindow *window;
        int width;
        int height;

        WorldRenderer(World *w);
        int init();
        void draw();
        void drawCircle(int x, int y, int r);
        void update();
        void exit();
        void displayFps();
        short getFps();

        void setScaleFactor(short scale);
        short getScaleFactor();

        static Brush *b;
    private:
        short fps = 0;
        double prevTime = 0;
        uint32_t frameCount = 0;
        static double mouseX;
        static double mouseY;
        static int *buttons;
        short scaleFactor = 1;
};

#endif // !WORLD_RENDERER_H

