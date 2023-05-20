#pragma once
#include <opencv2/opencv.hpp>
#include "QuadTree.h"

using namespace cv;
//创建绘图对象w
class Plot {
public:
    Mat image;
    double times ; // 图像放大倍数
};

class PlotMap : public Plot {
public:
    double x_min, x_max, y_min, y_max; // 图形的坐标范围
    PlotMap(QuadTreeNode* root, int Times) : x_min(root->x_min), x_max(root->x_max), y_min(root->y_min), y_max(root->y_max) {
        // 初始化图像
        times = 0.0125 * Times;
        image = Mat::zeros((y_max - y_min) * times, (x_max - x_min) * times, CV_8UC4);
        image.setTo(Scalar(255, 255, 255, 0));
    }
    void PlotStation(QuadTreeNode* root); //绘制基站
    void PlotPath(MovePoint &movePoint); //绘制移动点的移动轨迹
    void PlotStress(QuadTreeNode* root); //强调某个区块
};

class PlotPoint : public Plot {
public:
    double x_min, x_max, y_min, y_max; // 图形的坐标范围
    QuadTreeNode *root{};
    PlotPoint(QuadTreeNode *root, MovePoint *movePoint, double Times, double window  = 1) {
        this->root = root;
        //获取宽度和高度
        double width = (root->x_max - root->x_min) / (32 * Times);
        double height = (root->y_max - root->y_min) / (32 * Times);
        //获取坐标范围
        x_min = movePoint->x - width;
        x_max = movePoint->x + width;
        y_min = movePoint->y - height;
        y_max = movePoint->y + height;
        // 初始化图像
        times = 0.2 * Times * window;
        image = Mat::zeros((y_max - y_min) * times, (x_max - x_min) * times, CV_8UC4);
        image.setTo(Scalar(255, 255, 255, 0));
    }
    void PlotMovePoint(MovePoint &movePoint); //绘制移动点
    void PlotMovePointHelper(MovePoint &movePoint, QuadTreeNode *root); //绘制移动点依赖
    void PlotLink(MovePoint &movePoint, StationPoint LinkStation, int type = 0); //绘制移动点与基站的连线
    void PlotFakePoint(FakePoint &fakePoint); //伪基站
};

