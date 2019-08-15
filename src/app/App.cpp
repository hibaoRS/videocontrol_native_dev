//
// Created by xqdd on 7/4/19.
//

#include <lv_examples/lv_apps/demo/demo.h>
#include <lv_examples/lv_tests/lv_test_theme/lv_test_theme_1.h>
#include <lv_examples/lv_tests/lv_test.h>
#include <lv_examples/lv_apps/benchmark/benchmark.h>
#include <string>
#include <res/font/my_symbol_def.h>
#include <src/app/pages/RecordPage.h>
#include <src/app/pages/VideoPage.h>
#include <src/app/styles/my_style.h>
#include <lvgl/src/lv_core/lv_style.h>
#include <src/app/utils/CpuMemory.h>
#include <iostream>
#include "App.h"
#include <map>
#include <string>
#include "../../res/img/my_img_def.h"

using namespace std;
lv_obj_t *App::memory_label = NULL;
lv_obj_t *App::cpu_label = NULL;
lv_obj_t *App::devices_ddlist = NULL;
lv_obj_t *App::tab_view = NULL;
string App::curr_devices = "";


void App::init_app() {

//    init_layout();

    /*Select display 1*/
//    demo_create();

    /*Try the benchmark to see how fast your GUI is*/
//    benchmark_create();

    /*Check the themes too*/
//    lv_test_theme_1(lv_theme_night_init(15, NULL));

    /*Try the touchpad-less navigation (use the Tab and Arrow keys or the Mousewheel)*/
//    lv_test_group_1();

    //初始化样式
    init_my_styles();

    //主布局
    lv_obj_t *main_layout = lv_cont_create(lv_scr_act(), NULL);

    lv_obj_set_style(main_layout, &lv_style_transp_tight);
    lv_obj_set_style(lv_scr_act(), &lv_style_transp_tight);

    lv_cont_set_layout(main_layout, LV_LAYOUT_COL_M);
    lv_cont_set_fit(main_layout, LV_FIT_FLOOD);

    //导航栏和页面
    create_navigation_bar(main_layout);
    create_tabview(main_layout);


    lv_tabview_set_tab_act(tab_view, 1, true);

}

void App::create_tabview(lv_obj_t *parent) {
    tab_view = lv_tabview_create(parent, NULL);
    lv_obj_set_style(tab_view, &lv_style_transp_tight);
    lv_tabview_set_btns_hidden(tab_view, true);

    //监控页面
    lv_obj_t *page;
    page = lv_tabview_add_tab(tab_view, "");
    new RecordPage(page);
    //录像页面
    page = lv_tabview_add_tab(tab_view, "");
    new VideoPage(page);

    //设置页面
    page = lv_tabview_add_tab(tab_view, "");

}

void App::readCpuMemoryEvent(lv_obj_t *obj, lv_event_t event) {
    auto m = *(map<string, string> *) lv_event_get_data();
    auto it = m.begin();

    char s[100];
    if (cpu_label != NULL) {
        sprintf(s, "CPU使用:%s%%", m["cpuPercent"].c_str());
        lv_label_set_text(cpu_label, s);
    }
    if (memory_label != NULL) {
        sprintf(s, "内存使用:%s%%,%sM/%sM", m["memoryPercent"].c_str(),
                m["memoryUse"].c_str(), m["memoryAll"].c_str());
        lv_label_set_text(memory_label, s);
    }
    if (devices_ddlist != NULL) {
        /**
        auto ext = (lv_ddlist_ext_t *) lv_obj_get_ext_attr(devices_ddlist);
        //设备下拉框没有打开，则重新刷新数据
        if (!ext->opened) {
            auto index = ext->sel_opt_id;
            lv_ddlist_set_options(devices_ddlist, m["devices"].c_str());
            lv_ddlist_set_selected(devices_ddlist, index);
        }**/
        if (m["devices"] != curr_devices) {
            curr_devices = m["devices"];
            lv_roller_set_options(devices_ddlist, curr_devices.c_str(), LV_ROLLER_MODE_INIFINITE);
        }
    }
}

//导航栏
void App::create_navigation_bar(lv_obj_t *parent) {
    //导航栏容器
    auto navigation_bar_cont = lv_cont_create(parent, NULL);
    lv_cont_set_fit2(navigation_bar_cont, LV_FIT_FLOOD, LV_FIT_TIGHT);
    lv_cont_set_layout(navigation_bar_cont, LV_LAYOUT_ROW_M);
    //logo
    auto icon = lv_img_create(navigation_bar_cont, NULL);
    lv_img_set_src(icon, &logo);

    //导航栏按钮
    lv_obj_t *list = lv_list_create(navigation_bar_cont, NULL);
    lv_obj_set_style(list, &lv_style_transp_tight);

    lv_cont_set_fit2(list, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_list_set_style(list, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);
    lv_obj_set_width(list, lv_obj_get_width(parent) - lv_obj_get_width(icon));
    lv_page_set_scrl_layout(list, LV_LAYOUT_ROW_M);
    auto list_height = lv_obj_get_height(list) - 5;
    //允许导航栏滚动
    lv_page_set_scrl_fit(list, LV_FIT_TIGHT);


    //页面切换
    //页面下标
    int i = -1;
    lv_obj_t *btn;
    btn = lv_list_add_btn(list, MY_SYMBOL_DESKTOP, "监控");
    lv_obj_set_user_data(btn, new int(++i));
    lv_obj_set_event_cb(btn, switch_page);
    btn = lv_list_add_btn(list, MY_SYMBOL_VIDEO, "录像管理");
    lv_obj_set_user_data(btn, new int(++i));
    lv_obj_set_event_cb(btn, switch_page);
    lv_obj_set_event_cb(btn, switch_page);
    btn = lv_list_add_btn(list, MY_SYMBOL_SETTING, "设置");
    lv_obj_set_user_data(btn, new int(++i));
    lv_obj_set_event_cb(btn, switch_page);

    btn = lv_list_add_btn(list, NULL, "CPU使用:");
    lv_obj_set_click(btn, false);
    cpu_label = lv_list_get_btn_label(btn);
    lv_obj_set_width(cpu_label, 120);


    btn = lv_list_add_btn(list, NULL, "内存使用:");
    lv_obj_set_click(btn, false);
    memory_label = lv_list_get_btn_label(btn);
    lv_obj_set_width(memory_label, 250);


    auto cont = lv_cont_create(list, NULL);
    lv_obj_set_height(cont, list_height);
    lv_cont_set_layout(cont, LV_LAYOUT_CENTER);
    lv_cont_set_fit2(cont, LV_FIT_TIGHT, LV_FIT_NONE);
    lv_obj_set_style(cont, &lv_style_transp_fit);
//    devices_ddlist = lv_ddlist_create(cont, NULL);
//    lv_ddlist_set_style(devices_ddlist, LV_DDLIST_STYLE_BG, lv_obj_get_style(btn));
//    lv_ddlist_set_fix_height(devices_ddlist, list_height);

    devices_ddlist = lv_roller_create(cont, NULL);
    lv_roller_set_style(devices_ddlist, LV_ROLLER_STYLE_BG, lv_obj_get_style(btn));
    lv_obj_set_height(devices_ddlist, list_height * 3);

    lv_obj_set_event_cb(CpuMemory::Instance()->obj, readCpuMemoryEvent);

}


void App::switch_page(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        lv_tabview_set_tab_act(tab_view, *(int *) lv_obj_get_user_data(btn), true);
    }
}

