
#include "world.h"
#include "particle.h"
#include "brush.h"
#include "simConfig.h"
#include "particleTypeDefs.h"
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <iostream>
#include <ostream>
#include <sstream>
#include <sys/types.h>
#include <vector>
#include <fstream>
#include <array>

#include "json.hpp"
using json = nlohmann::json;

World::World() {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            particleMap[x][y] = 0;
        }
    }

    loadTypes();

}

void World::loadTypes() {

    // TODO: Compile json into executable and reference it from there. 
    // Might be easiest to just hold it in a .h file as a string
    std::ifstream typeFile("../res/particleTypes.json", std::ifstream::binary);
    json ptypes = json::parse(typeFile);

    for(json type : ptypes) {
        ParticleType pt;

        pt.id = type["id"];
        pt.name = type["name"];

        // Load color data from hex string
        std::string hexColor = type["color"];
        std::istringstream converter(hexColor);
        unsigned int color;
        converter >> std::hex >> color;
        pt.setColor(color);

        // Set general properties of pt first
        json properties = type["properties"];
        uint16_t propInt = 0;

        // Use if/else if for states of matter because each particle can only be one
        if(properties["powder"] == true) propInt += TYPE_POW;
        else if(properties["liquid"] == true)  propInt += TYPE_LIQUID;
        else if(properties["solid"] == true) propInt += TYPE_SOLID;

        pt.properties = propInt;

        // Set specific properties of pt
        pt.mass = type["mass"];

        // Insert into types array
        types[pt.id] = pt;
    }

    for(int i = 0; i < 32; i ++) {
        ParticleType pt = types[i];

        if(pt.name != "")
        std::cout << i << " : " << pt.name << ": \t" << std::bitset<16>(pt.properties) << std::endl;
    }
}

void World::createWalls(int thickness) {
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            if(x <= thickness || x >= width-thickness || y <= thickness || y >= height-thickness) {
                Particle p(Vec2i(x, y));
                p.type = 1;

                insert(&p);
            }
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
    short type = p->type;
    uint16_t props = (uint32_t)types[(int)type].properties;
    particleMap[pos->x][pos->y] = 0;

    // Check for particle directly underneath
    bool particleBelow = false;
    if(pos->y < height-1) {
        particleBelow = (bool) particleMap[pos->x][pos->y+1];
    }

    // Basic movement calculations
    if(props & TYPE_POW || props & TYPE_LIQUID) {
        if(!particleBelow) {
            // TODO: Add actual physics calculations with acceleration and velocity
            //int nx = pos->x + p->vel.x;
            //int ny = pos->y + p->vel.y;
            int nx = pos->x;
            int ny = pos->y + 1;

            pos->x = nx;
            pos->y = ny;
        }
    }

    // Vertical stacking calculations
    // TODO: Add support for stacking in other directions, not based on gravity
    if(props & TYPE_POW && particleBelow && isSettled(pos->x, pos->y+1)) {
        if(!particleMap[pos->x+1][pos->y+1]) {
                pos->x++;
                pos->y++;
        } else if(!particleMap[pos->x-1][pos->y+1]) {
                pos->x--;
                pos->y++;
        }
        
    }

    //TODO: Make liquid physics a little bit better, they're kinda wack right now
    if(props & TYPE_LIQUID && particleBelow) {
        // Move along velocity vector until hitting another particle, then bounce
            if(!particleMap[pos->x+1][pos->y]) {
                pos->x++;
            } else if(!particleMap[pos->x-1][pos->y]) {
                pos->x--;
            }

            if(!particleMap[pos->x][pos->y+1]) {
                pos->y++;
            }
    }

    if(pos->x > width-1 || pos->x < 0 || pos->y > height-1 || pos->y < 0) {
        p->type = 0;
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
    return(isSettled(x, y, 0));
}

bool World::isSettled(int x, int y, uint16_t props) {

    //TODO: implement a settleMap for optimization
    if((uint32_t)types[particleMap[x][y]].properties & TYPE_SOLID) {
        return true;
    }

    if(y == height-1) return true;
    int py = y+1;

    while(py < height-1) {
       if(!particleMap[x][py]) {
           return false;
       }

       if((uint32_t)types[particleMap[x][py]].properties & TYPE_SOLID) {
           return true;
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

