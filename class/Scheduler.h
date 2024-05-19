//
// Created by 15517 on 24-5-19.
//

#ifndef PROCESSSCHEDULING_SCHEDULER_H
#define PROCESSSCHEDULING_SCHEDULER_H

#include <vector>
#include <algorithm>
#include "PCB.h"

using namespace std;

class Scheduler {
    std::vector<PCB> processes;  // 存储所有的进程

public:
    void addProcess(const PCB &process) {
        processes.push_back(process);
    }

    void schedule_shortest_job_first(std::vector<PCB> &processes) {
        // 对进程列表按照所需运行时间进行排序
        std::sort(processes.begin(), processes.end(), [](const PCB &a, const PCB &b) {
            return a.requiredTime < b.requiredTime;
        });

        // 遍历排序后的进程列表，对每个进程进行调度
        for (PCB &process: processes) {
            process.processState = "R";  // 将进程状态改为运行
            process.elapsedTime += std::chrono::seconds(1);  // 增加已运行时间
        }
    }

// 时间片轮转调度算法
    void schedule_round_robin() {
        std::chrono::duration<double> timeSlice = std::chrono::seconds(2); // 设定时间片长度
        for (PCB &process: processes) {
            if (process.requiredTime > timeSlice) {
                process.requiredTime -= timeSlice;
                process.elapsedTime += timeSlice;
            } else {
                process.elapsedTime += process.requiredTime;
                process.requiredTime = std::chrono::seconds(0);
                process.processState = "F";  // 将进程状态改为完成
            }
        }
    }

// 高响应比优先调度算法
    void schedule_highest_response_ratio_next() {
        for (PCB &process: processes) {
            // 计算响应比
            process.responseRatio = (process.elapsedTime + process.requiredTime) / process.requiredTime;
        }

        // 对进程列表按照响应比进行排序
        std::sort(processes.begin(), processes.end(), [](const PCB &a, const PCB &b) {
            return a.responseRatio > b.responseRatio;
        });

        // 调度响应比最高的进程
        if (!processes.empty()) {
            PCB &process = processes.front();
            process.elapsedTime += process.requiredTime;
            process.requiredTime = std::chrono::seconds(0);
            process.processState = "F";  // 将进程状态改为完成
        }
    }
};

#endif //PROCESSSCHEDULING_SCHEDULER_H
