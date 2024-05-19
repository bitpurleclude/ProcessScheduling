//
// Created by 15517 on 24-5-19.
//

#ifndef PROCESSSCHEDULING_PCB_H
#define PROCESSSCHEDULING_PCB_H


#include <string>
#include <chrono>

class PCB {
    std::string processName;  // 进程名
    int priority;  // 进程优先级
    std::chrono::system_clock::time_point arrivalTime;  // 进程到达时间
public:
    std::chrono::duration<double> requiredTime;
    std::string processState;
    std::chrono::duration<double> elapsedTime;
    double responseRatio;
};


#endif //PROCESS SCHEDULING_PCB_H
