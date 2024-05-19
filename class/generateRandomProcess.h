//
// Created by 15517 on 24-5-19.
//

#ifndef PROCESSSCHEDULING_GENERATERANDOMPROCESS_H
#define PROCESSSCHEDULING_GENERATERANDOMPROCESS_H
#include <random>
#include <chrono>
#include "PCB.h"

class  generateRandomProcess {


public:
    PCB generateRandomProcessWithCount(int processCount) {
        // 创建一个随机数生成器
        std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> distribution(1, 1000);  // 修改参数为(1, 1000)

        // 生成随机的到达时间、所需时间和优先级
        int arrivalTime = processCount == 0 ? 0 : distribution(generator);
        int requiredTime = distribution(generator);
        int priority = distribution(generator);

        // 创建一个新的PCB对象
        PCB process;
        process.arrivalTime = std::chrono::system_clock::now() + std::chrono::milliseconds(arrivalTime);
        process.requiredTime = std::chrono::milliseconds(requiredTime);
        process.priority = priority;
        process.elapsedTime = std::chrono::milliseconds(0);
        process.processState = "W";
        process.processName = "Process " + std::to_string(processCount + 1);  // 设置进程名

        return process;
    }
};


#endif //PROCESSSCHEDULING_GENERATERANDOMPROCESS_H
