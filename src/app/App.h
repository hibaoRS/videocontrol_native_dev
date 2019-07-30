//
// Created by xqdd on 7/4/19.
//

#ifndef LVGL_APP_H
#define LVGL_APP_H


class App {
public:

    void init_app();

private:
    void create_tabview(lv_obj_t *parent);

    static void switch_page(lv_obj_t *btn, lv_event_t event);

    void create_navigation_bar(lv_obj_t *parent);

    static lv_obj_t *memory_label;
    static lv_obj_t *cpu_label;
    static lv_obj_t *devices_ddlist;
    static lv_obj_t *tab_view;

    static void readCpuMemoryEvent(lv_obj_t *obj, lv_event_t event);
};


#endif //LVGL_APP_H
