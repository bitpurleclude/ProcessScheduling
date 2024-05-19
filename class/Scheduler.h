//
// Created by 15517 on 24-5-19.
//

#ifndef PROCESSSCHEDULING_SCHEDULER_H
#define PROCESSSCHEDULING_SCHEDULER_H

#include <vector>
#include <algorithm>
#include <thread>
#include <iostream>
#include <deque>
#include "PCB.h"

using namespace std;

class Scheduler {
    std::vector<PCB> processes;  // 存储所有的进程
    std::chrono::duration<double> timeSlice;  // 时间片长度
    std::chrono::duration<double> finishTime;
    std::chrono::duration<double> totalSchedulingTime;

public:
    Scheduler(std::chrono::duration<double> timeSlice, std::chrono::duration<double> finishTime) {
        this->timeSlice = timeSlice;
        this->finishTime = finishTime;
    }

    void addProcess(const PCB &process) {
        processes.push_back(process);
    }

    // 打印所有进程的状态
    void printProcesses() {
        for (const PCB &process: processes) {
            std::cout << "Process Name: " << process.processName
                      << ", State: " << process.processState
                      << ", Elapsed Time: " << process.elapsedTime.count()
                      << ", Required Time: " << process.requiredTime.count()
                      << ", Response Ratio: " << process.responseRatio << std::endl;
        }
    }

    void schedule_shortest_job_first() {
        // 对进程列表按照所需运行时间进行排序
        std::sort(processes.begin(), processes.end(), [](const PCB &a, const PCB &b) {
            if (a.requiredTime == b.requiredTime) {
                return a.priority < b.priority;  // 如果所需时间相同，比较优先级
            }
            return a.requiredTime < b.requiredTime;  // 否则，比较所需时间
        });

        // 遍历排序后的进程列表，对每个进程进行调度
        for (PCB &process: processes) {
            if (process.processState == "W" || process.processState == "R") {
                process.processState = "R";  // 将进程状态改为运行
                process.startTime = std::chrono::system_clock::now();  // 记录开始时间
                while (process.requiredTime > timeSlice) {
                    process.elapsedTime += timeSlice;  // 增加已运行时间
                    process.requiredTime -= timeSlice;
                    std::this_thread::sleep_for(timeSlice);
                }
                process.processState = "F";  // 将进程状态改为完成
                std::chrono::duration<double> schedulingTime = std::chrono::system_clock::now() - process.startTime;  // 计算调度时间
                totalSchedulingTime += schedulingTime;  // 添加到总的调度时间
                process.elapsedTime += process.requiredTime;
                process.requiredTime = finishTime;
            }
            printProcesses();
        }
        std::cout << "Average Scheduling Time: " << totalSchedulingTime.count() / processes.size() << std::endl;  // 计算并打印平均调度时间
    }

// 时间片轮转调度算法
    void schedule_round_robin() {
        // 使用队列来存储进程的指针，以便在每个时间片结束后将当前进程移动到队列的末尾
        std::deque<PCB*> processQueue;
        for (PCB &process : processes) {
            processQueue.push_back(&process);
        }

        while (!processQueue.empty()) {
            PCB* process = processQueue.front();  // 获取队列中的第一个进程

            if (process->processState == "W" || process->processState == "R") {
                process->processState = "R";  // 将进程状态改为运行
                process->startTime = std::chrono::system_clock::now();  // 记录开始时间
                if (process->requiredTime > timeSlice) {
                    process->requiredTime -= timeSlice;
                    std::this_thread::sleep_for(timeSlice);
                    process->elapsedTime += timeSlice;
                } else {
                    process->elapsedTime += process->requiredTime;
                    process->requiredTime = finishTime;
                    process->processState = "F";
                    std::chrono::duration<double> schedulingTime = std::chrono::system_clock::now() - process->startTime;  // 计算调度时间
                    totalSchedulingTime += schedulingTime;  // 添加到总的调度时间// 将进程状态改为完成
                }

                if (process->processState != "F") {
                    processQueue.push_back(process);  // 如果进程还没有完成，将它移动到队列的末尾
                }
            }

            processQueue.pop_front();  // 移除队列中的第一个进程
            printProcesses();
        }
        std::cout << "Average Scheduling Time: " << totalSchedulingTime.count() / processes.size() << std::endl;  // 计算并打印平均调度时间
    }

// 高响应比优先调度算法
// 高响应比优先调度算法
    void schedule_highest_response_ratio_next() {
        for (PCB &process: processes) {
            // 计算等待时间
            std::chrono::duration<double> waitingTime = process.elapsedTime;

            // 计算响应比
            process.responseRatio = (waitingTime + process.requiredTime) / process.requiredTime;
        }

        while (!processes.empty()) {
            // 对进程列表按照响应比进行排序
            std::sort(processes.begin(), processes.end(), [](const PCB &a, const PCB &b) {
                return a.responseRatio > b.responseRatio;
            });

            // 调度响应比最高的进程
            PCB &process = processes.front();
            process.processState = "R";  // 将进程状态改为运行
            process.startTime = std::chrono::system_clock::now();  // 记录开始时间
            process.elapsedTime += process.requiredTime;
            process.requiredTime = finishTime;
            process.processState = "F";  // 将进程状态改为完成
            std::chrono::duration<double> schedulingTime = std::chrono::system_clock::now() - process.startTime;  // 计算调度时间
            totalSchedulingTime += schedulingTime;  // 添加到总的调度时间

            // 从进程列表中移除已完成的进程
            processes.erase(processes.begin());
        }
        // 打印所有进程的状态
        printProcesses();
        std::cout << "Average Scheduling Time: " << totalSchedulingTime.count() / processes.size() << std::endl;  // 计算并打印平均调度时间
    }
};

#endif //PROCESSSCHEDULING_SCHEDULER_H