#pragma once
#include "display/lvgl.h"
#include <vector>

namespace xlib {
    class Grapher {
        lv_obj_t * chart;
        lv_chart_series_t * ser1;
        lv_chart_series_t * ser2;
    public:
        void initGraph(std::vector<int> range, int pointCount);
        void clearGraph();
        void newData(double data, int series);
    };
}

