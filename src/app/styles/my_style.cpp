//
// Created by 10324 on 2019/7/13.
//

#include <lvgl/src/lv_misc/lv_color.h>
#include <lvgl/src/lv_core/lv_style.h>
#include "my_style.h"

lv_style_t my_style_transp_with_border;
lv_style_t my_style_transp_with_border2;
lv_style_t my_style_transp_with_border3;
lv_style_t my_style_transp_with_border4;

void init_my_styles() {
    auto dpi = LV_DPI / 50;
    lv_style_copy(&my_style_transp_with_border, &lv_style_transp_tight);
    my_style_transp_with_border.body.border.color = LV_COLOR_BLUE;
    my_style_transp_with_border.body.border.width = dpi >= 1 ? dpi : 1;
    my_style_transp_with_border.body.border.opa = LV_OPA_50;


    lv_style_copy(&my_style_transp_with_border2, &my_style_transp_with_border);
    my_style_transp_with_border2.body.border.color = LV_COLOR_RED;

    lv_style_copy(&my_style_transp_with_border3, &my_style_transp_with_border);
    my_style_transp_with_border3.body.border.color = LV_COLOR_AQUA;

    lv_style_copy(&my_style_transp_with_border4, &my_style_transp_with_border);
    my_style_transp_with_border3.body.border.color = LV_COLOR_WHITE;

}
