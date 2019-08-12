//
// Created by 10324 on 2019/7/6.
//

#include <lvgl/src/lv_core/lv_style.h>
#include <cstdio>
#include <res/font/my_symbol_def.h>
#include <lib/restclient-cpp/include/restclient-cpp/restclient.h>
#include <iostream>
#include "RecordPage.h"


void RecordPage::init_layout() {
    init_monitors();
    init_menu();
}


//初始化布局
void RecordPage::init_monitors() {
    auto layoutHeight = lv_obj_get_height(layout);

    monitors = lv_cont_create(layout, NULL);
    lv_cont_set_fit2(monitors, LV_FIT_FLOOD, LV_FIT_NONE);
    lv_obj_set_height(monitors, layoutHeight * (3.0 / 4));
    lv_obj_set_style(monitors, &lv_style_transp_tight);

    init_center_monitors();
    init_left_right_monitors();
}

//初始化菜单栏
void RecordPage::init_menu() {
    auto layoutHeight = lv_obj_get_height(layout);

    menu = lv_cont_create(layout, NULL);
    lv_cont_set_fit2(menu, LV_FIT_FLOOD, LV_FIT_NONE);
    lv_obj_set_height(menu, layoutHeight * (1.0 / 4));
    lv_cont_set_layout(menu, LV_LAYOUT_CENTER);

    auto menu2 = lv_cont_create(menu, NULL);
    lv_cont_set_fit2(menu2, LV_FIT_TIGHT, LV_FIT_FLOOD);
    lv_cont_set_layout(menu2, LV_LAYOUT_ROW_M);
    static lv_style_t my_style;
    lv_style_copy(&my_style, &lv_style_transp_tight);
    my_style.body.padding.inner = 10;
    lv_obj_set_style(menu2, &my_style);


    create_menu(menu2, "录制控制");
    create_menu(menu2, "录制控制");
    create_menu(menu2, "录制控制");
}

//左右布局
void RecordPage::init_left_right_monitors() {
    clear_monitors();
    lv_cont_set_layout(monitors, LV_LAYOUT_ROW_M);

    auto monitorsWidth = lv_obj_get_width(monitors);
    auto monitorsHeight = lv_obj_get_height(monitors);

    auto left = lv_cont_create(monitors, NULL);
    lv_cont_set_fit2(left, LV_FIT_NONE, LV_FIT_FLOOD);
    lv_obj_set_width(left, monitorsWidth / 2);
    lv_obj_set_style(left, &lv_style_transp_tight);

    //TODO 使用map存储各个监视器的坐标信息（value不为空则传过去）
    create_monitor(left, "主播", monitorsHeight);

    auto right = lv_cont_create(monitors, NULL);
    lv_obj_set_style(right, &lv_style_transp_tight);
    lv_cont_set_fit2(right, LV_FIT_NONE, LV_FIT_FLOOD);
    lv_obj_set_width(right, monitorsWidth / 2);
    lv_cont_set_layout(right, LV_LAYOUT_ROW_M);

    auto right_width = lv_obj_get_width(right);

    auto right_left = lv_cont_create(right, NULL);
    lv_cont_set_fit2(right_left, LV_FIT_NONE, LV_FIT_FLOOD);
    lv_obj_set_width(right_left, right_width / 2);

    lv_cont_set_layout(right_left, LV_LAYOUT_COL_M);
    lv_obj_set_style(right_left, &lv_style_transp_tight);

    create_monitor(right_left, "学生特写", monitorsHeight / 3);
    create_monitor(right_left, "教师特写", monitorsHeight / 3);
    create_monitor(right_left, "学生全景", monitorsHeight / 3);


    auto right_right = lv_cont_create(right, NULL);
    lv_obj_set_style(right_right, &lv_style_transp_tight);
    lv_cont_set_fit2(right_right, LV_FIT_NONE, LV_FIT_FLOOD);
    lv_obj_set_width(right_right, right_width / 2);
    lv_cont_set_layout(right_right, LV_LAYOUT_COL_M);


    create_monitor(right_right, "教师全景", monitorsHeight / 3);
    create_monitor(right_right, "板书特写", monitorsHeight / 3);
    create_monitor(right_right, "课件画面", monitorsHeight / 3);


}


//移除布局
void RecordPage::clear_monitors() {
    lv_obj_clean(monitors);
}

/**
 * 创建一个小监视器
 * @param parent 父组件
 * @param text 名字
 * @param h 高度
 * @return
 */
lv_obj_t *RecordPage::create_monitor(lv_obj_t *parent, const char *name, lv_coord_t h) {
    auto container = lv_cont_create(parent, NULL);
    lv_cont_set_fit2(container, LV_FIT_FLOOD, LV_FIT_NONE);
    lv_obj_set_height(container, h);
    lv_obj_set_style(container, &lv_style_transp_tight);
    lv_cont_set_layout(container, LV_LAYOUT_COL_L);

    auto label = lv_label_create(container, NULL);
    lv_label_set_text(label, name);
    lv_obj_set_style(container, &my_style_transp_with_border2);

    return container;
}


void RecordPage::init_center_monitors() {
    clear_monitors();
    lv_cont_set_layout(monitors, LV_LAYOUT_ROW_M);

    auto monitorsWidth = lv_obj_get_width(monitors);
    auto monitorsHeight = lv_obj_get_height(monitors);

    auto left = lv_cont_create(monitors, NULL);
    lv_cont_set_fit2(left, LV_FIT_NONE, LV_FIT_FLOOD);
    lv_obj_set_width(left, monitorsWidth / 2);
    lv_obj_set_style(left, &lv_style_transp_tight);

    create_monitor(left, "主播", monitorsHeight);

    auto right = lv_cont_create(monitors, NULL);
    lv_obj_set_style(right, &lv_style_transp_tight);
    lv_cont_set_fit2(right, LV_FIT_NONE, LV_FIT_FLOOD);
    lv_obj_set_width(right, monitorsWidth / 2);
    lv_cont_set_layout(right, LV_LAYOUT_COL_M);


    create_monitor(right, "教师全景", monitorsHeight / 2);
    create_monitor(right, "板书特写", monitorsHeight / 2);

}

void RecordPage::create_menu(lv_obj_t *parent, const char *title) {
    auto container = lv_cont_create(parent, NULL);
    lv_cont_set_fit2(container, LV_FIT_TIGHT, LV_FIT_FLOOD);
    lv_cont_set_layout(container, LV_LAYOUT_COL_M);
    auto label = lv_label_create(container, NULL);
    lv_label_set_text(label, title);

    auto btn = lv_btn_create(container, NULL);
    label = lv_label_create(btn, NULL);
    lv_label_set_text(label, MY_SYMBOL_CIRCLE"开始录制");
    lv_obj_set_height(btn, 30);

    lv_obj_set_event_cb(btn, call_cb);

    btn = lv_btn_create(container, NULL);
    label = lv_label_create(btn, NULL);
    lv_label_set_text(label, MY_SYMBOL_PAUSE"暂停录制");
    lv_obj_set_height(btn, 30);

}

void RecordPage::call_cb(struct _lv_obj_t *obj, lv_event_t event) {
    if (event == LV_EVENT_PRESSED) {
        auto r = RestClient::get("https://driving.seeyouweb.com/tea_database/orders?page=&size=&sort=");
        std::cout << r.body << std::endl;
    }
}