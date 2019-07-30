//
// Created by 10324 on 2019/7/6.
//

#ifndef LVGL_RECORDPAGE_H
#define LVGL_RECORDPAGE_H


#include <src/app/BasePage.h>

class RecordPage : public BasePage {
public:
    explicit RecordPage(lv_obj_t *parent) : BasePage(parent) {
        init_layout();
    }

private:
    lv_obj_t *menu;
    lv_obj_t *monitors;

protected:
    void init_layout() override;;

    void init_monitors();

    void init_menu();

    void init_left_right_monitors();

    void init_center_monitors();

    void clear_monitors();

private :
    lv_obj_t *create_monitor(lv_obj_t *parent, const char *name, lv_coord_t h);

    void create_menu(lv_obj_t *parent, const char *title);
    static void call_cb(struct _lv_obj_t * obj, lv_event_t event);
};


#endif //LVGL_RECORDPAGE_H
