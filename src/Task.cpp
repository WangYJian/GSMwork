#pragma once
#include <iostream>
#include <vector>
#include "../Task.h"

using namespace std;

// 显示首个分块/西北角区域中的基站数据
void Task1_2(QuadTreeNode* root) {
    while (root->Child(0)) {
        root = root->Child(0);
    }
    cout << "西北角区域中的基站数据" << endl;
    root->Print();
}

// 显示最后一个分块/东南角区域中的基站数据
void Task1_3(QuadTreeNode* root) {
    while (root->Child(3)) {
        root = root->Child(3);
    }
    cout << "东南角区域中的基站数据" << endl;
    root->Print();
}

// 显示出西北角树叶的东侧树叶基站数据
void Task2_1(QuadTreeNode* root) {
    while (root->Child(0)) {
        root = root->Child(0);
    }
    cout << "西北角树叶的东侧树叶基站数据" << endl;
    root->Parent()->Child(1)->Print();
}

// 西北角树叶的南侧相邻树叶的基站数据
void Task2_2(QuadTreeNode* root) {
    while (root->Child(0)) {
        root = root->Child(0);
    }
    cout << "西北角树叶的南侧相邻树叶的基站数据" << endl;
    if(!root->Parent()->Child(2)->Print()) {
        cout << "没有基站" << endl;
    }
}

// 东南角所在树叶的西北侧基站数据
void Task2_3(QuadTreeNode* root) {
    while (root->Child(3)) {
        root = root->Child(3);
    }
    cout << "东南角所在树叶的西北侧基站数据" << endl;
    root->Parent()->Child(0)->Print();
}

// 东南角西北侧的北侧相邻分块/树叶的基站数据
void Task2_4(QuadTreeNode* root) {
    while (root->Child(3)) {
        root = root->Child(3);
    }
    QuadTreeNode* result = root->Parent()->Parent()->Child(1)->Child(2);
    cout << "东南角西北侧的北侧相邻分块/树叶的基站数据" << endl;
    result->Print();
}

//信号最强基站
void Task3(QuadTreeNode* root, MovePoint target) {
    double signal;
    StationPoint result = root->FindStrongestPoints(target, 1, signal);
    cout << result << endl;
}

//查询基站信号是否覆盖
void Task4(QuadTreeNode* root, MovePoint target) {
    double signal;
    if (root->is_cover(target, signal)) {
        cout << target << "没有被信号覆盖" << endl;
    } else {
        cout << target << "被信号覆盖" << endl;
    }
}

//移动目标
void Task5(QuadTreeNode* root, MovePoint target) {
    int i = 0;
    while (true) {
        switch (target.Move(1 , root).type) {
            case -1: {
                break;
            }
            case 0: {
                printf("%.2fs (%.2f,%.2f) 断开连接\n", target.NowTime, target.x, target.y);
                i++;
                break;
            }
            default : {
                printf("%.2fs (%.2f,%.2f) 连接到基站 (%.0f,%.0f) 编号 %d\n", target.NowTime, target.x, target.y, target.Link.x, target.Link.y, target.Link.id);
                i++;
                break;
            }
        }
        if (target.head == nullptr && target.speed == 0) {
            break;
        }
    }
    cout << "断开连接次数：" << i << endl;
}

//分析移动轨迹
void ETask1(QuadTreeNode* root, MovePoint target) {
    while (target.head->id == 1) {
        switch (target.Move(1 , root).type) {
            case -1: {
                break;
            }
            case 0: {
                printf("%.2fs (%.2f,%.2f) 断开连接\n", target.NowTime, target.x, target.y);
                break;
            }
            default : {
                printf("%.2fs (%.2f,%.2f) 连接到基站 (%.0f,%.0f) 编号 %d\n", target.NowTime, target.x, target.y, target.Link.x, target.Link.y, target.Link.id);
                break;
            }
        }
        if (target.head == nullptr && target.speed == 0) {
            break;
        }
    }
}

//分析移动轨迹
void ETask2(QuadTreeNode* root, MovePoint target) {
    bool flag = false;
    StationPoint temp1(0, 0, 0, 0, 0), temp2(0, 0, 0, 0, 0);
    while (target.head->id <= 6) {
        target.Move(1 , root).type;
        //当目标有连接基站且最强基站不是目标连接基站时，进入基站重叠区
        double signal;
        double NowSignal = Signal(target, target.Link);
        auto MaxSignal = root->FindStrongestPoints(target, 1, signal);
        if (target.Link.type != 0 && target.Link.id != MaxSignal.id && !flag) {
            printf("%.2fs (%.2f,%.2f) 进入基站重叠区, 编号分别为%d %d\n", target.NowTime, target.x, target.y, target.Link.id, MaxSignal.id);
            temp1 = target.Link;
            temp2 = MaxSignal;
            flag = true;
        }
        // 当到达两个任意目标的信号低于1，退出基站重叠区
        if (flag && (Signal(target, temp1) < 1 || Signal(target, temp2) < 1)) {
            printf("%.2fs (%.2f,%.2f) 退出基站重叠区\n", target.NowTime, target.x, target.y);
            flag = false;
        }
        if (target.head == nullptr && target.speed == 0) {
            break;
        }
    }
}