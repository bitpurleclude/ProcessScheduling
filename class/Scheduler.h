//
// Created by 15517 on 24-5-19.
//

#ifndef PROCESSSCHEDULING_SCHEDULER_H
#define PROCESSSCHEDULING_SCHEDULER_H
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include "PCB.h"

#define MAX 100

typedef struct {
    PCB processes[MAX];  // 存储所有的进程
    int processCount;  // 进程数量
    double timeSlice;  // 时间片长度
    double finishTime;
    double totalSchedulingTime;
} Scheduler;

void initScheduler(Scheduler* scheduler, double timeSlice, double finishTime) {
    scheduler->timeSlice = timeSlice;
    scheduler->finishTime = finishTime;
    scheduler->processCount = 0;
}

void addProcess(Scheduler* scheduler, const PCB process) {
    scheduler->processes[scheduler->processCount++] = process;
}

// 打印所有进程的状态
void printProcesses(Scheduler* scheduler) {
    printf("=============================================================================\n");
    for (int i = 0; i < scheduler->processCount; i++) {
        PCB process = scheduler->processes[i];
        printf("进程名称: %s, 状态: %s, 已运行时间: %.2f, 所需时间: %.2f, 响应比: %.2f\n",
               process.processName, process.processState, process.elapsedTime, process.requiredTime, process.responseRatio);
    }
}
int comparePCB(const void* a, const void* b) {
    PCB* pcbA = (PCB*)a;
    PCB* pcbB = (PCB*)b;
    return pcbA->requiredTime - pcbB->requiredTime;
}
// 最短作业优先调度算法
void schedule_shortest_job_first(Scheduler* scheduler) {
    // 对进程列表按照所需运行时间进行排序
    qsort(scheduler->processes, scheduler->processCount, sizeof(PCB), comparePCB);

    // 遍历排序后的进程列表，对每个进程进行调度
    for (int i = 0; i < scheduler->processCount; i++) {
        PCB* process = &scheduler->processes[i];
        if (strcmp(process->processState, "W") == 0 || strcmp(process->processState, "R") == 0) {
            strcpy(process->processState, "R");  // 将进程状态改为运行
            process->startTime = time(NULL);  // 记录开始时间
            while (process->requiredTime > scheduler->timeSlice) {
                process->elapsedTime += scheduler->timeSlice;  // 增加已运行时间
                process->requiredTime -= scheduler->timeSlice;
                usleep(scheduler->timeSlice*1000);
            }
            strcpy(process->processState, "F");  // 将进程状态改为完成
            double schedulingTime = difftime(time(NULL), process->startTime);  // 计算调度时间
            scheduler->totalSchedulingTime += schedulingTime;  // 添加到总的调度时间
            process->elapsedTime += process->requiredTime;
            process->requiredTime = scheduler->finishTime;
        }
        printProcesses(scheduler);
    }
    printf("平均调度时间: %.2f\n", scheduler->totalSchedulingTime / scheduler->processCount);  // 计算并打印平均调度时间
}

// 时间片轮转调度算法
void schedule_round_robin(Scheduler* scheduler) {
    // 使用队列来存储进程的指针，以便在每个时间片结束后将当前进程移动到队列的末尾
    PCB* processQueue[MAX];
    int front = 0, rear = scheduler->processCount;
    for (int i = 0; i < scheduler->processCount; i++) {
        processQueue[i] = &scheduler->processes[i];
    }

    while (front != rear) {
        PCB* process = processQueue[front];  // 获取队列中的第一个进程

        if (strcmp(process->processState, "W") == 0 || strcmp(process->processState, "R") == 0) {
            strcpy(process->processState, "R");  // 将进程状态改为运行
            process->startTime = time(NULL);  // 记录开始时间
            if (process->requiredTime > scheduler->timeSlice) {
                process->requiredTime -= scheduler->timeSlice;
                usleep(scheduler->timeSlice*1000);
                process->elapsedTime += scheduler->timeSlice;
            } else {
                process->elapsedTime += process->requiredTime;
                process->requiredTime = scheduler->finishTime;
                strcpy(process->processState, "F");  // 将进程状态改为完成
            }
            double schedulingTime = difftime(time(NULL), process->startTime);  // 计算调度时间
            scheduler->totalSchedulingTime += schedulingTime;  // 添加到总的调度时间
            if (strcmp(process->processState, "F") != 0) {
                processQueue[rear++] = process;  // 如果进程还没有完成，将它移动到队列的末尾
                if(rear > MAX) {
                    rear%=MAX;
                }
            }
        }

        front++;
        if(front>MAX){
            front%=MAX;
        }// 移除队列中的第一个进程
        printProcesses(scheduler);
    }
    printf("平均调度时间: %.2f\n", scheduler->totalSchedulingTime / scheduler->processCount);  // 计算并打印平均调度时间
}

int comparePCBResponseRatio(const void* a, const void* b) {
    PCB* pcbA = (PCB*)a;
    PCB* pcbB = (PCB*)b;
    if (pcbA->responseRatio < pcbB->responseRatio) {
        return 1;
    } else if (pcbA->responseRatio > pcbB->responseRatio) {
        return -1;
    } else {
        return 0;
    }
}
// 高响应比优先调度算法
void schedule_highest_response_ratio_next(Scheduler* scheduler) {
    for (int i = 0; i < scheduler->processCount; i++) {
        PCB* process = &scheduler->processes[i];
        // 计算等待时间
        double waitingTime = process->waitingTime;

        // 计算响应比
        process->responseRatio = (waitingTime + process->requiredTime) / process->requiredTime;
    }



    // 遍历排序后的进程列表，对每个进程进行调度
    for (int i = 0; i < scheduler->processCount; i) {
        // 对进程列表按照响应比进行排序
        qsort(scheduler->processes, scheduler->processCount, sizeof(PCB), comparePCBResponseRatio);
        PCB* process = &scheduler->processes[i];
        if (strcmp(process->processState, "F") == 0){
            break;
        }
        process->startTime = time(NULL);  // 记录开始时间
        int requiredTime = process->requiredTime;
        strcpy(process->processState, "R");  // 将进程状态改为运行
        process->elapsedTime += process->requiredTime;
        process->requiredTime = scheduler->finishTime;
        usleep(requiredTime * 1000);  // 模拟进程运行
        strcpy(process->processState, "F");  // 将进程状态改为完成
        printProcesses(scheduler);
        // 更新其他所有进程的等待时间和响应比
        for (int j = 0; j < scheduler->processCount; j++) {
                PCB* otherProcess = &scheduler->processes[j];
                if(i != j){
                    otherProcess->waitingTime += requiredTime;
                }
                // 重新计算响应比
                if (otherProcess->requiredTime != 0) {
                    otherProcess->responseRatio = (otherProcess->waitingTime + otherProcess->requiredTime) / otherProcess->requiredTime;
                } else {
                    // 如果所需时间为0，设置响应比为一个特定的值，或者直接跳过这个进程
                    otherProcess->responseRatio = -1;  // 或者其他您认为合适的值
                }


        }
        double schedulingTime = difftime(time(NULL), process->startTime);  // 计算调度时间
        scheduler->totalSchedulingTime += schedulingTime;  // 添加到总的调度时间
    }
    // 打印所有进程的状态
    printf("平均调度时间: %.2f\n", scheduler->totalSchedulingTime / scheduler->processCount);  // 计算并打印平均调度时间
}


#endif //PROCESSSCHEDULING_SCHEDULER_H