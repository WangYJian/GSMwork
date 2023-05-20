#pragma once
#include <cmath>
#include "../QuadTree.h"


// 重载小于运算符
bool Point::operator<(const Point &other) const {
    if (x != other.x) {
        return x < other.x;
    }
    return y < other.y;
}

//查看当前是否需要改变链接
bool MovePoint::ChangeLink(QuadTreeNode* root) {
    //获取当前信号
    double NowPower = Link.type == 0 ? 0 : Signal(*this, Link);
    //如果信号存在且小于1，断开链接
    if (NowPower != 0 && NowPower < 1) {
        Link = StationPoint(0, 0, 0, 0, 0);
        return true;
    }
    //获取最大信号基站
    double MaxPower = 0;
    StationPoint MaxLink = root->FindStrongestPoints(*this, 1, MaxPower);
    if (NowPower > MaxPower) {
        MaxPower = NowPower;
    }

    //如果最大信号大于当前信号一定范围，改变链接
    if (MaxPower - NowPower > 1) {
        Link = MaxLink;
        return true;
    } else {
        return false;
    }
}

//根据当前时间移动
StationPoint MovePoint::Move(double Time, QuadTreeNode* root) {
    //计算移动距离
    double Distance = speed * Time;
    //计算移动方向
    double Angle = atan2(endPlace.y - y, endPlace.x - x);
    //计算移动后坐标
    x += Distance * cos(Angle);
    y += Distance * sin(Angle);
    //更新时间
    NowTime = NowTime+Time;
    //如果到达终点，改变终点，并更新状态
    if (Distance > distance(*this, endPlace)) {
        x = endPlace.x;
        y = endPlace.y;
        UpdateStatus();
    }
    //检查是否需要改变链接
    if(ChangeLink(root)) {
        return Link;
    } else {
        return StationPoint(0, 0, -1, 0, 0);
    }
}

//更新状态
void MovePoint::UpdateStatus() {
    head = head->next;
    if (head != nullptr) {
        x = head->beginPlace.x;
        y = head->beginPlace.y;
        endPlace = head->endPlace;
        speed = head->speed;
        NowTime = head->beginTime;
    } else {
        speed = 0;
    }
}

//插入规则
void Rule::Insert(Rule *rule) {
    if (next == nullptr) {
        next = rule;
    } else {
        next->Insert(rule);
    }
}

void FakePoint::Move(double Time, QuadTreeNode *root) {
    //计算移动距离
    double Distance = Time > 0 ? speed * Time : 0;
    //计算移动方向
    double Angle = atan2(endPlace.y - y, endPlace.x - x);
    //计算移动后坐标
    x += Distance * cos(Angle);
    y += Distance * sin(Angle);
    //更新时间
    NowTime = Time > 0 ? NowTime+Time : NowTime;
    //如果到达终点，改变终点，并更新状态
    if (Distance > distance(*this, endPlace)) {
        x = endPlace.x;
        y = endPlace.y;
        speed = 0;
    }
}




