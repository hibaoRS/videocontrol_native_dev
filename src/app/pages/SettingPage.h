//
// Created by 10324 on 2019/7/15.
//

#ifndef LVGL_SETTINGPAGE_H
#define LVGL_SETTINGPAGE_H


#include <lvgl/src/lv_core/lv_obj.h>
#include <src/app/BasePage.h>

class SettingPage : public BasePage {

public :
    explicit SettingPage(lv_obj_t *parent) : BasePage(parent) {
        init_layout();
    }

private:
    void init_layout() override;;

};


#endif //LVGL_SETTINGPAGE_H
