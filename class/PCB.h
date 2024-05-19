//
// Created by 15517 on 24-5-19.
//

#ifndef PROCESSSCHEDULING_PCB_H
#define PROCESSSCHEDULING_PCB_H


#include <string>
#include <chrono>

class PCB {
public:
    std::chrono::duration<double> requiredTime; // 进程所需的运行时间
    std::string processState; // 进程的状态，例如"R"表示运行，"W"表示等待，"F"表示完成
    std::chrono::duration<double> elapsedTime; // 进程已经运行的时间
    double responseRatio; // 进程的响应比
    std::string processName; // 进程的名称
    std::chrono::system_clock::time_point arrivalTime; // 进程的到达时间
    int priority; // 进程的优先级
    std::chrono::time_point<std::chrono::system_clock> startTime;
};


#endif //PROCESS SCHEDULING_PCB_H
