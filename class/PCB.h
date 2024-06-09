//
// Created by 15517 on 24-5-19.
//

#ifndef PROCESSSCHEDULING_PCB_H
#define PROCESSSCHEDULING_PCB_H

#include <time.h>
#include <string.h>

typedef struct {
    double requiredTime; // 进程所需的运行时间
    char processState[2]; // 进程的状态，例如"R"表示运行，"W"表示等待，"F"表示完成
    double elapsedTime; // 进程已经运行的时间
    double waitingTime; // 进程等待时间
    double responseRatio; // 进程的响应比
    char processName[50]; // 进程的名称
    time_t arrivalTime; // 进程的到达时间
    int priority; // 进程的优先级
    time_t startTime; // 进程的开始时间
} PCB;

#endif //PROCESSSCHEDULING_PCB_H
