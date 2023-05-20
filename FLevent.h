#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include "FileProcessing.h"
#include "Plot.h"




//窗口基类
class Windows {
public:
    Fl_Window *windows;
    virtual void show() = 0; //显示窗口
    virtual void destroy() = 0; //销毁窗口
};


//提示窗口
class TipWindows : public Windows {
public:
    TipWindows();
    Fl_Window *windows;
    Fl_Box *box;
    void show() override;
    void fail() ;
    void destroy() override;
};


//导入文件窗口
class ImportFileWindows : public Windows {
public:
    ImportFileWindows();
    TipWindows tip; //提示窗口
    FileReader* fileReader{}; //文件读取类
    //窗口内容
    Fl_Window *windows;
    Fl_Input *input;
    Fl_Button *btn;
    void show() override;
    void destroy() override { windows->hide(); }
};

//导入窗口
class ImportWindows : public Windows{
public:
    ImportWindows();
    vector<StationPoint *> stationPoints = vector<StationPoint *>(); //基站数据
    int x_min = _MAX, x_max = _MIN, y_min = _MAX, y_max = _MIN; //基站的最大最小值
    Rule *ruleList{}; //规则数据
    vector<pair<Rule, int>> fakeRuleList = vector<pair<Rule, int>>(); //伪基站规则数据
    ImportFileWindows station; //导入基站文件窗口
    ImportFileWindows rule; //导入移动文件窗口
    ImportFileWindows fakeRule; //导入伪基站规则文件窗口
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Box *box1;
    Fl_Box *box2;
    Fl_Box *box3;
    void show() override;
    void destroy() override { windows->hide(); }
};

//绘制基站与移动窗口
class DrawWindows : public Windows{
public:
    DrawWindows(QuadTreeNode *_root, MovePoint *_movePoint);
    PlotMap *plot{}; //绘图类
    QuadTreeNode *root{}; //四叉树根节点
    QuadTreeNode *view{}; //四叉树可视化节点
    MovePoint* movePoints{}; //移动数据
    double Times = 1; //放大倍数
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Button *btn5;
    Fl_Button *btn6;
    Fl_Button *btn7;
    Fl_Button *btn8;
    Fl_Button *btn9;
    Fl_Button *btn10;
    Fl_Button *btn11;
    Fl_Box *box1;
    Fl_RGB_Image *image{};
    void show() override;
    void destroy() override { windows->hide(); }
};

//查找窗口
class SearchWindows : public Windows{
public:
    SearchWindows(QuadTreeNode* _root);
    QuadTreeNode *root{}; //四叉树根节点
    PlotPoint *plotPoint{}; //绘图类
    MovePoint *Points{}; //点数据
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Input *input1;
    Fl_Input *input2;
    Fl_Box *box1{};
    Fl_Image *image{};
    double viewTimes = 1;
    void show() override;
    void destroy() override { windows->hide(); }
};

class Task1Windows : public Windows {
public:
    Task1Windows(QuadTreeNode *root);
    QuadTreeNode *root{}; //四叉树根节点
    PlotMap *plot{}; //绘图类
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Box *box1;
    Fl_Text_Buffer *buffer;
    Fl_Text_Display *display;
    Fl_Image *image{};

    void show() override;
    void destroy() override { windows->hide(); }
};

class Task2Windows : public Windows {
public:
    Task2Windows(QuadTreeNode *root);
    QuadTreeNode *root{}; //四叉树根节点
    PlotMap *plot{}; //绘图类
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Button *btn5;
    Fl_Box *box1;
    Fl_Text_Buffer *buffer;
    Fl_Text_Display *display;
    Fl_Image *image{};

    void show() override;
    void destroy() override { windows->hide(); }
};

