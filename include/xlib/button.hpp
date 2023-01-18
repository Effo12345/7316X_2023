#pragma once
#include "display/lvgl.h"

namespace xlib {

    class Button {
        lv_obj_t* button;
    public:
        Button(lv_obj_t * parent, int column, int row, const char * title, bool isHidden);
        Button(lv_obj_t * parent, int horiz, int vert, int width, int height, const char * title, bool isHidden);

        lv_obj_t* getInternalButton() { 
            return button;
        }

        void setHidden(bool istate) {
            lv_obj_set_hidden(button, istate);
        }
    };

}