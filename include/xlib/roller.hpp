#pragma once
#include "globals.hpp"

namespace xlib {
    class Roller {
        int flipDegrees = 200;

    public:
        void flip();
    };
    extern Roller rollerMech;
}