
#include "world.h"
#include "particle.h"
#include "brush.h"
#include "simConfig.h"
#include <iostream>
#include <ostream>
#include <sys/types.h>
#include <vector>

World::World() {

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            particleMap[x][y] = 0;
        }
    }
}

void World::insert(Particle *p) {

    if(p->pos.x > width-1 || p->pos.x < 0) return;
    if(p->pos.y > height-1 || p->pos.y < 0) return;

    int x = p->pos.x;
    int y = p->pos.y;

    if(particleMap[x][y]) return; 

    int index = 0;

    insertQueue.push_back(*p);
}

void World::flushInsertQueue() {
    int i = 0;

    int toInsert = insertQueue.size();

    while(toInsert > 0) {
        if(particles[i].type) {
            i++;
            continue;
        }

        particles[i] = Particle(insertQueue[0]);
        int x = insertQueue[0].pos.x;
        int y = insertQueue[0].pos.y;
        particleMap[x][y] = insertQueue[0].type;
        insertQueue.erase(insertQueue.begin());
        toInsert--;

        i++;
    }

    std::vector<Particle> p;
    insertQueue = p;
}

void World::update() {
    brush->update();
    flushInsertQueue();

    updateParticles();
    flushMap();
}

void World::updateParticles() {
    for(int i = 0; i < width*height; i++) {
        updateParticle(&particles[i]);       
    }
}

void World::updateParticle(Particle *p) {
    if(updateMap[p->pos.x][p->pos.y]) return; 
    if(!p->type) return;

    Vec2i *pos = &p->pos;

    particleMap[pos->x][pos->y] = 0;

    // Check for particle directly underneath
    bool particleBelow = false;
    if(pos->y < height-1) {
        particleBelow = (bool) particleMap[pos->x][pos->y+1];
    }

    // Basic movement calculations
    if(pos->y < height-1 && !particleBelow) {
        // TODO: Add actual physics calculations with acceleration and velocity
        //int nx = pos->x + p->vel.x;
        //int ny = pos->y + p->vel.y;
        int nx = pos->x;
        int ny = pos->y + 1;

        if(ny > height-1) {
            ny = height-1;
        }

        pos->x = nx;
        pos->y = ny;
    }

    // Vertical stacking calculations
    // TODO: Add support for stacking in other directions, not based on gravity
    if(particleBelow && isSettled(pos->x, pos->y+1)) {
        if(!particleMap[pos->x+1][pos->y+1]) {
            if(pos->x < width-1) 
                pos->x++;
        } else if(!particleMap[pos->x-1][pos->y+1]) {
            if(pos->x > 0) 
                pos->x--;
        }
    }

    // Update particleMap and updateMap to include new position
    particleMap[pos->x][pos->y] = p->type;
    updateMap[pos->x][pos->y] = 1; 
}

void World::flushMap() {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            particleMap[x][y] = 0;
            updateMap[x][y] = 0;
        }
    }

    for(int i = 0; i < width*height; i++) {

        int type = particles[i].type;

        Vec2i pos = particles[i].pos;
        particleMap[pos.x][pos.y] = type;
    }
}

bool World::isSettled(int x, int y) {

    //TODO: implement a settleMap for optimization

    if(y == height-1) return true;
    int py = y+1;

    while(py < height-1) {
       if(!particleMap[x][py]) {
           return false;
       }
       py++;
    }
    
    return true;
}

void World::setGravity(float x, float y) {
    gravity.x = x;
    gravity.y = y;
}

void World::setBrush(Brush *brush) {
    this->brush = brush;
}

Brush *World::getBrush() {
    return brush;
}

