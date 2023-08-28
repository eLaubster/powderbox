
#include "worldRenderer.h"
#include "brush.h"
#include "particle.h"

#include <any>
#include <chrono>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>

int *WorldRenderer::buttons = new int[2];
double WorldRenderer::mouseX = 0;
double WorldRenderer::mouseY = 0;

Brush* WorldRenderer::b = nullptr;

WorldRenderer::WorldRenderer(World *w) {
    world = w;
    width = world->width;
    height = world->height;
}

void WorldRenderer::setScaleFactor(short scale) {
    this->scaleFactor = scale;
    world->getBrush()->setScaleFactor(scale);
}

short WorldRenderer::getScaleFactor() {
    return scaleFactor;
}

int WorldRenderer::init() {

    if(!glfwInit()) {
        std::cerr << "Unable to initialize GLFW" << std::endl;
        return 0;
    } else {
        std::cout << "GLFW Initialized - Version: " << glfwGetVersionString() << std::endl;
    }

    prevTime = glfwGetTime();

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Find solution to check if display is retina or not
    window = glfwCreateWindow(width*scaleFactor/2, height*scaleFactor/2, "powderbox", NULL, NULL);

    if(window == NULL) {
        std::cerr << "Unable to create window with GLFW" << std::endl;
        return 0; 
    }

    glfwMakeContextCurrent(window);
    close = glfwWindowShouldClose(window);
   
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    int glewErr = glewInit();

    if(glewErr != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW" << std::endl;
    }

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);

    glMatrixMode(GL_PROJECTION);

    glOrtho(0, w, h, 0, -1, 1);
    glViewport(0, 0, w, h);
    return 1; 
}

void WorldRenderer::draw() {

    glClearColor(0.08627450980392157, 0.08627450980392157, 0.11372549019607843, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS);
   
    glColor4f((float)241/255,(float)216/255,(float)181/255,1);
    int hw = width / 2;
    int hh = height / 2;

    for(int x = 0; x < world->width; x++) {
        for(int y = 0; y < world->height; y++) {
           
            if(world->particleMap[x][y] != 0) {
                int mx = x * scaleFactor;
                int my = y * scaleFactor;

                for(int px = 0; px < scaleFactor; px++) {
                    for(int py = 0; py < scaleFactor; py++) {
                        glVertex2i(mx+px, my+py);
                    }
                }
            }

        }
    }

    glEnd();

    Brush *b = world->getBrush();

    drawCircle(b->getPos().x*2, b->getPos().y*2, b->getSize()*b->getScaleFactor()/2);

    const short fpsUpdateRate = 30;

    frameCount++;
    if(frameCount % fpsUpdateRate == 0) {
        double currentTime = glfwGetTime();
        fps = fpsUpdateRate / (currentTime - prevTime);
        prevTime = currentTime;
        displayFps();
    }

    //std::cout <<  "Frametime: " << duration.count() << std::endl;
}

void WorldRenderer::drawCircle(int x, int y, int r) {
    glBegin(GL_LINE_LOOP);
    glColor3f(1, 1, 1);

    int segCount = 32;

    for(int i = 0; i < segCount; i++) {

         float theta = 2.0f * 3.1415926f * float(i) / float(segCount);
         float ox = r * cosf(theta);
         float oy = r * sinf(theta);
    
        glVertex2i(x+ox, y+oy);
    }

    glEnd();
}

void WorldRenderer::update() {
   glfwSwapBuffers(window);
   glfwPollEvents();

   close = glfwWindowShouldClose(window);
}

void WorldRenderer::exit() {
    glfwTerminate();
}

short WorldRenderer::getFps() {
    return fps;
}

void WorldRenderer::displayFps() {
    std::stringstream ss;

    ss << "powderBox - FPS: " << fps;
    glfwSetWindowTitle(window, ss.str().c_str());
}

