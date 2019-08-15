//
// Created by 10324 on 2019/7/6.
//

#include <lvgl/src/lv_core/lv_style.h>
#include <src/app/utils/ShellUtils.h>
#include <src/app/utils/StringUtils.h>
#include <src/app/utils/IconUtils.h>
#include "VideoPage.h"

VideoPage *VideoPage::pThis = NULL;
const string VideoPage::root_dir = "/mnt/d";
const string VideoPage::root_dir1 = root_dir + "/mnt/d";
const string VideoPage::root_dir2 = "/mnt";

void VideoPage::init_layout() {
    create_file_explorer();
}

void VideoPage::create_file_explorer() {
    explorer = lv_cont_create(layout, NULL);
    lv_cont_set_layout(explorer, LV_LAYOUT_COL_L);
    lv_cont_set_fit2(explorer, LV_FIT_FLOOD, LV_FIT_TIGHT);

    //表头
    auto header = lv_cont_create(explorer, NULL);
    lv_cont_set_layout(header, LV_LAYOUT_ROW_M);
    lv_cont_set_fit2(header, LV_FIT_FLOOD, LV_FIT_TIGHT);

    auto headerText = lv_label_create(header, NULL);
    lv_label_set_text(headerText, "文件名");
    lv_label_set_long_mode(headerText, LV_LABEL_LONG_SROLL);
    lv_obj_set_width(headerText, pThis->name_width);

    headerText = lv_label_create(header, NULL);
    lv_label_set_text(headerText, "大小");
    lv_label_set_long_mode(headerText, LV_LABEL_LONG_SROLL);
    lv_obj_set_width(headerText, pThis->size_width);

    int folder_info_height = 30;
    //文件列表
    files_content = lv_list_create(explorer, NULL);
    lv_cont_set_fit2(files_content, LV_FIT_FLOOD, LV_FIT_NONE);
    lv_obj_set_height(files_content, lv_obj_get_height(layout) - lv_obj_get_height(header) - 15 - folder_info_height);
    lv_list_set_style(files_content, LV_LIST_STYLE_SCRL, &lv_style_transp_tight);

    //信息栏
    folder_info = lv_cont_create(explorer, NULL);
    lv_cont_set_fit2(folder_info, LV_FIT_FLOOD, LV_FIT_NONE);
    lv_obj_set_height(folder_info, folder_info_height);
    lv_cont_set_layout(folder_info, LV_LAYOUT_ROW_M);

    auto label = lv_label_create(folder_info, NULL);
    lv_label_set_text(label, "当前文件夹：");

    curr_folder_label = lv_label_create(folder_info, NULL);
    lv_label_set_text(curr_folder_label, root_dir.c_str());


    //复制相关
    copy_info_area = lv_cont_create(folder_info, NULL);
    lv_cont_set_fit2(copy_info_area, LV_FIT_TIGHT, LV_FIT_NONE);
    lv_obj_set_height(copy_info_area, folder_info_height);
    lv_cont_set_layout(copy_info_area, LV_LAYOUT_ROW_M);

    label = lv_label_create(copy_info_area, NULL);
    lv_label_set_text(label, "当前已复制：");

    copy_link_label = lv_label_create(copy_info_area, NULL);
    lv_label_set_text(copy_link_label, copy_link->c_str());

    auto btn = lv_btn_create(copy_info_area, NULL);
    label = lv_label_create(btn, NULL);
    lv_label_set_text(label, "粘贴");

    btn = lv_btn_create(copy_info_area, NULL);
    lv_obj_set_event_cb(copy_info_area,)
    label = lv_label_create(btn, NULL);
    lv_label_set_text(label, "取消");

    open_folder(root_dir);
}


