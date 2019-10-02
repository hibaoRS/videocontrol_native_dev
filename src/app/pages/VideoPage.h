//
// Created by 10324 on 2019/7/6.
//

#ifndef LVGL_VIDEOPAGE_H
#define LVGL_VIDEOPAGE_H


#include <src/app/BasePage.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

class VideoPage : public BasePage {
public :
    explicit VideoPage(lv_obj_t *parent) : BasePage(parent) {
        //TODO pThis为什么为空
        pThis = this;
        init_layout();
    }


private:
    static VideoPage *pThis;

    static const string root_dir;
    static const string root_dir1;
    static const string root_dir2;
    string *copy_link = new string();

    const int name_width = 500;
    const int size_width = 100;


    void init_layout() override;;

    void create_file_explorer();

    lv_obj_t *explorer;
    lv_obj_t *files_content;
    lv_obj_t *folder_info;

    lv_obj_t *curr_folder_label;
    lv_obj_t *copy_link_label;
    lv_obj_t *copy_info_area;

    string *curr_dir;


    vector<map<string, string>> files;

    void open_folder(const string &fd);

    void flush_files();

    static void file_click_cb(lv_obj_t *btn, lv_event_t event);

    static void copy_file_cb(lv_obj_t *btn, lv_event_t event);
    static void cancel_copy_cb(lv_obj_t *btn, lv_event_t event);
    static void paste_cb(lv_obj_t *btn, lv_event_t event);

    void flush_file_info();

};


#endif //LVGL_VIDEOPAGE_H
