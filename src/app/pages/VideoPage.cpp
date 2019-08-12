//
// Created by 10324 on 2019/7/6.
//

#include <lvgl/src/lv_core/lv_style.h>
#include <src/app/utils/ShellUtils.h>
#include <src/app/utils/StringUtils.h>
#include <src/app/utils/IconUtils.h>
#include "VideoPage.h"

VideoPage *VideoPage::pThis = NULL;

void VideoPage::init_layout() {
    create_file_explorer();
}

void VideoPage::create_file_explorer() {
    explorer = lv_cont_create(layout, NULL);
    lv_cont_set_layout(explorer, LV_LAYOUT_COL_L);
    lv_cont_set_fit2(explorer, LV_FIT_FLOOD, LV_FIT_TIGHT);

    auto header = lv_cont_create(explorer, NULL);
    lv_cont_set_layout(header, LV_LAYOUT_ROW_M);
    lv_cont_set_fit2(header, LV_FIT_FLOOD, LV_FIT_TIGHT);

    auto headerText = lv_label_create(header, NULL);
    lv_label_set_text(headerText, "文件名");
    lv_label_set_long_mode(headerText, LV_LABEL_LONG_SROLL);
    lv_obj_set_width(headerText, 300);

    headerText = lv_label_create(header, NULL);
    lv_label_set_text(headerText, "大小");
    lv_label_set_long_mode(headerText, LV_LABEL_LONG_SROLL);
    lv_obj_set_width(headerText, 100);

    files_content = lv_list_create(explorer, NULL);
    lv_cont_set_fit2(files_content, LV_FIT_FLOOD, LV_FIT_NONE);
    lv_obj_set_height(files_content, lv_obj_get_height(layout) - lv_obj_get_height(header) - 15);
    lv_list_set_style(files_content, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);

    open_folder("/mnt/d");
}


void VideoPage::flush_files() {
    lv_list_clean(files_content);
    for (auto &file:files) {
        auto row = lv_list_add_btn(files_content, NULL, NULL);
        lv_cont_set_layout(row, LV_LAYOUT_ROW_T);
        lv_cont_set_fit2(row, LV_FIT_FLOOD, LV_FIT_TIGHT);

        auto text = lv_label_create(row, NULL);
        bool isFile = file["type"] == "file";
        auto name = file["name"];

        lv_obj_set_user_data(row, &file);
        lv_obj_set_event_cb(row, file_click_cb);

        string icon = isFile ? StringUtils::EndsWithNoCase(name, ".mp4") ? ICON_FILM : ICON_FILE : ICON_FOLDER;
        lv_label_set_text(text, icon.append(" ").append(name).c_str());
        lv_label_set_long_mode(text, LV_LABEL_LONG_SROLL);
        lv_obj_set_width(text, 300);

        if (isFile) {
            text = lv_label_create(row, NULL);
            lv_label_set_text(text, file["size"].c_str());
            lv_label_set_long_mode(text, LV_LABEL_LONG_SROLL);
            lv_obj_set_width(text, 100);
        }
    }
}


void VideoPage::file_click_cb(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        auto data = *(map<string, string> *) lv_obj_get_user_data(btn);
        if (data["type"] == "dir" && pThis != NULL) {
            pThis->open_folder(data["name"]);
        }
    }
}

void VideoPage::open_folder(const string &fd) {
    string dir;
    if (StringUtils::StartsWith(fd, "/")) {
        dir = fd;
    } else {
        dir = *curr_dir + "/" + fd;
    }
    files.clear();
    ShellUtils::execute("ls " + dir + " -hal --group-directories-first", [this](const char *line) {
        if (StringUtils::StartsWithNoCase(line, "total")) {
            return;
        }
        vector<string> list = StringUtils::Split(line, " ");
        list.erase(remove_if(list.begin(), list.end(),
                             [](const string &str) { return str.empty(); }),
                   list.end());
        map<string, string> file;
        file["type"] = StringUtils::StartsWith(list[0], "d") ? "dir" : "file";
        file["size"] = list[4];
        string name = list[8];
        StringUtils::Replace(name, "\n", "");
        file["name"] = name;
        files.push_back(file);
    });
    ShellUtils::execute("cd " + dir + ";pwd", [this](const char *str) {
        curr_dir = new string(str);
    });
    flush_files();
}


