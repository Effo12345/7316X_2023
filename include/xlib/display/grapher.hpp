#pragma once
#include "display/lvgl.h"
#include <vector>
#include <array>

namespace xlib {
    class Grapher {
        lv_obj_t * chart;
        lv_chart_series_t * ser1;
        lv_chart_series_t * ser2;
        lv_chart_series_t * ser3;
        lv_chart_series_t * ser4;
        std::array<lv_chart_series_t*, 4> serArr;
    public:
        void initGraph(std::vector<int> range, int pointCount);
        void clearGraph();
        void newData(double data, int series);
    };
}

