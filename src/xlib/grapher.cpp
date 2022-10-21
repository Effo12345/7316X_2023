#include "grapher.hpp"

namespace xlib {
    #define LVGL_SCREEN_WIDTH 480
    #define LVGL_SCREEN_HEIGHT 240
    
    void Grapher::initGraph() {
        chart = lv_chart_create(lv_scr_act(), NULL);
        lv_obj_set_size(chart, 480, 240);
        lv_obj_align(chart, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
        lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
        lv_chart_set_range(chart, 0, 600);
        lv_chart_set_point_count(chart, 250);
        //lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);

        ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
        ser2 = lv_chart_add_series(chart, LV_COLOR_GREEN);
    }
    
    void Grapher::clearGraph() {
        lv_chart_set_points(chart, ser1, NULL);
        lv_chart_set_points(chart, ser2, NULL);
    }

    void Grapher::newData(double data, int series) {
        if(series == 0)
            lv_chart_set_next(chart, ser1, data);
        else if(series == 1)
            lv_chart_set_next(chart, ser2, data);
    
        lv_chart_refresh(chart);
    }

    Grapher grapher;
}


/*
void chartTestInit() {
    lv_obj_t * chart;
    chart = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_size(chart, 480, 240);
    lv_obj_align(chart, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    //lv_chart_set_range(chart, 0, 250);
    lv_chart_set_range(chart, 0, 100);
    //lv_chart_set_point_count(chart, 100);
    //lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);

    lv_chart_series_t * ser1 = lv_chart_add_series(chart, LV_COLOR_RED);
    lv_chart_series_t * ser2 = lv_chart_add_series(chart, LV_COLOR_GREEN);

    //Dummy data for testing purposes

    //Set the next points on 'ser1'
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 10);
    lv_chart_set_next(chart, ser1, 30);
    lv_chart_set_next(chart, ser1, 70);
    lv_chart_set_next(chart, ser1, 90);

    //Directly set points on 'ser2'
    ser2->points[0] = 90;
    ser2->points[1] = 70;
    ser2->points[2] = 65;
    ser2->points[3] = 65;
    ser2->points[4] = 65;
    ser2->points[5] = 65;
    ser2->points[6] = 65;
    ser2->points[7] = 65;
    ser2->points[8] = 65;
    ser2->points[9] = 65;
    lv_chart_refresh(chart); //Required after direct set
}
*/