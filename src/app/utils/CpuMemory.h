//
// Created by 10324 on 2019/7/29.
//

#ifndef LVGL_CPUMEMORY_H
#define LVGL_CPUMEMORY_H
#define MB (1024 * 1024)
#define KB (1024)

#include <lvgl/lvgl.h>
#include <map>
#include <thread>
#include <functional>
#include <unistd.h>


using namespace std;


class CpuMemory {
private :

    explicit CpuMemory();;

    void check_cpu_memory();

    void check_device();

    void send_cpu_memory();

    static CpuMemory *self;
    map<string, string> m;

    int memoryPercent, memoryAll, memoryUse, memoryFree,cpuPercent;
    int totalNew, idleNew, totalOld, idleOld;

    thread *t;


public:

    static CpuMemory *Instance();

    lv_obj_t *obj;

    void cat_cpu();

    void cat_memory();

    void exe_shell(const char string[4]);

    void read_cpu_memory_data(FILE *pFile);

    void get_device_info(const char str[], map<string, string> *map, string basicString);
};


#endif //LVGL_CPUMEMORY_H
