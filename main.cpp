#include <iostream>
#include "generateRandomProcess.h"
#include "Scheduler.h"

int main() {
    Scheduler scheduler = Scheduler(std::chrono::milliseconds(10) , std::chrono::milliseconds(0));

    // 生成10个随机的进程
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        PCB process = generateRandomProcess().generateRandomProcessWithCount(i);
        scheduler.addProcess(process);
    }

    // 执行调度算法
    scheduler.schedule_shortest_job_first();

    return 0;
}
