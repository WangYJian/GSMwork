#pragma once
#include <iostream>
#include <cmath>
#include <queue>
#include "../QuadTree.h"


using namespace std;
// 计算距离
double distance(const Point &p1, const Point &p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return sqrt(dx * dx + dy * dy);
}

// 计算信号强度
double Signal(MovePoint target, StationPoint point1) {
    //选择不同类型
    switch (point1.type) {
        case 1: {
            return point1.power * pow((300 / distance(target, point1)), 2);
        }
        case 2: {
            return point1.power * pow((1000 / distance(target, point1)), 2);
        }
        case 3: {
            return point1.power * pow((5000 / distance(target, point1)), 2);
        }
    }
}

// 构造函数，用于初始化QuadTreeNode类的对象
QuadTreeNode::QuadTreeNode(int _x_min, int _x_max, int _y_min, int _y_max) {
    // 初始化节点代表的矩形区域的坐标范围
    x_min = _x_min;
    x_max = _x_max;
    y_min = _y_min;
    y_max = _y_max;
    // 初始化父节点为null
    parent = nullptr;
    // 初始化四个子节点为null
    for (auto &i: children) {
        i = nullptr;
    }
    // 初始化节点上存储的点为null
    point = nullptr;
}

// 析构函数，用于删除QuadTreeNode类的对象及其子节点和点
QuadTreeNode::~QuadTreeNode() {
    // 删除四个子节点
    for (auto &i: children) {
        delete i;
    }
    // 删除节点上存储的点
    delete point;
}

//插入基站
void QuadTreeNode::Insert(StationPoint *p) {
    // 如果点不在节点代表的矩形区域内，直接返回
    if (p->x < x_min || p->x > x_max || p->y < y_min || p->y > y_max) {
        return;
    }
    // 如果节点上没有点和子节点，直接存储该点
    if (!(point || children[0])) {
        point = p;
        type.insert(p->type);
        return;
    }
    // 如果节点上已经有点，且该节点没有子节点，就分裂该节点
    if (!children[0]) {
        Split();
    }
    // 将该点插入到子节点中
    for (auto &i: children) {
        i->Insert(p);
    }
}

// 分裂该节点，将该节点的四个子节点创建出来
void QuadTreeNode::Split() {
    int x_mid = (x_min + x_max) / 2;
    int y_mid = (y_min + y_max) / 2;
    children[0] = new QuadTreeNode(x_min, x_mid, y_min, y_mid);
    children[1] = new QuadTreeNode(x_mid + 1, x_max, y_min, y_mid);
    children[2] = new QuadTreeNode(x_min, x_mid, y_mid + 1, y_max);
    children[3] = new QuadTreeNode(x_mid + 1, x_max, y_mid + 1, y_max);
    // 设定父节点
    for (auto &i: children) {
        i->parent = this;
    }
    // 将该节点上存储的点插入到子节点中
    for (auto &i: children) {
        i->Insert(point);
    }
    // 清空该节点上存储的点
    point = nullptr;
}

// 输出以该节点为根节点的四叉树中的所有点的坐标
int QuadTreeNode::Print() const {
    int ALL = 0;
    if (point) {
        cout << "(" << point->x << ", " << point->y << ")" << endl;
        return 1;
    } else {
        for (auto i: children) {
            if (i) {
                ALL += i->Print();
            }
        }
    }
    return ALL;

}

//判断该结点是否是叶子
bool QuadTreeNode::is_leaf() {
    return !children[0];
}

//判断目标圆是否与边界相交
bool QuadTreeNode::IntersectsCircle(const MovePoint &target, double r) {
    // 判断圆心是否在正方形内部或边界上
    if (target.x >= x_min && target.x <= x_max && target.y >= y_min && target.y <= y_max) {
        return true;
    }

    // 计算圆心到区域中点的距离
    double dx = abs(target.x - (x_min + x_max) / 2.0);
    double dy = abs(target.y - (y_min + y_max) / 2.0);
    auto D = sqrt(dx * dx + dy * dy);

    //计算区域中心到角的距离
    auto L = sqrt((x_max - x_min) * (x_max - x_min) + (y_max - y_min) * (y_max - y_min)) / 2.0;

    // 判断是否与圆相交
    if (D <= L + r) {
        return true;
    }

    return false;
}

