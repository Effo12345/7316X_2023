#include "xlib/display/selector.hpp"

namespace xlib {

    //This pointer should never be used when it is unitialized because it is used by
    //objects declared inside the Selector class but initialized in that class'
    //constructor. It is also not visible in other files because it isn't declared
    //In the header file
    Selector* ptrSelector;

    /**
     * A trampoline function to call member functions of ptrSelector when a
     * selector button is pressed
     *
     * @param btn The selector button that was pressed
     * @return A success flag
     */
    static lv_res_t selectionTrampoline(lv_obj_t* btn) {
        //Just in case, throw an easy to understand error if the pointer is 
        //left unitialized
        if(ptrSelector == nullptr)
            throw std::runtime_error("Selector pointer unitialized");

        ptrSelector->setSelection(btn);

        return LV_RES_OK;
    }

    /**
     * A trampoline function fo call a member function of ptrSelector when the
     * back button is pressed
     *
     * @param btn Pointer to the back button
     * @return A success flag
     */
    static lv_res_t backTrampoline(lv_obj_t* btn) {
        if(ptrSelector == nullptr)
            throw std::runtime_error("Selector pointer unitialized");

        ptrSelector->back();

        return LV_RES_OK;
    }


    //Dummy function to call when no auton is selected
    void Selector::None() {};

    /**
     * Auton selector using LVGL's buttons. Computes buttons' locations. Fills
     * mainPage and buttonMap. Initalizes autonToRun. Throws a runtime error
     * if the selector dimensions are out of bounds
     *
     * @param input The names/functions to be processed
     */
    Selector::Selector(inputData input) {
        //Check if any of the rows or columns are out of bounds
        if(input.size() > 3)
            throw std::runtime_error("Auton selector overflow");
        for(int i = 0; i < input.size(); i++) {
            if(input[i].size() > 5)
                throw std::runtime_error("Auton selector overflow");
        }

        //Initialize selector buttons
        for(int i = 0; i < input.size(); i++) {
            for(int j = 0; j < input[i].size(); j++) {
                mainPage.push_back(Button(lv_scr_act(), i, j, input[i][j].first.c_str(), false));
                int buttonIndex = mainPage.size() - 1;
                buttonMap.insert({mainPage[buttonIndex].getInternalButton(), input[i][j].second});
                
                lv_btn_set_action(mainPage[buttonIndex].getInternalButton(), LV_BTN_ACTION_CLICK, selectionTrampoline);
            }
        }
        
        lv_btn_set_action(backButton.getInternalButton(), LV_BTN_ACTION_CLICK, backTrampoline);

        //Set the desired auton to the dummy None function
        //Non-static member functions must be bound to a this-pointer
        autonToRun = std::bind(&Selector::None, this);

        //Initalize prtSelector
        ptrSelector = this;

        //Set theme and place title in the correct spot
        //Sets theme to night
        lv_theme_t * th = lv_theme_night_init(65, NULL);
        lv_theme_set_current(th);

        //Places autonName in the correct spot
        lv_obj_align(autonName, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
        lv_obj_set_hidden(autonName, true);
    }

    /**
     * A callback function run by the selector buttons when they are pressed.
     * Sets autonToRun based on the buttonMap. Enables backButton and autonName
     * while disabling the selector buttons
     *
     * @param btn Pointer to the button pressed
     */
    void Selector::setSelection(lv_obj_t* btn) {
        autonToRun = buttonMap[btn];

        lv_label_set_text(autonName, lv_label_get_text(lv_obj_get_child(btn, NULL)));

        
        for(Button b : mainPage) {
            b.setHidden(true);
        }

        backButton.setHidden(false);
        lv_obj_set_hidden(autonName, false);
    }

    /**
     * A callback function run by the back button when it is pressed.
     * Enables the selector buttons while disabling backButton and autonName
     */
    void Selector::back() {
        autonToRun = std::bind(&Selector::None, this);

        lv_label_set_text(autonName, "Returned");

        for(Button b : mainPage) {
            b.setHidden(false);
        }

        backButton.setHidden(true);
        lv_obj_set_hidden(autonName, true);
    }

    /**
     * Get the final selection
     */
    std::function<void()> Selector::getSelection() {
        return autonToRun;
    }

    /**
     * Run the selected auton, or None if none were selected
     */
    void Selector::runSelection() {
        autonToRun();
    }

    /**
     * Sets whether the auton selector is visible/interactable
     *
     * @param istate The desired state to set
     */
    void Selector::setActive(bool istate) {
        for(auto& button : mainPage) {
            button.setHidden(!istate);
        }

        backButton.setHidden(!istate);
        lv_obj_set_hidden(autonName, !istate);
    }
}