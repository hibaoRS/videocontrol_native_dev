//
// Created by 10324 on 2019/8/1.
//

#include <cstring>
#include "ShellUtils.h"

int ShellUtils::execute(const string& cmd, std::function<void(const char *)> handler) {
    FILE *fp= popen(cmd.c_str(), "r");
    char buf[2048];
    while (true) {
        memset(buf, '\0', sizeof(buf));  //初始化buf,以免后面写入乱码到文件中
        /* 若命令没有结束则fgets会等待,逐行读取文件 */
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            break;
        }
        //调用lambda函数
        handler(buf);
    }
    return pclose(fp);
}
