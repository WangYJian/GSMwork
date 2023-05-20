#pragma once
#include "Point.h"
#include <queue>
#include <set>


using namespace std;

// 定义四叉树的节点类
class QuadTreeNode {
public:
    int x_min, x_max, y_min, y_max; // 节点代表的矩形区域的坐标范围
    QuadTreeNode *children[4]{};    // 四个子节点, 0:左上, 1:右上, 2:左下, 3:右下
    QuadTreeNode *parent{}; // 父母结点
    StationPoint *point;   // 节点上存储的点
    set<int> type; //存储该区域内的点的类型
    void Split(); // 分裂该节点，将该节点的四个子节点创建出来
    bool IntersectsCircle(const MovePoint &target, double r);   //判断目标圆是否与边界相交
    void FindNearestPointsHelper(const MovePoint &target, int k, priority_queue<pair<double, StationPoint>> &heap, int type, int id = 0);
    QuadTreeNode(int _x_min, int _x_max, int _y_min, int _y_max);   // 构造函数，用于初始化QuadTreeNode类的对象
    ~QuadTreeNode();    // 析构函数，用于删除QuadTreeNode类的对象及其子节点和点
    void Insert(StationPoint *p); // 插入一个点到四叉树中
    int Print() const; // 输出以该节点为根节点的四叉树中的所有点的坐标
    bool is_leaf(); //判断该结点是否是叶子
    vector<StationPoint> FindNearestPoints(const MovePoint &target, int k, int id = 0); //找到离某个点最近的几个基站(三种类型基站分别找k个)
    StationPoint FindStrongestPoints(const MovePoint &target, int k, double &signal, int id = 0);  //找到最近k个点中信号最强的一个点
    QuadTreeNode* Child(int i); //获取第i个孩子结点
    QuadTreeNode* Parent(); //获取父节点
    StationPoint* Area(MovePoint p, int id = 0); //通过点的位置找到对应的区块较近点对应类型的点
    vector<StationPoint*> GetPoints(); //获取当前结点存储的所有点
    StationPoint* is_cover(MovePoint target, double &signal); //目标是否被信号覆盖

};

double distance(const Point &p1, const Point &p2);  //计算两个坐标点的距离
double Signal(MovePoint target, StationPoint point1);  //计算基站到目标点处的信号强度