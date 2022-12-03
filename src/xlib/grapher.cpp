#include "grapher.hpp"

/*
 * The Grapher class uses LVGL to display data graphs (up to 2 datasets 
 * simultaneously) on the brain's screen. This is primarily used to monitor the
 * flywheel's velocity and ensure it is operating as expected
 */

namespace xlib {
    //The VEX Brain's screen is 480 pixels wide, 240 pixels tall
    #define LVGL_SCREEN_WIDTH 480
    #define LVGL_SCREEN_HEIGHT 240
    
    //Initialize all necessary LVGL objects
    void Grapher::initGraph() {
        chart = lv_chart_create(lv_scr_act(), NULL);
        lv_obj_set_size(chart, 480, 240);
        lv_obj_align(chart, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
        lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
        lv_chart_set_range(chart, 0, 3600);
        lv_chart_set_point_count(chart, 250);
        //lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);

        ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
        ser2 = lv_chart_add_series(chart, LV_COLOR_GREEN);
    }
    
    //Clear all data currently on the graph
    void Grapher::clearGraph() {
        lv_chart_set_points(chart, ser1, NULL);
        lv_chart_set_points(chart, ser2, NULL);
    }

    //Add a new data point to the graph (pushing back old data points)
    void Grapher::newData(double data, int series) {
        if(series == 0)
            lv_chart_set_next(chart, ser1, data);
        else if(series == 1)
            lv_chart_set_next(chart, ser2, data);
    
        lv_chart_refresh(chart);
    }
}