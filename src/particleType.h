

#include <_types/_uint32_t.h>
#include <string>
#include <iostream>

/*class color3f {
    public:
        float r, g, b;

        color3f() {
            r = 0;
            g = 0;
            b = 0;
        }

        color3f(float r, float g, float b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }
};*/

struct ParticleType {

    public:

        int id;
        std::string name;
        float r, g, b;

        uint16_t properties;

        int mass;

        void setColor(uint32_t color) {
            uint32_t ib = color & 0xFF;
            uint32_t ig = color & 0xFF00;
            uint32_t ir = color & 0xFF0000;

            r = (ir >> 16) / 255.0;
            g = (ig >> 8) / 255.0;
            b = ib / 255.0;
        }

};