void VideoPage::flush_files() {
    lv_list_clean(pThis->files_content);
    for (auto &file:pThis->files) {
        auto row = lv_list_add_btn(pThis->files_content, NULL, NULL);
        //样式
        lv_obj_set_style(row, &lv_style_transp_tight);
        lv_btn_set_style(row, LV_BTN_STYLE_REL, &lv_style_transp_tight);
        lv_btn_set_style(row, LV_BTN_STYLE_PR, &lv_style_pretty_color);

        //单行布局
        lv_cont_set_layout(row, LV_LAYOUT_ROW_T);
        lv_cont_set_fit2(row, LV_FIT_FLOOD, LV_FIT_TIGHT);

        //回调设置
        lv_obj_set_user_data(row, &file);
        lv_obj_set_event_cb(row, file_click_cb);

        //单行数据
        bool isFile = file["type"] == "file";
        auto name = file["name"];
        string icon = isFile ? StringUtils::EndsWithNoCase(name, ".mp4") ? ICON_FILM : ICON_FILE : ICON_FOLDER;
        //图标
        auto text = lv_label_create(row, NULL);
        lv_label_set_text(text, (icon + " ").c_str());

        //文件名
        text = lv_label_create(row, NULL);
        lv_label_set_text(text, name.c_str());
        lv_label_set_long_mode(text, LV_LABEL_LONG_SROLL);
        lv_obj_set_width(text, pThis->name_width);

        //文件显示文件大小
        text = lv_label_create(row, NULL);
        lv_label_set_long_mode(text, LV_LABEL_LONG_SROLL_CIRC);
        lv_obj_set_width(text, pThis->size_width);

        if (isFile) {
            lv_label_set_text(text, file["size"].c_str());
        } else {
            lv_label_set_text(text, "");
        }

        //删除复制操作
        if (name != "..") {
            auto ops = lv_cont_create(row, NULL);
            lv_cont_set_layout(ops, LV_LAYOUT_ROW_M);
            lv_cont_set_fit2(ops, LV_FIT_TIGHT, LV_FIT_NONE);
            lv_obj_set_height(ops, 30);
            lv_obj_set_style(ops, &lv_style_transp_tight);

            auto btn = lv_btn_create(ops, NULL);
            auto label = lv_label_create(btn, NULL);
            lv_label_set_text(label, "复制");
            lv_obj_set_event_cb(btn, copy_file_cb);
            lv_obj_set_user_data(btn, &file);

            btn = lv_btn_create(ops, NULL);
            label = lv_label_create(btn, NULL);
            lv_label_set_text(label, "删除");
            lv_obj_set_event_cb(btn, copy_file_cb);
            lv_obj_set_user_data(btn, &file);
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
        dir = *pThis->curr_dir + "/" + fd;
    }
    if (dir == root_dir1 || dir == root_dir2) {
        return;
    }
    vector<map<string, string>> tempFiles;
    int result = ShellUtils::execute("ls '" + dir + "' -hal --group-directories-first", [&](const char *line) {
        if (StringUtils::StartsWithNoCase(line, "total")) {
            return;
        }
        vector<string> list = StringUtils::Split(line, " ");
        //跳过无权限的文件夹
        auto isDir = StringUtils::StartsWith(list[0], "d");
        if (isDir && list[0].find_last_of('r') != 7) {
            return;
        }
        //去除空字符串
        list.erase(remove_if(list.begin(), list.end(),
                             [](const string &str) { return str.empty(); }),
                   list.end());
        //防止文件名包含空格
        string name = list[8];
        for (unsigned long i = 9; i < list.size(); i++) {
            name += " " + list[i];
        }
        //跳过.文件夹
        if (name == ".\n") {
            return;
        }
        map<string, string> file;
        file["type"] = isDir ? "dir" : "file";
        file["size"] = list[4];
        StringUtils::Replace(name, "\n", "");
        file["name"] = name;
        tempFiles.push_back(file);
    });
    if (result != 0) {
        return;
    }
    pThis->files.clear();
    pThis->files.insert(pThis->files.end(), tempFiles.begin(), tempFiles.end());
    ShellUtils::execute("cd '" + dir + "';pwd", [this](const char *str) {
        auto name = new string(str);
        StringUtils::Replace(*name, "\n", "");
        pThis->curr_dir = name;
    });

    flush_files();
    flush_file_info();
}

void VideoPage::copy_file_cb(lv_obj_t *btn, lv_event_t event) {
    if (event == LV_EVENT_CLICKED) {
        auto data = *(map<string, string> *) lv_obj_get_user_data(btn);
        pThis->copy_link = new string(*pThis->curr_dir + "/" + data["name"]);
        pThis->flush_file_info();
    }
}

void VideoPage::flush_file_info() {
    if (copy_link->empty()) {
        lv_obj_set_hidden(copy_info_area, true);
    } else {
        lv_obj_set_hidden(copy_info_area, false);
        lv_label_set_text(copy_link_label, copy_link->c_str());
    }
    lv_label_set_text(curr_folder_label, curr_dir->c_str());
}


