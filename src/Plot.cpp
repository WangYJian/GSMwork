#pragma once
#include <opencv2/opencv.hpp>
#include "../Plot.h"

using namespace cv;

// 绘制基站
void PlotMap::PlotStation(QuadTreeNode* root) {
    // 绘制当前结点的分块
    rectangle(image, cv::Point((root->x_min - x_min) * times, (root->y_max - y_min) * times), cv::Point((root->x_max - x_min) * times, (root->y_min - y_min) * times), Scalar(128, 128, 128, 200), 1);
    // 如果当前结点有点，绘制点，当times>=0.2时显示坐标
    if (root->point != nullptr) {
        //不同的基站用不同的颜色表示
        if (root->point->type == 1) {
            circle(image, cv::Point((root->point->x - x_min) * times, (root->point->y - y_min) * times), 2, Scalar(255, 0, 0), -1, LINE_AA);
        }
        else if (root->point->type == 2) {
            circle(image, cv::Point((root->point->x - x_min) * times, (root->point->y - y_min) * times), 2, Scalar(0, 255, 0), -1, LINE_AA);
        }
        else if (root->point->type == 3) {
            circle(image, cv::Point((root->point->x - x_min) * times, (root->point->y - y_min) * times), 2, Scalar(0, 0, 255), -1, LINE_AA);
        }
        if (times >= 0.2) {
            putText(image, "[" + to_string(int(root->point->x)) + "," + to_string(int(root->point->y)) + "]", cv::Point((root->point->x - x_min) * times, (root->point->y - y_min) * times), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 0), 1, LINE_AA);
        }
    }
    // 如果当前结点有子结点，递归绘制子结点
    for (auto &i: root->children) {
        if (i != nullptr) {
            PlotStation(i);
        }
    }
}

// 绘制移动点
void PlotPoint::PlotMovePoint(MovePoint &movePoint) {
    // 绘制点，当times>=0.2时显示坐标
    circle(image, cv::Point((movePoint.x - x_min) * times, (movePoint.y - y_min) * times), 2, Scalar(0, 255, 0), -1, LINE_AA);
    if (times >= 0.2) {
        putText(image, "[" + to_string(int(movePoint.x)) + "," + to_string(int(movePoint.y)) + "]", cv::Point((movePoint.x - x_min) * times, (movePoint.y - y_min) * times), FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0, 0, 0), 1, LINE_AA);
    }

    PlotMovePointHelper(movePoint, root);
}
void PlotPoint::PlotMovePointHelper(MovePoint &movePoint, QuadTreeNode *Node) {
    // 绘制当前结点的分块和点
    rectangle(image, cv::Point((Node->x_min - x_min) * times, (Node->y_max - y_min) * times), cv::Point((Node->x_max - x_min) * times, (Node->y_min - y_min) * times), Scalar(128, 128, 128, 200), 1);

    // 如果当前结点有点，绘制点，当times>0.2时显示坐标
    if (Node->point != nullptr) {
        //不同的基站用不同的颜色表示
        if (Node->point->type == 1) {
            circle(image, cv::Point((Node->point->x - x_min) * times, (Node->point->y - y_min) * times), 2, Scalar(255, 0, 0), -1, LINE_AA);
        } else if (Node->point->type == 2) {
            circle(image, cv::Point((Node->point->x - x_min) * times, (Node->point->y - y_min) * times), 2,
                   Scalar(0, 255, 0), -1, LINE_AA);
        } else if (Node->point->type == 3) {
            circle(image, cv::Point((Node->point->x - x_min) * times, (Node->point->y - y_min) * times), 2,
                   Scalar(0, 0, 255), -1, LINE_AA);
        }
        if (times >= 0.2) {
            putText(image, "[" + to_string(int(Node->point->x)) + "," + to_string(int(Node->point->y)) + to_string(Node->point->id) + "]",
                    cv::Point((Node->point->x - x_min) * times, (Node->point->y - y_min) * times), FONT_HERSHEY_SIMPLEX,
                    0.3, Scalar(0, 0, 0), 1, LINE_AA);
        }
    }

    // 如果不是叶子节点，遍历每个子节点，如果子节点和目标圆相交，同时具有同类型基站，递归查找
    for (auto &child: Node->children) {
        if (child && child->IntersectsCircle(movePoint, x_max)) {
            PlotMovePointHelper(movePoint, child);
        }
    }
}

