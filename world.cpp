
#include "world.h"
#include "powder.h"
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

void World::insert(Powder *p) {

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

        particles[i] = Powder(insertQueue[0]);
        int x = insertQueue[0].pos.x;
        int y = insertQueue[0].pos.y;
        particleMap[x][y] = insertQueue[0].type;
        insertQueue.erase(insertQueue.begin());
        toInsert--;

        i++;
    }

    std::vector<Powder> p;
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
        if(!particles[i].type) return;

        Powder *p = &particles[i];

        p->acc.x += gravity.x;
        p->acc.y += gravity.y;

        p->vel.x += p->acc.x;
        p->vel.y += p->acc.y;

        Vec2i *pos = &p->pos;

        particleMap[pos->x][pos->y] = 0;

        bool particleBelow = (bool) particleMap[pos->x][pos->y+1];
        
        // For now... add proper calcuations with velocity and acceleration later
        if(!particleBelow) {
            if(pos->y < height-1 ) {
                pos->y++;
            }
        } else if (pos->y+1 < height){

            bool leftOpen = !particleMap[pos->x-1][pos->y+1];
            bool rightOpen = !particleMap[pos->x+1][pos->y+1];

            if(leftOpen || rightOpen && pos->y < height-1) {
                if(checkSettled(pos->x, pos->y)) {
                    if(leftOpen && pos->x > 0) {
                        pos->x--;
                        pos->y++;
                    } else if (pos->x && pos->x < width-1){
                        pos->x++;
                        pos->y++;
                    }
                }
            }
        }

        particleMap[pos->x][pos->y] = particles[i].type;
        
    }

}

void World::flushMap() {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            particleMap[x][y] = 0;
        }
    }

    for(int i = 0; i < width*height; i++) {

        int type = particles[i].type;

        Vec2i pos = particles[i].pos;
        particleMap[pos.x][pos.y] = type;
    }
}

bool World::checkSettled(int x, int y) {

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

void World::DiscountDoubleCheck() {
    int cellCount = 0;
    int pCount = 0;

    for(int i = 0; i < width*height; i++) {
        if(particles[i].type) pCount++;
    }

    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            if(particleMap[x][y]) {
                cellCount++;
            }
        }
    }

    std::cout << "pCount: " << pCount << ", CellCount: " << cellCount << std::endl;
}