class Task3Windows : public Windows {
public:
    Task3Windows(QuadTreeNode *root);
    QuadTreeNode *root{}; //四叉树根节点
    PlotPoint *plot{}; //绘图类
    MovePoint *Points{}; //点数据
    StationPoint stationPoints; //当前连接基站
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Input *input1;
    Fl_Input *input2;
    Fl_Box *box1;
    Fl_Text_Buffer *buffer;
    Fl_Text_Display *display;
    Fl_Image *image{};

    double Times = 1; //放大倍数
    double signal = 0; //信号强度

    void show() override;
    void destroy() override { windows->hide(); }
};

class Task4Windows : public Windows {
public:
    Task4Windows(QuadTreeNode *root);
    Task3Windows *task3Windows{};
    void show() override;
    void destroy() override { windows->hide(); }
};

class Task5Windows : public Windows {
public:
    Task5Windows(QuadTreeNode *root, MovePoint *movePoint);
    QuadTreeNode *root{}; //四叉树根节点
    PlotPoint *plot{}; //绘图类
    MovePoint *rootMovePoint; //初始点
    MovePoint *movePoint{}; //点数据
    StationPoint stationPoints; //当前连接基站
    bool flag = true;
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Button *btn5;
    Fl_Button *btn6;
    Fl_Button *btn7;
    Fl_Input *input1;
    Fl_Input *input2;
    Fl_Box *box1;
    Fl_Text_Buffer *buffer;
    Fl_Text_Display *display;
    Fl_Image *image{};

    double Times = 1; //放大倍数
    double speedTimes = 1; //速度倍速
    double signal = 0; //信号强度

    void show() override;
    void destroy() override { windows->hide(); }

};

class ETask1Windows : public Windows {
public:
    ETask1Windows(QuadTreeNode *root, MovePoint *movePoint);
    QuadTreeNode *root{}; //四叉树根节点
    PlotPoint *plot{}; //绘图类
    MovePoint *rootMovePoint; //初始点
    MovePoint *movePoint{}; //点数据
    StationPoint stationPoints; //当前连接基站
    bool flag = true;
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Button *btn5;
    Fl_Button *btn6;
    Fl_Button *btn7;
    Fl_Button *btn8;
    Fl_Box *box1;
    Fl_Text_Buffer *buffer;
    Fl_Text_Display *display;
    Fl_Image *image{};

    double Times = 1; //放大倍数
    double speedTimes = 1; //速度倍速
    double signal = 0; //信号强度

    void show() override;
    void destroy() override { windows->hide(); }
};

class ETask2Windows : public Windows {
public:
    ETask2Windows(QuadTreeNode *root, MovePoint *movePoint);
    QuadTreeNode *root{}; //四叉树根节点
    PlotPoint *plot{}; //绘图类
    MovePoint *rootMovePoint; //初始点
    MovePoint *movePoint{}; //点数据
    StationPoint stationPoints; //当前连接基站
    bool flag = true;
    bool flag1 = false;
    StationPoint temp1{};
    StationPoint temp2{};
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Button *btn5;
    Fl_Button *btn6;
    Fl_Button *btn7;
    Fl_Box *box1;
    Fl_Text_Buffer *buffer;
    Fl_Text_Display *display;
    Fl_Image *image{};

    double Times = 1; //放大倍数
    double speedTimes = 1; //速度倍速
    double signal = 0; //信号强度

    void show() override;
    void destroy() override { windows->hide(); }
};

class PTask1Windows : public Windows {
public:
    PTask1Windows(QuadTreeNode *root, MovePoint *movePoint, vector<FakePoint> fakePoint);
    QuadTreeNode* root{}; //四叉树根节点
    PlotPoint *plot{}; //绘图类
    MovePoint *rootMovePoint; //初始点
    MovePoint *movePoint{}; //点数据
    vector<FakePoint> fakePoints{}; //伪基站数据
    vector<FakePoint> rootFake{};
    StationPoint stationPoints; //当前连接基站
    double lastDistance[4] = {-1,-1,-1,-1};
    bool flag = true;
    bool flag1 = false;
    StationPoint temp1{};
    StationPoint temp2{};
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Button *btn5;
    Fl_Button *btn6;
    Fl_Button *btn7;
    Fl_Box *box1;
    Fl_Text_Buffer *buffer;
    Fl_Text_Display *display;
    Fl_Image *image{};

