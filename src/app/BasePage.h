//
// Created by 10324 on 2019/7/6.
//

#ifndef LVGL_BASEPAGE_H
#define LVGL_BASEPAGE_H


#include <lvgl/lvgl.h>
#include <src/app/styles/my_style.h>

class BasePage {
public :
    explicit BasePage(lv_obj_t *parent) : layout(lv_cont_create(parent, NULL)) {
        lv_obj_set_style(parent, &my_style_transp_with_border);
        lv_page_set_style(parent, LV_PAGE_STYLE_SCRL, &lv_style_transp_tight);

        lv_cont_set_layout(layout, LV_LAYOUT_COL_L);
        lv_cont_set_fit2(layout, LV_FIT_FLOOD, LV_FIT_NONE);
        lv_obj_set_height(layout, lv_obj_get_height(parent) - 70);
        lv_obj_set_style(layout, &lv_style_transp_tight);

    };


protected:
    lv_obj_t *layout;

    virtual void init_layout() = 0;
};


#endif //LVGL_BASEPAGE_H
