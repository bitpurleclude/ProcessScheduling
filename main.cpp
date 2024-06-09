
#include "Scheduler.h"
#include "generateRandomProcess.h"

int main() {
    Scheduler scheduler;
    initScheduler(&scheduler, 10, 0);

    // 生成10个随机的进程
    for (int i = 0; i < 10; ++i) {
        usleep(50 * 1000);  // 延时50毫秒
        PCB process = generateRandomProcessWithCount(i);
        addProcess(&scheduler, process);
    }

    // 执行调度算法
    //schedule_shortest_job_first(&scheduler);
    //schedule_round_robin(&scheduler);
    schedule_highest_response_ratio_next(&scheduler);

    return 0;
}
