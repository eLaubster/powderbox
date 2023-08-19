#ifndef CALLBACK_MANAGER_H
#define CALLBACK_MANAGER_H

#include "brush.h"
#include <GLFW/glfw3.h>

class CallbackManager {
    public:
        GLFWwindow *window;

        CallbackManager(GLFWwindow *window);

        void init();
        void setBrush(Brush *brush);

        static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
        static void cursorPosCallback(GLFWwindow *window, double posx, double posy);
        static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
        static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

    private:
        static Brush *brush;
        static double mouseX;
        static double mouseY;
        static int *buttons;
};

#endif // !CALLBACK_MANAGER_H
