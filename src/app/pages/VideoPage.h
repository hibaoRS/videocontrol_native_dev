//
// Created by 10324 on 2019/7/6.
//

#ifndef LVGL_VIDEOPAGE_H
#define LVGL_VIDEOPAGE_H


#include <src/app/BasePage.h>

class VideoPage : public BasePage {
public :
    explicit VideoPage(lv_obj_t *parent) : BasePage(parent) {
        init_layout();
    }

protected:
    void init_layout() override;;


};


#endif //LVGL_VIDEOPAGE_H