void QuadTreeNode::FindNearestPointsHelper(const MovePoint &target, int k, priority_queue<pair<double, StationPoint>> &heap, int type, int id) {
    // 如果当前节点是叶子节点，返回
    if (is_leaf()) {
        // 查看该结点是否有对应类型的基站, 同时不是指定id
        if (!point || type != point->type || id == point->id) {
            return;
        }
        // 计算当前节点到目标点的距离
        double d = distance(*point, target);
        // 如果堆中元素不足k个或者距离小于堆顶元素的距离，将当前点加入堆中
        if (heap.size() < k || d < heap.top().first) {
            heap.emplace(d, *point);
            // 如果堆的大小超过k，弹出堆顶元素
            if (heap.size() > k) {
                heap.pop();
            }
        }
        return;
    }

    // 如果不是叶子节点，遍历每个子节点，如果子节点和目标圆相交，同时具有同类型基站，递归查找
    for (auto &child: children) {
        if (child && child->IntersectsCircle(target, heap.top().first) && child->type.find(type) != child->type.end()) {
            child->FindNearestPointsHelper(target, k, heap, type, id);
        }
    }
}

//找到离某个点最近的几个基站(三种类型基站分别找k个)
vector<StationPoint> QuadTreeNode::FindNearestPoints(const MovePoint &target, int k, int id) {
    priority_queue<pair<double, StationPoint>> heap;
    vector<StationPoint> result;
    auto temp = Area(target, id);
    for (int i = 1; i <= 3; i++) {
        //找到区块中较近的点
        //cout << *temp << endl;
        //通过类型伸缩半径距离
        auto r = distance(target, *temp);
        if (temp->type == 1 && i == 2) {
            r = r * 100 / 9;
        } else if (temp->type == 1 && i == 3) {
            r = r * 2500 / 9;
        } else if (temp->type == 2 && i == 3) {
            r = r * 25;
        }
        for (int i = 0; i < k; i++) {
            heap.emplace(r, *temp);
        }
        FindNearestPointsHelper(target, k, heap, i, id);
        while (!heap.empty()) {
            result.push_back(heap.top().second);
            heap.pop();
        }
    }
    return result;
}

//找到最近k个点中信号最强的一个点, 可以忽略掉制定点
StationPoint QuadTreeNode::FindStrongestPoints(const MovePoint &target, int k, double &signal, int id) {
    //获取最近的k个点
    auto result = FindNearestPoints(target, k, id);
    //计算强度, 保留最大点
    double max = 0;
    StationPoint maxPoint(0,0,0,0,0);
    for (auto i: result) {
        signal = Signal(target, i);
        if (signal > max) {
            max = signal;
            maxPoint = i;
        }
    }
    signal = max;
    return maxPoint;
}

//获取第i个孩子结点
QuadTreeNode* QuadTreeNode::Child(int i) {
    return children[i];
}

//获取父节点
QuadTreeNode* QuadTreeNode::Parent() {
    return parent;
}

//通过点的位置找到对应的区块较近对应类型点点
StationPoint* QuadTreeNode::Area(MovePoint p, int id) {
    // 如果当前节点是叶子节点，查看该结点是否有对应类型的基站
    if (is_leaf()) {
        if (id != 0) {

        }
        if (!point || point->id == id) {
            return nullptr;
        }
        return point;
    }
    // 优先返回区域内的点
    for (auto &i: children) {
        if (i->x_max >= p.x && i->x_min <= p.x && i->y_max >= p.y && i->y_min <= p.y) {
            StationPoint* temp = i->Area(p, id);
            if (temp) {
                return temp;
            }
        }
    }
    // 如果区域内没有点，返回区域外的点
    for (auto &i: children) {
        if (i->x_max < p.x || i->x_min > p.x || i->y_max < p.y || i->y_min > p.y) {
            StationPoint* temp = i->Area(p, id);
            if (temp) {
                return temp;
            }
        }
    }
}

//目标是否被信号覆盖
StationPoint* QuadTreeNode::is_cover(MovePoint target, double &signal) {
    auto Point =  FindStrongestPoints(target, 1, signal);
    StationPoint* p = &Point;
    if (signal < 1) {
        return nullptr;
    } else {
        return p;
    }
}

//获取当前结点所有点
vector<StationPoint*> QuadTreeNode::GetPoints() {
    vector<StationPoint*> result;
    if (is_leaf()) {
        if (point) {
            result.push_back(point);
        }
        return result;
    }
    for (auto &i: children) {
        if (i) {
            auto temp = i->GetPoints();
            result.insert(result.end(), temp.begin(), temp.end());
        }
    }
    return result;
}

