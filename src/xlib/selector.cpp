#include "xlib/selector.hpp"

/*
 * The Selector class uses LVGL to create an auton selector of digital buttons
 * on the brain's screen. Multiple autons can be stored in the same program and
 * run as desired.
 */

namespace xlib {
    //Dummy function called when no auton is selected
    void None() {};

    //Generalized function to initialize all values of an LVGL button and return
    //the completed object
    lv_obj_t * Selector::createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, 
    lv_coord_t width, lv_coord_t height, const char * title, bool isHidden){
        //title = std::to_string(y).c_str();
        lv_obj_t * btnTemp = lv_btn_create(parent, NULL);
        //lv_obj_align(btn, NULL, align, offsetX, offsetY);
        lv_obj_set_pos(btnTemp, x, y);
        lv_obj_set_size(btnTemp, width, height);
        lv_obj_set_hidden(btnTemp, isHidden);

        lv_obj_t * label = lv_label_create(btnTemp, NULL);
        lv_label_set_text(label, title);
        lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
        return btnTemp;
    }

    Selector selector;

    //Called when a button on the auton selector is pressed. Declared outside
    //the Selector class so they can return a static variable, as is required by
    //LVGL
    static lv_res_t AutonNumber(lv_obj_t * btnTemp) {
        selector.AutonNum(btnTemp);
        return LV_RES_OK;
    }

    //Called when the back button is pressed. Returns to the main screen of the
    //auton selector.
    static lv_res_t Back(lv_obj_t * btnTemp) {
        selector.BackButton(btnTemp);
        return LV_RES_OK;
    }

    //Initialize LVGL objects and set the background theme
    void Selector::init() {
            autonToRun = none;

            lv_obj_align(autonName, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
            lv_obj_set_hidden(autonName, true);

            //Sets theme to night
            lv_theme_t * th = lv_theme_night_init(65, NULL);
            lv_theme_set_current(th);

            //Initializes state of disabled button
            lv_btn_set_state(emptyL, LV_BTN_STATE_INA);
            lv_btn_set_state(emptyM1, LV_BTN_STATE_INA);
            lv_btn_set_state(emptyM2, LV_BTN_STATE_INA);
            lv_btn_set_state(emptyM3, LV_BTN_STATE_INA);
            
            for(auto &b : main_page)
                lv_btn_set_action(b, LV_BTN_ACTION_CLICK, 
                    AutonNumber);

            lv_btn_set_action(backButton, LV_BTN_ACTION_CLICK, 
                Back);
        }

    //Set the auton selector to be visible based on the value of iState
    void Selector::setActive(bool iState) {
        for(auto &b : main_page)
            lv_obj_set_hidden(b, !iState);

        for(auto &b : select_page)
            lv_obj_set_hidden(b, !iState);
    }
}