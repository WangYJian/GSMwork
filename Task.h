#pragma once
#include "QuadTree.h"



void Task1_2(QuadTreeNode* root); // 显示首个分块/西北角区域中的基站数据
void Task1_3(QuadTreeNode* root); // 显示最后一个分块/东南角区域中的基站数据
void Task2_1(QuadTreeNode* root); // 显示出西北角树叶的东侧树叶基站数据
void Task2_2(QuadTreeNode* root); // 西北角树叶的南侧相邻树叶的基站数据
void Task2_3(QuadTreeNode* root); // 东南角所在树叶的西北侧基站数据
void Task2_4(QuadTreeNode* root); // 西北侧的北侧相邻分块/树叶的基站数据
void Task3(QuadTreeNode* root, MovePoint target); //信号最强基站
void Task4(QuadTreeNode* root, MovePoint target); //查询基站信号是否覆盖
void Task5(QuadTreeNode* root, MovePoint target); //移动时基站信号切换情况
void ETask1(QuadTreeNode* root, MovePoint target); // 分析移动轨迹
void ETask2(QuadTreeNode* root, MovePoint target); // 分析移动轨迹