#pragma once
#include <map>
#include <string>
#include "display/lvgl.h"

namespace xlib {
    void None();

    class Selector {
        lv_obj_t * createBtn(lv_obj_t * parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, const char * title, bool isHidden);

        //Declares function pointers to hold the call to auton functions
        void (*grabL) (){&None}, (*grabR) (){&None}, (*winPointL) (){&None},
                        (*winPointR) (){&None}, (*fullL) (){&None},
                        (*fullR) (){&None}, (*rTall) (){&None}, (*dGrab) (){&None},
                        (*wP) (){&None}, (*skills_t) (){&None}, (*none) (){&None};
        


        //Left
        lv_obj_t * leftMain = createBtn(lv_scr_act(), 10, 5, 150, 40, "Left full", false);
        lv_obj_t * leftGrab = createBtn(lv_scr_act(), 10, 50, 150, 40, "Left grab", false);
        lv_obj_t * leftWinPoint = createBtn(lv_scr_act(), 10, 95, 150, 40, "Left WP", false);
        lv_obj_t * leftTall = createBtn(lv_scr_act(), 10, 140, 150, 40, "Left tall", false);
        lv_obj_t * emptyL = createBtn(lv_scr_act(), 10, 185, 150, 40, "Empty", false);
        //Misc
        lv_obj_t * fullWP = createBtn(lv_scr_act(), 165, 5, 150, 40, "Full WP", false);
        lv_obj_t * skills = createBtn(lv_scr_act(), 165, 50, 150, 40, "Skills", false);
        lv_obj_t * emptyM1 = createBtn(lv_scr_act(), 165, 95, 150, 40, "Empty", false);
        lv_obj_t * emptyM2 = createBtn(lv_scr_act(), 165, 140, 150, 40, "Empty", false);
        lv_obj_t * emptyM3 = createBtn(lv_scr_act(), 165, 185, 150, 40, "Empty", false);
        //Right
        lv_obj_t * rightMain = createBtn(lv_scr_act(), 320, 5, 150, 40, "Right full", false);
        lv_obj_t * rightGrab = createBtn(lv_scr_act(), 320, 50, 150, 40, "Right grab", false);
        lv_obj_t * rightWinPoint = createBtn(lv_scr_act(), 320, 95, 150, 40, "Right WP", false);
        lv_obj_t * rightTall = createBtn(lv_scr_act(), 320, 140, 150, 40, "Right tall", false);
        lv_obj_t * doubleGrab = createBtn(lv_scr_act(), 320, 185, 150, 40, "D Grab", false);
        //Array
        lv_obj_t * main_page[15] {leftMain, leftGrab, leftWinPoint, leftTall, emptyL, fullWP,
                            skills, emptyM1, emptyM2, emptyM3, rightMain, rightGrab, rightWinPoint, 
                            rightTall, doubleGrab};

        //Selection page
        lv_obj_t * backButton = createBtn(lv_scr_act(), 0, 0, 75, 40, "Back", true);
        lv_obj_t * autonName = lv_label_create(lv_scr_act(), NULL);
        //Page 2 array
        lv_obj_t * select_page[2] {backButton, autonName};

        std::map<lv_obj_t*, void(*)()> map { {leftMain, fullL}, {leftGrab, grabL}, {leftWinPoint, winPointL},
                                    {leftTall, none}, {fullWP, wP}, {skills, skills_t},
                                    {rightMain, fullR}, {rightGrab, grabR}, {rightWinPoint, winPointR},
                                    {rightTall, rTall}, {doubleGrab, dGrab} };
        
        
        int auton = 0;


        void (*autonToRun) (){};

        public:
        void AutonNum(lv_obj_t * btnTemp) {
            autonToRun = map[btnTemp];

            lv_label_set_text(autonName, lv_label_get_text(lv_obj_get_child(btnTemp, NULL)));

            
            for(auto &b : main_page)
                lv_obj_set_hidden(b, true);

            for(auto &b : select_page)
                lv_obj_set_hidden(b, false);
        }

        void (*getSelection()) () {
            return autonToRun;
        }

        void BackButton(lv_obj_t * btnTemp) {
            for(auto &b : main_page)
                lv_obj_set_hidden(b, false);

            for(auto &b : select_page)
                lv_obj_set_hidden(b, true);

            autonToRun = none;
        }

        void init();

        void setActive(bool iState);
    };

    extern Selector selector;
}