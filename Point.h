#pragma once
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;



// 定义一个二维坐标点类
class Point {
public:
    double x, y; //坐标
    Point(double _x, double _y) : x(_x), y(_y){} // 构造函数，用于初始化Point类的对象
    bool operator<(const Point &other) const; // 重载小于运算符
    friend ostream& operator<<(ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};


//派生基站
class StationPoint : public Point {
public:
    int type, id; //类型(1:城区 2:乡镇 3:高速)，编号
    double power;   //功率
    double r; //覆盖半径
    StationPoint(double _x = 0, double _y = 0, int _type = 0, double _power = 0, int _id = 0) : Point(_x, _y),type(_type), id(_id), power(_power) {
        switch (type) {
            case 1:
                r = sqrt(power) * 300;
                break;
            case 2:
                r = sqrt(power) * 1000;
                break;
            case 3:
                r = sqrt(power) * 5000;
                break;
            default:
                r = 0;
                break;
        }
    }
};

class QuadTreeNode;

// 定义移动规则
class Rule {
public:
    int id; //编号
    Point beginPlace, endPlace; //起始位置，终止位置
    double speed; //速度
    double beginTime; //开始时间
    Rule *next; //下一条规则
    Rule(int _id, Point _beginPlace, Point _endPlace, double _speed, double _beginTime) : beginPlace(_beginPlace), endPlace(_endPlace), speed(_speed), beginTime(_beginTime), next(nullptr), id(_id) {}
    void Insert(Rule *rule); //插入规则
};


//伪基站移动点
class FakePoint : public Point {
public:
    int id; //编号
    Point endPlace = Point(0, 0);
    double speed;
    double NowTime;
    FakePoint(Rule *rule, int _id) : Point(rule->beginPlace.x, rule->beginPlace.y), id(_id) {
        endPlace = rule->endPlace;
        speed = rule->speed;
        NowTime = rule->beginTime;
    }
    void Move(double Time, QuadTreeNode* root); //根据当前时间移动
};

//派生移动点
class MovePoint : public Point {
public:
    StationPoint Link = StationPoint(0, 0, 0, 0, 0); //当前链接的基站
    Point endPlace = Point(0, 0);
    Rule *head, *rule;
    double speed;
    double NowTime;

    MovePoint(double _x, double _y) : Point(_x, _y){}
    MovePoint(Rule *rule) : Point(rule->beginPlace.x, rule->beginPlace.y), head(rule), rule(rule) {
        endPlace = rule->endPlace;
        speed = rule->speed;
        NowTime = rule->beginTime;
    }
    bool ChangeLink(QuadTreeNode* root); //查看当前是否需要改变链接
    StationPoint Move(double Time, QuadTreeNode* root); //根据当前时间移动
    void UpdateStatus(); //更新状态
};