#pragma once
#include "display/lvgl.h"

namespace xlib {
    class Grapher {
        lv_obj_t * chart;
        lv_chart_series_t * ser1;
        lv_chart_series_t * ser2;
    public:
        void initGraph();
        void clearGraph();
        void newData(double data, int series);
    };

    extern Grapher grapher;
}

