#include <iostream>

#include "vec2.h"
#include "world.h"
#include "brush.h"
#include "worldRenderer.h"
#include "callbackManager.h"
#include <cmath>

#include <GLFW/glfw3.h>
int main (int argc, char *argv[]) {
    
    World world;
    world.setGravity(0, 0.005);

    Brush b = Brush();
    b.world = &world;

    world.setBrush(&b);

    WorldRenderer renderer(&world);
    renderer.setScaleFactor(3);
    renderer.init();

    CallbackManager callbackManager = CallbackManager(renderer.window);
    callbackManager.init();
    
    while(!renderer.close) {
        double currentTime = glfwGetTime();

        world.update();

        renderer.draw();
        renderer.update();
    }
 
    renderer.exit();
    return 0;
}
