//
// Created by 10324 on 2019/8/1.
//

#ifndef LVGL_SHELLUTILS_H
#define LVGL_SHELLUTILS_H

#include <functional>
#include <string>
using std::string;

class ShellUtils {
public :
    static int execute(const string& cmd, std::function<void(const char *)> handler);

};


#endif //LVGL_SHELLUTILS_H
