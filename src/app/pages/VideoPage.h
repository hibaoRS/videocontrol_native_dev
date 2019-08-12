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
        pThis = const_cast<VideoPage *>(this);
        init_layout();
    }


private:
    void init_layout() override;;

    void create_file_explorer();

    lv_obj_t *explorer;
    lv_obj_t *files_content;

    string *curr_dir;

    static VideoPage *pThis;

    vector<map<string, string>> files;

    void open_folder(const string &fd);

    void flush_files();

    static void file_click_cb(lv_obj_t *btn, lv_event_t event);
};


#endif //LVGL_VIDEOPAGE_H
