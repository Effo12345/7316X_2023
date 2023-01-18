#include "button.hpp"

namespace xlib {

    Button::Button(lv_obj_t * parent, int column, int row, const char * title, bool isHidden) {
        lv_obj_t * btnTmp = lv_btn_create(parent, NULL);
        lv_obj_set_pos(btnTmp, (column * 155) + 10, (row * 45) + 5);
        lv_obj_set_size(btnTmp, 150, 40);
        lv_obj_set_hidden(btnTmp, isHidden);

        lv_obj_t * label = lv_label_create(btnTmp, NULL);
        lv_label_set_text(label, title);
        lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

        button = btnTmp;
    }

    Button::Button(lv_obj_t * parent, int horiz, int vert, int width, int height, const char * title, bool isHidden) {
        lv_obj_t * btnTmp = lv_btn_create(parent, NULL);
        lv_obj_set_pos(btnTmp, horiz, vert);
        lv_obj_set_size(btnTmp, width, height);
        lv_obj_set_hidden(btnTmp, isHidden);

        lv_obj_t * label = lv_label_create(btnTmp, NULL);
        lv_label_set_text(label, title);
        lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

        button = btnTmp;
    }

}