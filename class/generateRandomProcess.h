//
// Created by 15517 on 24-5-19.
//

#ifndef PROCESSSCHEDULING_GENERATERANDOMPROCESS_H
#define PROCESSSCHEDULING_GENERATERANDOMPROCESS_H

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "PCB.h"


PCB generateRandomProcessWithCount(int processCount) {
    // 创建一个随机数生成器
    srand(time(NULL));

    // 生成随机的到达时间、所需时间和优先级
    int arrivalTime = processCount == 0 ? 0 : rand() % 1000 + 1;
    int requiredTime = rand() % 100 + 1;
    int priority = rand() % 1000 + 1;

    // 创建一个新的PCB对象
    PCB process;
    process.arrivalTime = arrivalTime;
    process.requiredTime = requiredTime;
    process.priority = priority;
    process.elapsedTime = 0;
    strcpy(process.processState, "W");
    sprintf(process.processName, "Process %d", processCount + 1);  // 设置进程名

    return process;
}

#endif //PROCESSSCHEDULING_GENERATERANDOMPROCESS_H
