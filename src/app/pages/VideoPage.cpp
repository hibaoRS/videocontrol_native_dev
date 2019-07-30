//
// Created by 10324 on 2019/7/6.
//

#include <lvgl/src/lv_core/lv_style.h>
#include "VideoPage.h"

void VideoPage::init_layout() {
    auto *obj = lv_arc_create(layout, NULL);
    lv_obj_set_style(obj, &lv_style_scr);
}
