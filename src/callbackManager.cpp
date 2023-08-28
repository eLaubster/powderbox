#include "callbackManager.h"
#include "brush.h"
#include <GLFW/glfw3.h>
#include <iostream>

double CallbackManager::mouseX = 0;
double CallbackManager::mouseY = 0;
int *CallbackManager::buttons = new int[2];

Brush *CallbackManager::brush = nullptr;

CallbackManager::CallbackManager(GLFWwindow *window) {
    this->window = window;
}

void CallbackManager::init() {
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void CallbackManager::setBrush(Brush *brush) {
    this->brush = brush;
}

void CallbackManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    if(key == GLFW_KEY_MINUS && action == 1) {
        brush->changeSize(-1);
    } else if(key == GLFW_KEY_EQUAL && action == 1) {
        brush->changeSize(1);
    }

}

void CallbackManager::cursorPosCallback(GLFWwindow *window, double posx, double posy) {
    mouseX = posx;
    mouseY = posy;

    brush->setPos(posx, posy);
}

void CallbackManager::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if(button > 1) // Only support for mice with only two buttons 
        return; 
    
    buttons[button] = action;

    if(button == 0) {
        brush->setEnabled(action);
    }
}

void CallbackManager::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    brush->changeSize(yoffset);
}
