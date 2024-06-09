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
    PCB processes[MAX];  // �洢���еĽ���
    int processCount;  // ��������
    double timeSlice;  // ʱ��Ƭ����
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

// ��ӡ���н��̵�״̬
void printProcesses(Scheduler* scheduler) {
    printf("=============================================================================\n");
    for (int i = 0; i < scheduler->processCount; i++) {
        PCB process = scheduler->processes[i];
        printf("��������: %s, ״̬: %s, ������ʱ��: %.2f, ����ʱ��: %.2f, ��Ӧ��: %.2f\n",
               process.processName, process.processState, process.elapsedTime, process.requiredTime, process.responseRatio);
    }
}
int comparePCB(const void* a, const void* b) {
    PCB* pcbA = (PCB*)a;
    PCB* pcbB = (PCB*)b;
    return pcbA->requiredTime - pcbB->requiredTime;
}
// �����ҵ���ȵ����㷨
void schedule_shortest_job_first(Scheduler* scheduler) {
    // �Խ����б�����������ʱ���������
    qsort(scheduler->processes, scheduler->processCount, sizeof(PCB), comparePCB);

    // ���������Ľ����б���ÿ�����̽��е���
    for (int i = 0; i < scheduler->processCount; i++) {
        PCB* process = &scheduler->processes[i];
        if (strcmp(process->processState, "W") == 0 || strcmp(process->processState, "R") == 0) {
            strcpy(process->processState, "R");  // ������״̬��Ϊ����
            process->startTime = time(NULL);  // ��¼��ʼʱ��
            while (process->requiredTime > scheduler->timeSlice) {
                process->elapsedTime += scheduler->timeSlice;  // ����������ʱ��
                process->requiredTime -= scheduler->timeSlice;
                usleep(scheduler->timeSlice*1000);
            }
            strcpy(process->processState, "F");  // ������״̬��Ϊ���
            double schedulingTime = difftime(time(NULL), process->startTime);  // �������ʱ��
            scheduler->totalSchedulingTime += schedulingTime;  // ��ӵ��ܵĵ���ʱ��
            process->elapsedTime += process->requiredTime;
            process->requiredTime = scheduler->finishTime;
        }
        printProcesses(scheduler);
    }
    printf("ƽ������ʱ��: %.2f\n", scheduler->totalSchedulingTime / scheduler->processCount);  // ���㲢��ӡƽ������ʱ��
}

// ʱ��Ƭ��ת�����㷨
void schedule_round_robin(Scheduler* scheduler) {
    // ʹ�ö������洢���̵�ָ�룬�Ա���ÿ��ʱ��Ƭ�����󽫵�ǰ�����ƶ������е�ĩβ
    PCB* processQueue[MAX];
    int front = 0, rear = scheduler->processCount;
    for (int i = 0; i < scheduler->processCount; i++) {
        processQueue[i] = &scheduler->processes[i];
    }

    while (front != rear) {
        PCB* process = processQueue[front];  // ��ȡ�����еĵ�һ������

        if (strcmp(process->processState, "W") == 0 || strcmp(process->processState, "R") == 0) {
            strcpy(process->processState, "R");  // ������״̬��Ϊ����
            process->startTime = time(NULL);  // ��¼��ʼʱ��
            if (process->requiredTime > scheduler->timeSlice) {
                process->requiredTime -= scheduler->timeSlice;
                usleep(scheduler->timeSlice*1000);
                process->elapsedTime += scheduler->timeSlice;
            } else {
                process->elapsedTime += process->requiredTime;
                process->requiredTime = scheduler->finishTime;
                strcpy(process->processState, "F");  // ������״̬��Ϊ���
            }
            double schedulingTime = difftime(time(NULL), process->startTime);  // �������ʱ��
            scheduler->totalSchedulingTime += schedulingTime;  // ��ӵ��ܵĵ���ʱ��
            if (strcmp(process->processState, "F") != 0) {
                processQueue[rear++] = process;  // ������̻�û����ɣ������ƶ������е�ĩβ
                if(rear > MAX) {
                    rear%=MAX;
                }
            }
        }

        front++;
        if(front>MAX){
            front%=MAX;
        }// �Ƴ������еĵ�һ������
        printProcesses(scheduler);
    }
    printf("ƽ������ʱ��: %.2f\n", scheduler->totalSchedulingTime / scheduler->processCount);  // ���㲢��ӡƽ������ʱ��
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
// ����Ӧ�����ȵ����㷨
void schedule_highest_response_ratio_next(Scheduler* scheduler) {
    for (int i = 0; i < scheduler->processCount; i++) {
        PCB* process = &scheduler->processes[i];
        // ����ȴ�ʱ��
        double waitingTime = process->waitingTime;

        // ������Ӧ��
        process->responseRatio = (waitingTime + process->requiredTime) / process->requiredTime;
    }



    // ���������Ľ����б���ÿ�����̽��е���
    for (int i = 0; i < scheduler->processCount; i) {
        // �Խ����б�����Ӧ�Ƚ�������
        qsort(scheduler->processes, scheduler->processCount, sizeof(PCB), comparePCBResponseRatio);
        PCB* process = &scheduler->processes[i];
        if (strcmp(process->processState, "F") == 0){
            break;
        }
        process->startTime = time(NULL);  // ��¼��ʼʱ��
        int requiredTime = process->requiredTime;
        strcpy(process->processState, "R");  // ������״̬��Ϊ����
        process->elapsedTime += process->requiredTime;
        process->requiredTime = scheduler->finishTime;
        usleep(requiredTime * 1000);  // ģ���������
        strcpy(process->processState, "F");  // ������״̬��Ϊ���
        printProcesses(scheduler);
        // �����������н��̵ĵȴ�ʱ�����Ӧ��
        for (int j = 0; j < scheduler->processCount; j++) {
                PCB* otherProcess = &scheduler->processes[j];
                if(i != j){
                    otherProcess->waitingTime += requiredTime;
                }
                // ���¼�����Ӧ��
                if (otherProcess->requiredTime != 0) {
                    otherProcess->responseRatio = (otherProcess->waitingTime + otherProcess->requiredTime) / otherProcess->requiredTime;
                } else {
                    // �������ʱ��Ϊ0��������Ӧ��Ϊһ���ض���ֵ������ֱ�������������
                    otherProcess->responseRatio = -1;  // ������������Ϊ���ʵ�ֵ
                }


        }
        double schedulingTime = difftime(time(NULL), process->startTime);  // �������ʱ��
        scheduler->totalSchedulingTime += schedulingTime;  // ��ӵ��ܵĵ���ʱ��
    }
    // ��ӡ���н��̵�״̬
    printf("ƽ������ʱ��: %.2f\n", scheduler->totalSchedulingTime / scheduler->processCount);  // ���㲢��ӡƽ������ʱ��
}


#endif //PROCESSSCHEDULING_SCHEDULER_H