// 绘制移动点的移动轨迹
void PlotMap::PlotPath(MovePoint &movePoint) {
    Rule *cur = movePoint.rule;
    while (cur) {
        line(image, cv::Point((cur->beginPlace.x - x_min) * times, (cur->beginPlace.y - y_min) * times), cv::Point((cur->endPlace.x - x_min) * times, (cur->endPlace.y - y_min) * times), Scalar(0, 0, 255), 1, LINE_AA);
        cur = cur->next;
    }
}

// 强调某个区块
void PlotMap::PlotStress(QuadTreeNode *Node) {
    rectangle(image, cv::Point((Node->x_min - x_min) * times, (Node->y_max - y_min) * times), cv::Point((Node->x_max - x_min) * times, (Node->y_min - y_min) * times), Scalar(255, 0, 0, 200), 1);
}

// 绘制移动点与基站的连线
void PlotPoint::PlotLink(MovePoint &movePoint, StationPoint LinkStation, int type) {
    // 绘制点，当times>=0.2时显示坐标
    circle(image, cv::Point((movePoint.x - x_min) * times, (movePoint.y - y_min) * times), 2, Scalar(0, 255, 0), -1,
           LINE_AA);
    if (times >= 0.2) {
        putText(image, "[" + to_string(int(movePoint.x)) + "," + to_string(int(movePoint.y)) + "]",
                cv::Point((movePoint.x - x_min) * times, (movePoint.y - y_min) * times), FONT_HERSHEY_SIMPLEX, 0.3,
                Scalar(0, 0, 0), 1, LINE_AA);
    }
    // 绘制基站
    circle(image, cv::Point((LinkStation.x - x_min) * times, (LinkStation.y - y_min) * times), 2, Scalar(255, 0, 0), -1,
           LINE_AA);
    if (times >= 0.2) {
        putText(image, "[" + to_string(int(LinkStation.x)) + "," + to_string(int(LinkStation.y)) + to_string(LinkStation.id) + "]",
                cv::Point((LinkStation.x - x_min) * times, (LinkStation.y - y_min) * times), FONT_HERSHEY_SIMPLEX, 0.3,
                Scalar(0, 0, 0), 1, LINE_AA);
    }
    // 绘制连线
    switch (type) {
        case 0:
            line(image, cv::Point((movePoint.x - x_min) * times, (movePoint.y - y_min) * times),
                 cv::Point((LinkStation.x - x_min) * times, (LinkStation.y - y_min) * times), Scalar(0, 0, 255), 1,
                 LINE_AA);
            break;
        case 1:
            line(image, cv::Point((movePoint.x - x_min) * times, (movePoint.y - y_min) * times),
                 cv::Point((LinkStation.x - x_min) * times, (LinkStation.y - y_min) * times), Scalar(0, 255, 0), 1,
                 LINE_AA);
            break;
        case 2:
            line(image, cv::Point((movePoint.x - x_min) * times, (movePoint.y - y_min) * times),
                 cv::Point((LinkStation.x - x_min) * times, (LinkStation.y - y_min) * times), Scalar(255, 0, 0), 1,
                 LINE_AA);
            break;
        default:
            break;
    }
    //在连线平行处写信号强度
    putText(image, to_string(Signal(movePoint,LinkStation)), cv::Point(((movePoint.x - x_min) * times + (LinkStation.x - x_min) * times) / 2,
                                                                       ((movePoint.y - y_min) * times + (LinkStation.y - y_min) * times) / 2), FONT_HERSHEY_SIMPLEX, 0.3,
            Scalar(0, 0, 0), 1, LINE_AA);
    //绘制基站范围
    circle(image, cv::Point((LinkStation.x - x_min) * times, (LinkStation.y - y_min) * times), LinkStation.r * times, Scalar(255, 0, 0), 1,
           LINE_AA);
}

void PlotPoint::PlotFakePoint(FakePoint &fakePoint) {
    // 绘制粉色点，times>=0.2时显示坐标
    circle(image, cv::Point((fakePoint.x - x_min) * times, (fakePoint.y - y_min) * times), 2, Scalar(255, 0, 255), -1,
           LINE_AA);
    // 画40m范围
    circle(image, cv::Point((fakePoint.x - x_min) * times, (fakePoint.y - y_min) * times), 40 * times, Scalar(255, 0, 255), 1,
           LINE_AA);

}

