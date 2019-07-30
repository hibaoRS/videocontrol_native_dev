//
// Created by 10324 on 2019/7/29.
//

#include "CpuMemory.h"
#include "StringUtils.h"

CpuMemory *CpuMemory::self = NULL;

void CpuMemory::check_cpu_memory() {
    cat_cpu();
    cat_memory();
}

void CpuMemory::send_cpu_memory() {
    int times = 0;
    check_device();
    while (true) {
        if (times > 3) {
            times = 0;
            check_device();
        }
        check_cpu_memory();
        lv_event_send(obj, LV_EVENT_VALUE_CHANGED, &m);
        usleep(3 * 1000 * 1000);
        times++;
    }
}

CpuMemory *CpuMemory::Instance() {
    if (!self) {
        self = new CpuMemory();
    }
    return self;
}

CpuMemory::CpuMemory() {
    memoryPercent = 0;
    memoryAll = 0;
    memoryUse = 0;
    memoryFree = 0;
    totalNew = 0;
    idleNew = 0;
    totalOld = 0;
    idleOld = 0;
    cpuPercent = 0;
    obj = lv_obj_create(NULL, NULL);
    t = new thread(std::bind(&CpuMemory::send_cpu_memory, this));
}

void CpuMemory::cat_cpu() {
    exe_shell("cat /proc/stat");
}

void CpuMemory::cat_memory() {
    exe_shell("cat /proc/meminfo");
}

void CpuMemory::exe_shell(const char *str) {
    FILE *fp;
    fp = popen(str, "r");
    read_cpu_memory_data(fp);
    pclose(fp);
}

void CpuMemory::read_cpu_memory_data(FILE *pFile) {
    char buf[2048];
    while (true) {
        memset(buf, '\0', sizeof(buf));  //初始化buf,以免后面写入乱码到文件中
        /* 若命令没有结束则fgets会等待,逐行读取文件 */
        if (fgets(buf, sizeof(buf), pFile) == NULL) {
            break;
        }
        string s(buf);
        if (StringUtils::StartsWithNoCase(s, "cpu ")) {
            vector<string> list = StringUtils::Split(s, " ");
            idleNew = StringUtils::StrToLongInt(list.at(5).c_str());
            for (const string &value: list) {
                totalNew += StringUtils::StrToLongInt(value.c_str());
            }
            int total = totalNew - totalOld;
            int idle = idleNew - idleOld;
            cpuPercent = 100 * (total - idle) / total;
            totalOld = totalNew;
            idleOld = idleNew;
            break;
        } else if (StringUtils::StartsWithNoCase(s, "MemTotal")) {
            StringUtils::Replace(s, " ", "");
            s = StringUtils::Split(s, ":").at(1);
            memoryAll = StringUtils::StrToLongInt(StringUtils::Left(s, s.length() - 3)) / KB;
        } else if (StringUtils::StartsWithNoCase(s, "MemFree")) {
            StringUtils::Replace(s, " ", "");
            s = StringUtils::Split(s, ":").at(1);
            memoryFree = StringUtils::StrToLongInt(StringUtils::Left(s, s.length() - 3)) / KB;
        } else if (StringUtils::StartsWithNoCase(s, "Buffers")) {
            StringUtils::Replace(s, " ", "");
            s = StringUtils::Split(s, ":").at(1);
            memoryFree += StringUtils::StrToLongInt(StringUtils::Left(s, s.length() - 3)) / KB;
        } else if (StringUtils::StartsWithNoCase(s, "Cached")) {
            StringUtils::Replace(s, " ", "");
            s = StringUtils::Split(s, ":").at(1);
            memoryFree += StringUtils::StrToLongInt(StringUtils::Left(s, s.length() - 3)) / KB;
            memoryUse = memoryAll - memoryFree;
            memoryPercent = 100 * memoryUse / memoryAll;
            break;
        }
    }

    m["cpuPercent"] = std::to_string(cpuPercent);
    m["memoryPercent"] = std::to_string(memoryPercent);
    m["memoryUse"] = std::to_string(memoryUse);
    m["memoryAll"] = std::to_string(memoryAll);
}

void CpuMemory::check_device() {
    FILE *fp;
    fp = popen("df -h", "r");

    char buf[2048];
    map<string, string> devices;
    int usb_num = 1;
    while (true) {
        memset(buf, '\0', sizeof(buf));  //初始化buf,以免后面写入乱码到文件中
        /* 若命令没有结束则fgets会等待,逐行读取文件 */
        if (fgets(buf, sizeof(buf), fp) == NULL) {
            break;
        }
        string s(buf);
        if (StringUtils::StartsWithNoCase(s, "Filesystem")) {
            continue;
        }
#if __arm__
            else if (StringUtils::StartsWithNoCase(s, "/dev/sda")) {
                get_device_info("内置硬盘", devices, s);
            }
#else
        else if (true) {
            get_device_info(NULL, &devices, s);
        }
#endif
        else if (StringUtils::StartsWithNoCase(s, "/dev/sdb")) {
            char name[64];
            sprintf(name, "U盘%i", usb_num);
            get_device_info(name, &devices, s);
            usb_num++;
        }
    }
    pclose(fp);

    vector<string> v;
    auto it = devices.begin();
    while (it != devices.end()) {
        v.push_back(it->first + ":" + it->second);
        it++;
    }
    m["devices"] = StringUtils::Join(v, "\n");
}

void CpuMemory::get_device_info(const char *chs, map<string, string> *map, string str) {
    vector<string> list = StringUtils::Split(str, " ");
    list.erase(std::remove_if(
            list.begin(), list.end(),
            [](const string &s) {
                return s == "";
            }), list.end());
    string val = list.at(4) + " " + list.at(2) + "/" + list.at(1);
    if (chs != NULL) {
        (*map)[chs] = val;
    } else {
        (*map)[list.at(0)] = val;
    }
}