    double Times = 1; //放大倍数
    double speedTimes = 1; //速度倍速
    double signal = 0; //信号强度

    void show() override;
    void destroy() override { windows->hide(); }
};

class PTask2Windows : public Windows {
public:
    PTask2Windows(QuadTreeNode *root, MovePoint *movePoint, vector<FakePoint> fakePoint);
    QuadTreeNode* root{}; //四叉树根节点
    PlotPoint *plot{}; //绘图类
    MovePoint *rootMovePoint; //初始点
    MovePoint *movePoint{}; //点数据
    vector<FakePoint> fakePoints{}; //伪基站数据
    vector<FakePoint> rootFake{};
    StationPoint stationPoints; //当前连接基站
    double lastDistance[4] = {-1,-1,-1,-1};
    bool flag = true;
    bool flag1 = false;
    StationPoint temp1{};
    StationPoint temp2{};
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    Fl_Button *btn5;
    Fl_Button *btn6;
    Fl_Button *btn7;
    Fl_Box *box1;
    Fl_Text_Buffer *buffer;
    Fl_Text_Display *display;
    Fl_Image *image{};

    double Times = 1; //放大倍数
    double speedTimes = 1; //速度倍速
    double signal = 0; //信号强度

    void show() override;
    void destroy() override { windows->hide(); }
};



//任务窗口
class TaskWindows : public Windows{
public:
    TaskWindows(QuadTreeNode *root, MovePoint *movePoint, vector<FakePoint> fakePoint);
    QuadTreeNode *root;
    MovePoint *movePoint;
    Task1Windows* task1Windows; //任务1窗口
    Task2Windows* task2Windows; //任务2窗口
    Task3Windows* task3Windows; //任务3窗口
    Task4Windows* task4Windows; //任务4窗口
    Task5Windows* task5Windows; //任务5窗口
    ETask1Windows* eTask1Windows; //扩展任务1窗口
    ETask2Windows* eTask2Windows; //扩展任务2窗口
    PTask1Windows* pTask1Windows; //升级任务1窗口
    PTask2Windows* pTask2Windows; //升级任务2窗口
    //窗口内容
    Fl_Window *windows; //窗口
    Fl_Button *Task1; //任务1
    Fl_Button *Task2; //任务2
    Fl_Button *Task3; //任务3
    Fl_Button *Task4; //任务4
    Fl_Button *Task5; //任务5
    Fl_Button *ETask1; //扩展任务1
    Fl_Button *ETask2; //扩展任务2
    Fl_Button *PTask1; //升级任务1
    Fl_Button *PTask2; //升级任务2

    void show() override;
    void destroy() override { windows->hide(); }
};

//主窗口
class MainWindows : public Windows{
public:
    MainWindows();
    QuadTreeNode *root{}; //四叉树根节点
    MovePoint* movePoints{}; //移动数据

    DrawWindows* drawWindows; //绘制窗口
    SearchWindows* searchWindows; //查找窗口
    TaskWindows* taskWindows; //任务窗口

    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    Fl_Button *btn3;
    Fl_Button *btn4;
    void show() override;
    void destroy() override { windows->hide(); }
};

//引导窗口
class IndexWindows : public Windows{
public:
    ImportWindows importWindows; //导入窗口
    MainWindows mainWindows; //主窗口
    TipWindows tipWindows; //提示窗口
    IndexWindows();
    //窗口内容
    Fl_Window *windows;
    Fl_Button *btn1;
    Fl_Button *btn2;
    void show() override;
    void destroy() override { windows->hide(); }
};