#include "../FLevent.h"
#include <FL/Fl_Window.H>
#include <string>
#include <utility>

using namespace std;
using namespace cv;

TipWindows::TipWindows() {
    windows = new Fl_Window(300, 100, "提示");
    box = new Fl_Box(0, 0, 300, 50, "正在导入，请稍等...");
}

ImportFileWindows::ImportFileWindows() {
    windows = new Fl_Window(350, 100, "导入文件");
    input = new Fl_Input(60, 20, 260, 30, "文件地址");
    btn = new Fl_Button(20, 60, 120, 30, "完成");
}



ImportWindows::ImportWindows() {
    windows = new Fl_Window(300, 200, "导入文件");
    btn1 = new Fl_Button(20, 20, 120, 30, "导入基站文件");
    btn2 = new Fl_Button(20, 60, 120, 30, "导入移动文件");
    btn3 = new Fl_Button(20, 100, 120, 30, "导入伪基站文件");
    btn4 = new Fl_Button(20, 140, 120, 30, "完成");
    box1 = new Fl_Box(150, 20, 100, 30, "基站个数: 0"); // 基站个数
    box2 = new Fl_Box(150, 60, 100, 30, "移动个数: 0"); // 移动个数
    box3 = new Fl_Box(150, 100, 100, 30, "伪基站个数: 0"); // 伪基站个数


}




// 提示窗口显示
void TipWindows::show() {
    box->copy_label("正在导入，请稍等...");
    windows->end();
    windows->show();
}

// 失败提示
void TipWindows::fail() {
    box->copy_label("导入失败,请检查文件地址和文件内容");
    Fl::add_timeout(1, [](void *data) {
        auto *This = (TipWindows *) data;
        This->windows->hide();
    }, this);
}

// 提示窗口销毁
void TipWindows::destroy() {
    box->copy_label("导入完成");
    Fl::add_timeout(0.5, [](void *data) {
        auto *This = (TipWindows *) data;
        This->windows->hide();
    }, this);
}

// 导入文件窗口显示
void ImportFileWindows::show() {
    windows->end();
    windows->show();
}

// 导入窗口显示
void ImportWindows::show() {
    // 导入基站文件
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (ImportWindows *) data;
        This->station.input->value("/Users/wangjian/code/C:C++/GSMwork/data/jz001.txt");
        This->station.show();
        //关闭窗口时加载文件信息
        This->station.btn->callback([](Fl_Widget *w, void *data) {
            auto *This = (ImportWindows *) data;
            This->station.tip.show();
            This->station.fileReader = new FileReader(This->station.input->value()); // 构建文件读取类
            if (This->station.fileReader->file_type == 0) {
                This->station.tip.fail();
                This->station.destroy();
                return;
            }
            auto newStationPoints = This->station.fileReader->StationDataProcess(This->x_min, This->x_max, This->y_min, This->y_max); // 获取新基站数据
            if (newStationPoints.empty()) {
                This->station.tip.fail();
                This->station.destroy();
                return;
            }
            This->stationPoints.insert(This->stationPoints.end(), newStationPoints.begin(), newStationPoints.end()); // 将新基站数据添加到原基站数据中
            // 修改基站个数
            This->box1->copy_label(("基站个数: " + to_string(This->stationPoints.size())).c_str());
            This->station.tip.destroy();
            This->station.destroy();
            }, This);
        }, this);
    // 导入移动文件
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (ImportWindows *) data;
        This->rule.input->value("/Users/wangjian/code/C:C++/GSMwork/data/yd001.txt");
        This->rule.show();
        //关闭窗口时加载文件信息
        This->rule.btn->callback([](Fl_Widget *w, void *data) {
            auto *This = (ImportWindows *) data;
            This->rule.tip.show();
            This->rule.fileReader = new FileReader(This->rule.input->value()); // 构建文件读取类
            if (This->rule.fileReader->file_type == 0) {
                This->rule.tip.fail();
                This->rule.destroy();
                return;
            }
            This->ruleList = This->rule.fileReader->RuleDataProcess(); // 获取新基站数据
            if (!This->ruleList) {
                This->rule.tip.fail();
                This->rule.destroy();
                return;
            }
            // 修改移动个数
            auto cur = This->ruleList;
            while (cur->next) {
                cur = cur->next;
            }
            This->box2->copy_label(("移动个数: " + to_string(cur->id)).c_str());
            This->rule.tip.destroy();
            This->rule.destroy();
        }, This);
    }, this);
    // 导入伪基站文件
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (ImportWindows *) data;
        This->fakeRule.input->value("/Users/wangjian/code/C:C++/GSMwork/data/wz001.txt");
        This->fakeRule.show();
        //关闭窗口时加载文件信息
        This->fakeRule.btn->callback([](Fl_Widget *w, void *data) {
            auto *This = (ImportWindows *) data;
            This->fakeRule.tip.show();
            This->fakeRule.fileReader = new FileReader(This->fakeRule.input->value()); // 构建文件读取类
            if (This->fakeRule.fileReader->file_type == 0) {
                This->fakeRule.tip.fail();
                This->fakeRule.destroy();
                return;
            }
            auto newFakePoints = This->fakeRule.fileReader->FakeRuleDataProcess(); // 获取新基站数据
            if (newFakePoints.empty()) {
                This->fakeRule.tip.fail();
                This->fakeRule.destroy();
                return;
            }
            //将新伪基站数据添加到原伪基站数据中
            This->fakeRuleList.insert(This->fakeRuleList.end(), newFakePoints.begin(), newFakePoints.end());
            // 修改伪基站个数
            This->box3->copy_label(("伪基站个数: " + to_string(This->fakeRuleList.size())).c_str());
            This->fakeRule.tip.destroy();
            This->fakeRule.destroy();
        }, This);
    }, this);
    // 完成
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (ImportWindows *) data;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}

DrawWindows::DrawWindows(QuadTreeNode *root, MovePoint *movePoint) {
    this->root = root;
    this->movePoints = movePoint;
    windows = new Fl_Window(1400, 600, "绘制地图");
    btn1 = new Fl_Button(20, 20, 100, 30, "左上地图");
    btn2 = new Fl_Button(140, 20, 100, 30, "右上地图");
    btn3 = new Fl_Button(260, 20, 100, 30, "左下地图");
    btn4 = new Fl_Button(380, 20, 100, 30, "右下地图");
    btn5 = new Fl_Button(500, 20, 100, 30, "退回");
    btn6 = new Fl_Button(620, 20, 100, 30, "退回全图");
    btn7 = new Fl_Button(740, 20, 100, 30, "左");
    btn8 = new Fl_Button(860, 20, 100, 30, "右");
    btn9 = new Fl_Button(980, 20, 100, 30, "上");
    btn10 = new Fl_Button(1100, 20, 100, 30, "下");
    btn11 = new Fl_Button(1220, 20, 100, 30, "完成");

    // 绘制地图区域
    box1 = new Fl_Box(0, 20, 1400, 600);

}

void DrawChildren (void *data) {
    auto *This = (DrawWindows *) data;
    This->plot = new PlotMap(This->view, This->Times);
    This->plot->PlotStation(This->view);
    This->plot->PlotPath(*This->movePoints);
    // 转换图像
    cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
    This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                              This->plot->image.rows, 3);
    This->box1->image(This->image);
    This->box1->redraw();
    delete This->plot;
}

// 绘制窗口显示
void DrawWindows::show() {
    Times = 1;
    view = root;
    DrawChildren(this);
    // 绘制左上地图
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        if (This->view->Child(0) != nullptr) {
            This->view = This->view->Child(0);
            This->Times *= 2;
        }
        DrawChildren(data);
    }, this);
    // 绘制右上地图
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        if (This->view->Child(1) != nullptr) {
            This->view = This->view->Child(1);
            This->Times *= 2;
        }
        DrawChildren(data);
    }, this);
    // 绘制左下地图
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        if (This->view->Child(2) != nullptr) {
            This->view = This->view->Child(2);
            This->Times *= 2;
        }
        DrawChildren(data);
    }, this);
    // 绘制右下地图
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        if (This->view->Child(3) != nullptr) {
            This->view = This->view->Child(3);
            This->Times *= 2;
        }
        DrawChildren(data);
    }, this);
    // 退回
    btn5->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        if (This->view->Parent() != nullptr) {
            This->view = This->view->Parent();
            This->Times /= 2;
        }
        DrawChildren(data);
    }, this);
    // 绘制地图
    btn6->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        This->view = This->root;
        This->Times = 1;
        DrawChildren(data);
    }, this);
    // 左
    btn7->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        if (This->view != This->root) {
            if (This->view == This->view->parent->Child(1)) {
                This->view = This->view->parent->Child(0);
            } else if (This->view == This->view->parent->Child(3)) {
                This->view = This->view->parent->Child(2);
            }
        }
        DrawChildren(data);
    }, this);
    // 右
    btn8->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        if (This->view != This->root) {
            if (This->view == This->view->parent->Child(0)) {
                This->view = This->view->parent->Child(1);
            } else if (This->view == This->view->parent->Child(2)) {
                This->view = This->view->parent->Child(3);
            }
        }
        DrawChildren(data);
    }, this);
    // 上
    btn9->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        if (This->view != This->root) {
            if (This->view == This->view->parent->Child(2)) {
                This->view = This->view->parent->Child(0);
            } else if (This->view == This->view->parent->Child(3)) {
                This->view = This->view->parent->Child(1);
            }
        }
        DrawChildren(data);
    }, this);
    // 下
    btn10->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->box1->image(nullptr);
        if (This->view != This->root) {
            if (This->view == This->view->parent->Child(0)) {
                This->view = This->view->parent->Child(2);
            } else if (This->view == This->view->parent->Child(1)) {
                This->view = This->view->parent->Child(3);
            }
        }
        DrawChildren(data);
    }, this);
    // 完成
    btn11->callback([](Fl_Widget *w, void *data) {
        auto *This = (DrawWindows *) data;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}


void DrawPoint (void *data) {
    auto *This = (SearchWindows *) data;
    // 绘制地图
    This->box1->image(nullptr);
    cvtColor(This->plotPoint->image, This->plotPoint->image, COLOR_BGR2RGB);
    This->image = new Fl_RGB_Image(This->plotPoint->image.data, This->plotPoint->image.cols,
                                                This->plotPoint->image.rows, 3);
    This->box1->image(This->image);
    This->box1->redraw();
}

// 搜寻窗口
SearchWindows::SearchWindows(QuadTreeNode *root) {
    this->root = root;
    windows = new Fl_Window(1400, 600, "搜索窗口");
    input1 = new Fl_Input(20, 20, 100, 30, "x坐标");
    input2 = new Fl_Input(140, 20, 100, 30, "y坐标");
    btn1 = new Fl_Button(500, 20, 100, 30, "搜索");
    btn2 = new Fl_Button(260, 20, 100, 30, "放大");
    btn3 = new Fl_Button(380, 20, 100, 30, "缩小");
    btn4 = new Fl_Button(620, 20, 100, 30, "完成");


    // 绘制地图区域
    box1 = new Fl_Box(0, 20, 1400, 600);
}

// 搜寻窗口显示
void SearchWindows::show() {
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (SearchWindows *) data;
        This->Points = new MovePoint(stod(This->input1->value()),
                                                  stod(This->input2->value()));
        This->plotPoint = new PlotPoint(This->root, This->Points, This->viewTimes);
        This->plotPoint->PlotMovePoint(*This->Points);
        // 绘制地图
        DrawPoint(data);
        // 缩小
        This->btn3->callback([](Fl_Widget *w, void *data) {
            auto *This = (SearchWindows *) data;
            if (This->viewTimes >= 0.25) {
                This->viewTimes /= 2;
            }
            This->plotPoint = new PlotPoint(This->root, This->Points, This->viewTimes);
            This->plotPoint->PlotMovePoint(*This->Points);
            // 绘制地图
            DrawPoint(data);
        }, This);
        // 放大
        This->btn2->callback([](Fl_Widget *w, void *data) {
            auto *This = (SearchWindows *) data;
            if (This->viewTimes <= 2) {
                This->viewTimes *= 2;
            }
            This->plotPoint = new PlotPoint(This->root, This->Points, This->viewTimes);
            This->plotPoint->PlotMovePoint(*This->Points);
            // 绘制地图
            DrawPoint(data);
        }, This);
    }, this);
    // 完成
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (SearchWindows *) data;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}


void MainWindows::show() {
    // 绘制地图
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (MainWindows *) data;
        This->drawWindows->show();
    }, this);
    // 查找窗口
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (MainWindows *) data;
        This->searchWindows->show();
    }, this);
    // 任务窗口
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (MainWindows *) data;
        This->taskWindows->show();
    }, this);
    // 退出
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (MainWindows *) data;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}


// 主界面
MainWindows::MainWindows() {
    windows = new Fl_Window(170, 200, "主界面");
    btn1 = new Fl_Button(20, 20, 120, 30, "查看地图");
    btn2 = new Fl_Button(20, 60, 120, 30, "查找点");
    btn3 = new Fl_Button(20, 100, 120, 30, "任务");
    btn4 = new Fl_Button(20, 140, 120, 30, "退出");

}

IndexWindows::IndexWindows() {
    windows = new Fl_Window(170, 120, "主界面");
    btn1 = new Fl_Button(20, 20, 120, 30, "导入文件");
    btn2 = new Fl_Button(20, 60, 120, 30, "开始构建");
}

// 引导窗口显示
void IndexWindows::show() {
    // 导入文件
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (IndexWindows *) data;
        This->importWindows.show();
    }, this);
    // 开始构建
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (IndexWindows *) data;
        This->mainWindows.show();
        This->tipWindows.show();
        // 构建四叉树
        This->mainWindows.root = new QuadTreeNode(This->importWindows.x_min, This->importWindows.x_max, This->importWindows.y_min, This->importWindows.y_max);
        for (auto &stationPoint : This->importWindows.stationPoints) {
            This->mainWindows.root->Insert(stationPoint);
        }
        // 构建移动点
        This->mainWindows.movePoints = new MovePoint(This->importWindows.ruleList);
        // 构建伪基站数据
        vector<FakePoint> fakePoints;
        for (auto &fakePoint : This->importWindows.fakeRuleList) {
            fakePoints.emplace_back(&fakePoint.first, fakePoint.second);
        }
        //初始化主程序
        This->mainWindows.drawWindows = new DrawWindows(This->mainWindows.root, This->mainWindows.movePoints);
        This->mainWindows.searchWindows = new SearchWindows(This->mainWindows.root);
        This->mainWindows.taskWindows = new TaskWindows(This->mainWindows.root, This->mainWindows.movePoints, fakePoints);
        This->tipWindows.destroy();
        This->destroy();
    }, this);
    windows->end();
    windows->show();
}

TaskWindows::TaskWindows(QuadTreeNode *root, MovePoint *movePoints, vector<FakePoint> fakePoints) {
    task1Windows = new Task1Windows(root);
    task2Windows = new Task2Windows(root);
    task3Windows = new Task3Windows(root);
    task4Windows = new Task4Windows(root);
    task5Windows = new Task5Windows(root, movePoints);
    eTask1Windows = new ETask1Windows(root, movePoints);
    eTask2Windows = new ETask2Windows(root, movePoints);
    pTask1Windows = new PTask1Windows(root, movePoints, fakePoints);
    pTask2Windows = new PTask2Windows(root, movePoints, fakePoints);

    windows = new Fl_Window(170, 400, "任务");
    Task1 = new Fl_Button(20, 20, 120, 30, "任务1");
    Task2 = new Fl_Button(20, 60, 120, 30, "任务2");
    Task3 = new Fl_Button(20, 100, 120, 30, "任务3");
    Task4 = new Fl_Button(20, 140, 120, 30, "任务4");
    Task5 = new Fl_Button(20, 180, 120, 30, "任务5");
    ETask1 = new Fl_Button(20, 220, 120, 30, "扩展任务1");
    ETask2 = new Fl_Button(20, 260, 120, 30, "扩展任务2");
    PTask1 = new Fl_Button(20, 300, 120, 30, "升级任务1");
    PTask2 = new Fl_Button(20, 340, 120, 30, "升级任务2");
}

void TaskWindows::show() {
    Task1->callback([](Fl_Widget *w, void *data) {
        auto *This = (TaskWindows *) data;
        This->task1Windows->show();
    }, this);
    Task2->callback([](Fl_Widget *w, void *data) {
        auto *This = (TaskWindows *) data;
        This->task2Windows->show();
    }, this);
    Task3->callback([](Fl_Widget *w, void *data) {
        auto *This = (TaskWindows *) data;
        This->task3Windows->show();
    }, this);
    Task4->callback([](Fl_Widget *w, void *data) {
        auto *This = (TaskWindows *) data;
        This->task4Windows->show();
    }, this);
    Task5->callback([](Fl_Widget *w, void *data) {
        auto *This = (TaskWindows *) data;
        This->task5Windows->show();
    }, this);
    ETask1->callback([](Fl_Widget *w, void *data) {
        auto *This = (TaskWindows *) data;
        This->eTask1Windows->show();
    }, this);
    ETask2->callback([](Fl_Widget *w, void *data) {
        auto *This = (TaskWindows *) data;
        This->eTask2Windows->show();
    }, this);
    PTask1->callback([](Fl_Widget *w, void *data) {
        auto *This = (TaskWindows *) data;
        This->pTask1Windows->show();
    }, this);
    PTask2->callback([](Fl_Widget *w, void *data) {
        auto *This = (TaskWindows *) data;
        This->pTask2Windows->show();
    }, this);
    windows->end();
    windows->show();
}

Task1Windows::Task1Windows(QuadTreeNode *root) {
    this->root = root;
    // 任务1窗口
    windows = new Fl_Window(1200, 300, "任务1");
    btn1 = new Fl_Button(20, 20, 50, 30, "西北角");
    btn2 = new Fl_Button(80, 20, 50, 30, "东南角");
    btn3 = new Fl_Button(140, 20, 50, 30, "完成");

    buffer = new Fl_Text_Buffer();
    display = new Fl_Text_Display(20, 100, 330, 180);
    // 绘制地图区域
    box1 = new Fl_Box(370, 20, 700, 250);

}

void Task1Windows::show() {
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task1Windows *) data;
        auto *cur = This->root;
        int Times = 1;
        while (cur->Child(0) != nullptr) {
            cur = cur->Child(0);
            Times *= 2;
        }
        This->plot = new PlotMap(cur->Parent(), Times / 4);
        This->plot->PlotStation(cur->Parent());
        // 转换图像
        cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
        This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                       This->plot->image.rows, 3);
        This->box1->image(This->image);
        This->box1->redraw();
        // 如果cur有点，显示数据
        if (cur->point != nullptr) {
            string str = "基站数据为:(" + to_string(cur->point->x) + "," + to_string(cur->point->y) + ")" + "  " + to_string(cur->point->power) + " " + to_string(cur->point->id) + "\n";
            This->buffer->append(str.c_str());
        } else {
            This->buffer->append("该区域没有基站\n");
        }
        This->display->buffer(This->buffer);
    }, this);
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task1Windows *) data;
        auto *cur = This->root;
        int Times = 1;
        while (cur->Child(3) != nullptr) {
            cur = cur->Child(3);
            Times *= 2;
        }
        This->plot = new PlotMap(cur->Parent(), Times / 4);
        This->plot->PlotStation(cur->Parent());
        This->plot->PlotStress(cur);
        // 转换图像
        cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
        This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                       This->plot->image.rows, 3);
        This->box1->image(This->image);
        This->box1->redraw();
        // 如果cur有点，显示数据
        if (cur->point != nullptr) {
            string str = "基站数据为:(" + to_string(cur->point->x) + "," + to_string(cur->point->y) + ")" + "  " + to_string(cur->point->power) + " " + to_string(cur->point->id) + "\n";
            This->buffer->append(str.c_str());
        } else {
            This->buffer->append("该区域没有基站\n");
        }
        This->display->buffer(This->buffer);
    }, this);
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task1Windows *) data;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}

Task2Windows::Task2Windows(QuadTreeNode *root) {
    this->root = root;
    // 任务2窗口
    windows = new Fl_Window(1200, 300, "任务2");
    btn1 = new Fl_Button(20, 20, 100, 30, "西北角的东侧");
    btn2 = new Fl_Button(120, 20, 100, 30, "西北角的南侧");
    btn3 = new Fl_Button(220, 20, 100, 30, "东南角的西北侧");
    btn4 = new Fl_Button(20, 50, 200, 30, "东南角的西北侧的北侧");
    btn5 = new Fl_Button(220, 50, 100, 30, "完成");

    buffer = new Fl_Text_Buffer();
    display = new Fl_Text_Display(20, 100, 330, 180);
    // 绘制地图区域
    box1 = new Fl_Box(370, 20, 700, 250);
}

void Task2Windows::show() {
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task2Windows *) data;
        auto *cur = This->root;
        int Times = 1;
        while (cur->Child(0) != nullptr) {
            cur = cur->Child(0);
            Times *= 2;
        }
        cur = cur->Parent()->Child(1);
        This->plot = new PlotMap(cur->Parent(), Times / 4);
        This->plot->PlotStation(cur->Parent());
        This->plot->PlotStress(cur);
        // 转换图像
        cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
        This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                       This->plot->image.rows, 3);
        This->box1->image(This->image);
        This->box1->redraw();
        // 获取cur的所有数据
        if (!cur->GetPoints().empty()) {
            for (auto &point : cur->GetPoints()) {
                string str = "基站数据为:(" + to_string(point->x) + "," + to_string(point->y) + ")" + "  " + to_string(point->power) + " " + to_string(point->id) + "\n";
                This->buffer->append(str.c_str());
            }
        } else {
            This->buffer->append("该区域没有基站\n");
        }
        This->display->buffer(This->buffer);
    }, this);
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task2Windows *) data;
        auto *cur = This->root;
        int Times = 1;
        while (cur->Child(0) != nullptr) {
            cur = cur->Child(0);
            Times *= 2;
        }
        cur = cur->Parent()->Child(2);
        This->plot = new PlotMap(cur->Parent(), Times / 4);
        This->plot->PlotStation(cur->Parent());
        This->plot->PlotStress(cur);
        // 转换图像
        cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
        This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                       This->plot->image.rows, 3);
        This->box1->image(This->image);
        This->box1->redraw();
        // 获取cur的所有数据
        if (!cur->GetPoints().empty()) {
            for (auto &point : cur->GetPoints()) {
                string str = "基站数据为:(" + to_string(point->x) + "," + to_string(point->y) + ")" + "  " + to_string(point->power) + " " + to_string(point->id) + "\n";
                This->buffer->append(str.c_str());
            }
        } else {
            This->buffer->append("该区域没有基站\n");
        }
    }, this);
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task2Windows *) data;
        auto *cur = This->root;
        int Times = 1;
        while (cur->Child(3) != nullptr) {
            cur = cur->Child(3);
            Times *= 2;
        }
        cur = cur->Parent()->Child(0);
        This->plot = new PlotMap(cur->Parent(), Times / 4);
        This->plot->PlotStation(cur->Parent());
        This->plot->PlotStress(cur);
        // 转换图像
        cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
        This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                       This->plot->image.rows, 3);
        This->box1->image(This->image);
        This->box1->redraw();
        // 获取cur的所有数据
        if (!cur->GetPoints().empty()) {
            for (auto &point : cur->GetPoints()) {
                string str = "基站数据为:(" + to_string(point->x) + "," + to_string(point->y) + ")" + "  " + to_string(point->power) + " " + to_string(point->id) + "\n";
                This->buffer->append(str.c_str());
            }
        } else {
            This->buffer->append("该区域没有基站\n");
        }
    }, this);
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task2Windows *) data;
        auto *cur = This->root;
        int Times = 1;
        while (cur->Child(3) != nullptr) {
            cur = cur->Child(3);
            Times *= 2;
        }
        cur = cur->Parent()->Parent()->Child(1)->Child(2);
        This->plot = new PlotMap(cur->Parent(), Times / 4);
        This->plot->PlotStation(cur->Parent());
        This->plot->PlotStress(cur);
        // 转换图像
        cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
        This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                       This->plot->image.rows, 3);
        This->box1->image(This->image);
        This->box1->redraw();
        // 获取cur的所有数据
        if (!cur->GetPoints().empty()) {
            for (auto &point : cur->GetPoints()) {
                string str = "基站数据为:(" + to_string(point->x) + "," + to_string(point->y) + ")" + "  " + to_string(point->power) + " " + to_string(point->id) + "\n";
                This->buffer->append(str.c_str());
            }
        } else {
            This->buffer->append("该区域没有基站\n");
        }
    }, this);
    // 完成
    btn5->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task2Windows *) data;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}

Task3Windows::Task3Windows(QuadTreeNode *root) {
    this->root = root;
    // 任务2窗口
    windows = new Fl_Window(1200, 300, "任务3");
    input1 = new Fl_Input(40, 20, 80, 30, "x坐标");
    input2 = new Fl_Input(40, 50, 80, 30, "y坐标");
    btn1 = new Fl_Button(250, 20, 100, 30, "搜索");
    btn2 = new Fl_Button(250, 50, 100, 30, "完成");
    btn3 = new Fl_Button(150, 20, 100, 30, "放大");
    btn4 = new Fl_Button(150, 50, 100, 30, "缩小");

    buffer = new Fl_Text_Buffer();
    display = new Fl_Text_Display(20, 100, 330, 180);
    // 绘制地图区域
    box1 = new Fl_Box(370, 20, 700, 250);
}

// 绘制点窗口
void DrawMovePoint (void* data) {
    auto *This = (Task3Windows *) data;
    This->plot = new PlotPoint(This->root, This->Points, This->Times, 0.5);
    This->plot->PlotMovePoint(*This->Points);
    // 获取强度最强点
    This->signal = 0;
    This->stationPoints = This->root->FindStrongestPoints(*This->Points, 1, This->signal);
    //强度大于1时画线
    if (This->signal > 1) {
        This->plot->PlotLink(*This->Points, This->stationPoints);
    }
    // 转换图像
    cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
    This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                   This->plot->image.rows, 3);
}


void Task3Windows::show() {
    // 搜索点位置
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task3Windows *) data;
        This->Points = new MovePoint(stod(This->input1->value()), stod(This->input2->value()));
        DrawMovePoint(data);
        This->box1->image(This->image);
        This->box1->redraw();

        // 输出基站数据
        string str = "基站数据为:(" + to_string(This->stationPoints.x) + "," + to_string(This->stationPoints.y) + ")" + "  " + to_string(This->stationPoints.power) + " " + to_string(This->stationPoints.id) + "\n";
        This->buffer->append(str.c_str());
        This->buffer->append("信号强度为:");
        This->buffer->append(to_string(This->signal).c_str());
        This->buffer->append("\n");
        This->buffer->append("距离为:");
        This->buffer->append(to_string(distance(*This->Points, This->stationPoints)).c_str());
        if (This->signal > 1) {
            This->buffer->append("\n");
            This->buffer->append("信号被覆盖");
        } else {
            This->buffer->append("\n");
            This->buffer->append("信号没有覆盖");
        }
        This->display->buffer(This->buffer);
    }, this);
    // 放大
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task3Windows *) data;
        This->Times *= 2;
        DrawMovePoint(data);
        This->box1->image(This->image);
        This->box1->redraw();
    }, this);
    // 缩小
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task3Windows *) data;
        This->Times /= 2;
        DrawMovePoint(data);
        This->box1->image(This->image);
        This->box1->redraw();
    }, this);
    // 完成
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task3Windows *) data;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}

Task4Windows::Task4Windows(QuadTreeNode *root) {
    task3Windows = new Task3Windows(root);
}

void Task4Windows::show() {
    task3Windows->show();
}

Task5Windows::Task5Windows(QuadTreeNode *root, MovePoint *points) {
    rootMovePoint = points;
    this->root = root;
    // 任务2窗口
    windows = new Fl_Window(1200, 300, "任务5");
    btn1 = new Fl_Button(20, 20, 100, 30, "运行");
    btn2 = new Fl_Button(20, 50, 100, 30, "过程模拟");
    btn3 = new Fl_Button(120, 20, 100, 30, "放大");
    btn4 = new Fl_Button(120, 50, 100, 30, "缩小");
    btn5 = new Fl_Button(220, 20, 100, 30, "加速");
    btn6 = new Fl_Button(220, 50, 100, 30, "减速");
    btn7 = new Fl_Button(320, 20, 100, 30, "完成");
    buffer = new Fl_Text_Buffer();
    display = new Fl_Text_Display(20, 150, 400, 100);
    // 绘制地图区域
    box1 = new Fl_Box(500, 20, 700, 250);
}

void Task5Windows::show() {
    static MovePoint temp = *rootMovePoint;
    movePoint = &temp;
    // 运行
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task5Windows *) data;
        static MovePoint temp = *This->rootMovePoint;
        This->movePoint = &temp;
        while (true) {
            switch (This->movePoint->Move(0.1 , This->root).type) {
                case -1: {
                    break;
                }
                case 0: {
                    auto str = to_string(This->movePoint->NowTime) + "s 断开连接";
                    This->buffer->append(str.c_str());
                    This->buffer->append("\n");
                    break;
                }
                default : {
                    auto str = to_string(This->movePoint->NowTime) + "s 连接到基站" + to_string(This->movePoint->x) + "," + to_string(This->movePoint->y) + "编号" + to_string(This->movePoint->Link.id);
                    This->buffer->append(str.c_str());
                    This->buffer->append("\n");
                    break;
                }
            }
            if (This->movePoint->head == nullptr && This->movePoint->speed == 0) {
                break;
            }
            This->display->buffer(This->buffer);
            This->display->redraw();
        }
        static MovePoint newTemp = *This->rootMovePoint;
        This->movePoint = &newTemp;
    }, this);
    // 过程模拟
    btn2->callback(([](Fl_Widget *w, void *data) {
        auto *This = (Task5Windows *) data;
        switch (This->movePoint->Move(This->speedTimes, This->root).type) {
            case -1: {
                break;
            }
            case 0: {
                auto str = to_string(This->movePoint->NowTime) + "s 断开连接";
                This->buffer->append(str.c_str());
                This->buffer->append("\n");
                break;
            }
            default : {
                auto str =
                        to_string(This->movePoint->NowTime) + "s 连接到基站" + to_string(This->movePoint->x) +
                        "," +
                        to_string(This->movePoint->y) + "编号" + to_string(This->movePoint->Link.id);
                This->buffer->append(str.c_str());
                This->buffer->append("\n");
                break;
            }
        }
        This->display->buffer(This->buffer);
        This->display->redraw();
        This->plot = new PlotPoint(This->root, This->movePoint, This->Times, 0.5);
        This->plot->PlotMovePoint(*This->movePoint);
        // 获取除当前的强度最强点
        This->signal = 0;
        This->stationPoints = This->root->FindStrongestPoints(*This->movePoint, 1, This->signal, This->movePoint->Link.id);
        //画当前连接线
        if (This->movePoint->Link.type != 0) {
            This->plot->PlotLink(*This->movePoint, This->movePoint->Link, 1);
        }
        This->plot->PlotLink(*This->movePoint, This->stationPoints);
        // 转换图像
        cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
        This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                       This->plot->image.rows, 3);
        This->box1->image(This->image);
        if (!(This->movePoint->head && This->movePoint->speed) && This->flag ) {
            This->flag = false;
            static MovePoint temp = *This->rootMovePoint;
            This->movePoint = &temp;
        }
        if (This->flag) {
            Fl::add_timeout(0.05 / This->speedTimes, [](void *data) {
                auto This = (Task5Windows *) data;
                This->btn2->do_callback();
                } , This);
        } else {
            This->flag = true;
        }
        This->windows->redraw();
    }), this);
    // 放大
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task5Windows *) data;
        This->Times *= 2;
    }, this);
    // 缩小
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task5Windows *) data;
        This->Times /= 2;
    }, this);
    // 加速
    btn5->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task5Windows *) data;
        This->speedTimes *= 2;
    }, this);
    // 减速
    btn6->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task5Windows *) data;
        This->speedTimes /= 2;
    }, this);
    // 完成
    btn7->callback([](Fl_Widget *w, void *data) {
        auto *This = (Task5Windows *) data;
        This->flag = false;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}

ETask1Windows::ETask1Windows(QuadTreeNode *root, MovePoint *movePoint) {
    this->root = root;
    rootMovePoint = movePoint;

    // 任务2窗口
    windows = new Fl_Window(1200, 300, "扩展任务1");
    btn1 = new Fl_Button(20, 20, 100, 30, "运行");
    btn2 = new Fl_Button(20, 50, 100, 30, "过程模拟");
    btn3 = new Fl_Button(120, 20, 100, 30, "放大");
    btn4 = new Fl_Button(120, 50, 100, 30, "缩小");
    btn5 = new Fl_Button(220, 20, 100, 30, "加速");
    btn6 = new Fl_Button(220, 50, 100, 30, "减速");
    btn7 = new Fl_Button(320, 20, 100, 30, "完成");
    buffer = new Fl_Text_Buffer();
    display = new Fl_Text_Display(20, 150, 400, 100);
    // 绘制地图区域
    box1 = new Fl_Box(500, 20, 700, 250);
}

void ETask1Windows::show() {
    static MovePoint temp = *rootMovePoint;
    movePoint = &temp;
    // 运行
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask1Windows *) data;
        static MovePoint temp = *This->rootMovePoint;
        This->movePoint = &temp;
        while(true) {
            auto last = *This->movePoint;
            This->movePoint->Move(This->speedTimes, This->root);
            if (This->movePoint->Link.type != 0 && last.Link.type == 0) {
                //二分法求出改变的准确位置
                double time = 0.5;
                for (int i = 0; i < 12; i++) {
                    auto cur = last;
                    cur.Move(time, This->root);
                    if (temp.Link.type != 0) {
                        time -= 0.5 / (1 << (i + 1));
                    } else {
                        time += 0.5 / (1 << (i + 1));
                    }
                }
                auto str = "在" + to_string(last.NowTime + time) + "s时，连接上" + to_string(This->movePoint->Link.id) + "\n";
                This->buffer->append(str.c_str());
                This->display->buffer(This->buffer);
            }
            if (This->movePoint->Link.type == 0 && last.Link.type != 0) {
                //二分法求出改变的准确位置
                double time = 0.5;
                for (int i = 0; i < 12; i++) {
                    auto cur = last;
                    cur.Move(time, This->root);
                    if (temp.Link.type != 0) {
                        time += 0.5 / (1 << (i + 1));
                    } else {
                        time -= 0.5 / (1 << (i + 1));
                    }
                }
                auto str = "在" + to_string(last.NowTime + time) + "s时，断开连接" + to_string(This->movePoint->Link.id) + "\n";
                This->buffer->append(str.c_str());
                This->display->buffer(This->buffer);
                This->display->redraw();
                break;
            }
        }
    }, this);
    // 过程模拟
    btn2->callback(([](Fl_Widget *w, void *data) {
        auto *This = (ETask1Windows *) data;
        auto last = *This->movePoint;
        This->movePoint->Move(This->speedTimes, This->root);
        if (This->movePoint->Link.type != 0 && last.Link.type == 0) {
            //二分法求出改变的准确位置
            double time = 0.5;
            for (int i = 0; i < 12; i++) {
                auto cur = last;
                cur.Move(time, This->root);
                if (temp.Link.type != 0) {
                    time -= 0.5 / (1 << (i + 1));
                } else {
                    time += 0.5 / (1 << (i + 1));
                }
            }
            auto str = "在" + to_string(last.NowTime + time) + "s时，连接上" + to_string(This->movePoint->Link.id) + "\n";
            This->buffer->append(str.c_str());
        }
        if (This->movePoint->Link.type == 0 && last.Link.type != 0) {
            //二分法求出改变的准确位置
            double time = 0.5;
            for (int i = 0; i < 12; i++) {
                auto cur = last;
                cur.Move(time, This->root);
                if (temp.Link.type != 0) {
                    time += 0.5 / (1 << (i + 1));
                } else {
                    time -= 0.5 / (1 << (i + 1));
                }
            }
            auto str = "在" + to_string(last.NowTime + time) + "s时，断开连接" + to_string(This->movePoint->Link.id) + "\n";
            This->buffer->append(str.c_str());
            This->display->buffer(This->buffer);
            This->display->redraw();
            This->flag = false;
        }
        This->plot = new PlotPoint(This->root, This->movePoint, This->Times, 0.5);
        This->plot->PlotMovePoint(*This->movePoint);
        // 获取强度最强点
        This->signal = 0;
        This->stationPoints = This->root->FindStrongestPoints(*This->movePoint, 1, This->signal);
        //画当前连接线
        if (This->movePoint->Link.type != 0) {
            This->plot->PlotLink(*This->movePoint, This->movePoint->Link, 1);
        }
        //强度大于1时画线
        if (This->signal > 1) {
            This->plot->PlotLink(*This->movePoint, This->stationPoints);
        }
        // 转换图像
        cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
        This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                       This->plot->image.rows, 3);
        This->box1->image(This->image);
        if (This->flag) {
            Fl::add_timeout(0.05 / This->speedTimes, [](void *data) {
                auto This = (Task5Windows *) data;
                This->btn2->do_callback();
            } , This);
        } else {
            This->flag = true;
            static MovePoint temp = *This->rootMovePoint;
            This->movePoint = &temp;
        }
        This->windows->redraw();
    }), this);
    // 放大
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask1Windows *) data;
        This->Times *= 2;
    }, this);
    // 缩小
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask1Windows *) data;
        This->Times /= 2;
    }, this);
    // 加速
    btn5->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask1Windows *) data;
        This->speedTimes *= 2;
    }, this);
    // 减速
    btn6->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask1Windows *) data;
        This->speedTimes /= 2;
    }, this);
    // 退出
    btn7->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask1Windows *) data;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}

ETask2Windows::ETask2Windows(QuadTreeNode *root, MovePoint *movePoint) {
    this->root = root;
    rootMovePoint = movePoint;
    // 任务2窗口
    windows = new Fl_Window(1200, 300, "扩展任务2");
    btn1 = new Fl_Button(20, 20, 100, 30, "运行");
    btn2 = new Fl_Button(20, 50, 100, 30, "过程模拟");
    btn3 = new Fl_Button(120, 20, 100, 30, "放大");
    btn4 = new Fl_Button(120, 50, 100, 30, "缩小");
    btn5 = new Fl_Button(220, 20, 100, 30, "加速");
    btn6 = new Fl_Button(220, 50, 100, 30, "减速");
    btn7 = new Fl_Button(320, 20, 100, 30, "完成");
    buffer = new Fl_Text_Buffer();
    display = new Fl_Text_Display(20, 150, 400, 100);
    // 绘制地图区域
    box1 = new Fl_Box(500, 20, 700, 250);
}

void ETask2Windows::show() {
    static MovePoint temp = *rootMovePoint;
    movePoint = &temp;
    // 第三和第六个路径重叠
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask2Windows *) data;
        bool flag = false;
        StationPoint temp1(0, 0, 0, 0, 0), temp2(0, 0, 0, 0, 0);
        while(This->movePoint->head->id <= 6){
            This->movePoint->Move(1 , This->root);
            double signal = 0;
            double maxSignal = 0;
            if (This->movePoint->head->id == 3 || This->movePoint->head->id == 6) {
                // 获取当前结点和除当前结点外最强点，如果结点外最强点信号大于1，进入基站重叠区
                auto MaxSignal = This->root->FindStrongestPoints(*This->movePoint, 1, signal, This->movePoint->Link.id);
                if (signal > 1 && !flag && This->movePoint->Link.type) {
                    string str = to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                 to_string(This->movePoint->y) + ")" +  to_string(This->movePoint->Link.id) + " "  + to_string(MaxSignal.id) + "进入基站重叠区";
                    This->buffer->append(str.c_str());
                    This->buffer->append("\n");
                    This->display->buffer(This->buffer);
                    This->display->redraw();
                    temp1 = This->movePoint->Link;
                    temp2 = MaxSignal;
                    flag = true;
                }
                // 当到达两个任意目标的信号低于1，退出基站重叠区
                if (flag && (Signal(*This->movePoint, temp1) < 1 || Signal(*This->movePoint, temp2) < 1)) {
                    string str = to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                 to_string(This->movePoint->y) + ") 退出基站重叠区";
                    This->buffer->append(str.c_str());
                    This->buffer->append("\n");
                    This->display->buffer(This->buffer);
                    This->display->redraw();
                    flag = false;
                }
            }
        }
        static MovePoint temp = *This->rootMovePoint;
        This->movePoint = &temp;
    }, this);
    // 过程模拟
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask2Windows *) data;
        This->movePoint->Move(This->speedTimes , This->root);
        if (This->movePoint->head && (This->movePoint->head->id == 3 || This->movePoint->head->id == 6)) {
            // 获取当前结点和除当前结点外最强点，如果结点外最强点信号大于1，进入基站重叠区
            double signal;
            auto MaxSignal = This->root->FindStrongestPoints(*This->movePoint, 1, signal, This->movePoint->Link.id);
            if (signal > 1 && !This->flag1 && This->movePoint->Link.type) {
                string str = to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                             to_string(This->movePoint->y) + ")" + to_string(This->movePoint->Link.id) + " " +
                        to_string(MaxSignal.id)  + "进入基站重叠区";
                This->buffer->append(str.c_str());
                This->buffer->append("\n");
                This->display->buffer(This->buffer);
                This->display->redraw();
                This->temp1 = This->movePoint->Link;
                This->temp2 = MaxSignal;
                This->flag1 = true;
            }
            // 当到达两个任意目标的信号低于1，退出基站重叠区
            if (This->flag1 && (Signal(*This->movePoint, This->temp1) < 1 || Signal(*This->movePoint, This->temp2) < 1)) {
                string str = to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                             to_string(This->movePoint->y) + ")" + to_string(This->movePoint->Link.id) + " " +
                             to_string(MaxSignal.id)  + "进入基站重叠区";
                This->buffer->append(str.c_str());
                This->buffer->append("\n");
                This->display->buffer(This->buffer);
                This->display->redraw();
                This->flag1 = false;
            }
            This->plot = new PlotPoint(This->root, This->movePoint, This->Times, 0.5);
            This->plot->PlotMovePoint(*This->movePoint);
            // 获取除当前外强度最强点
            This->signal = 0;
            This->stationPoints = This->root->FindStrongestPoints(*This->movePoint, 1, This->signal, This->movePoint->Link.id);
            //画当前连接线
            if (This->movePoint->Link.type != 0) {
                This->plot->PlotLink(*This->movePoint, This->movePoint->Link, 1);
            }
            This->plot->PlotLink(*This->movePoint, This->stationPoints);
            // 转换图像
            cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
            This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                           This->plot->image.rows, 3);
            This->box1->image(This->image);
            This->windows->redraw();
        } else if (This->movePoint->head){
            while (This->movePoint->head && This->movePoint->head->id != 3 && This->movePoint->head->id != 6) {
                This->movePoint->Move(This->speedTimes , This->root);
            }
        } else {
            This->flag = false;
        }
        if (This->flag) {
            Fl::add_timeout(0.05 / This->speedTimes, [](void *data) {
                auto This = (ETask2Windows *) data;
                This->btn2->do_callback();
            } , This);
        } else {
            This->flag = true;
            static MovePoint temp = *This->rootMovePoint;
            This->movePoint = &temp;
        }
    }, this);
    // 放大
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask2Windows *) data;
        This->Times *= 2;
    }, this);
    // 缩小
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask2Windows *) data;
        This->Times /= 2;
    }, this);
    // 加速
    btn5->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask2Windows *) data;
        This->speedTimes *= 2;
    }, this);
    // 减速
    btn6->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask2Windows *) data;
        This->speedTimes /= 2;
    }, this);
    // 退出
    btn7->callback([](Fl_Widget *w, void *data) {
        auto *This = (ETask2Windows *) data;
        This->flag = false;
        This->windows->hide();
    }, this);
    windows->end();
    windows->show();
}

PTask1Windows::PTask1Windows(QuadTreeNode *root, MovePoint *movePoint, vector<FakePoint >fakePoint) {
    this->root = root;
    rootMovePoint = movePoint;
    rootFake = std::move(fakePoint);
    // 任务2窗口
    windows = new Fl_Window(1200, 300, "升级任务1");
    btn1 = new Fl_Button(20, 20, 100, 30, "运行");
    btn2 = new Fl_Button(20, 50, 100, 30, "过程模拟");
    btn3 = new Fl_Button(120, 20, 100, 30, "放大");
    btn4 = new Fl_Button(120, 50, 100, 30, "缩小");
    btn5 = new Fl_Button(220, 20, 100, 30, "加速");
    btn6 = new Fl_Button(220, 50, 100, 30, "减速");
    btn7 = new Fl_Button(320, 20, 100, 30, "完成");
    buffer = new Fl_Text_Buffer();
    display = new Fl_Text_Display(20, 150, 400, 100);
    // 绘制地图区域
    box1 = new Fl_Box(500, 20, 700, 250);
}

void PTask1Windows::show() {
    static MovePoint temp = *rootMovePoint;
    movePoint = &temp;
    fakePoints = rootFake;
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask1Windows *) data;
        //进入第十二阶段
        auto cur = This->movePoint->rule;
        while (cur->id != 12) {
            cur = cur->next;
        }
        while (This->movePoint->NowTime <= cur->beginTime) {
            This->movePoint->Move(This->speedTimes,This->root);
        }
        // 运算查看是否遇见伪基站
        while (This->movePoint->head) {
            auto last = *This->movePoint;
            This->movePoint->Move(This->speedTimes, This->root);
            int i = 0;
            for (auto &fakePoint : This->fakePoints) {
                if (fakePoint.NowTime < This->movePoint->NowTime && fakePoint.speed != 0) {
                    fakePoint.Move(This->movePoint->NowTime - fakePoint.NowTime, This->root);
                    auto NowDistance = distance(*This->movePoint, fakePoint);
                    if (NowDistance <= 40 && This->lastDistance[i] != -1) {
                        if (This->lastDistance[i] > 40) {
                            string str =
                                    to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                    to_string(This->movePoint->y) + ") 遇见" + to_string(fakePoint.id) + "号伪基站";
                            This->buffer->append(str.c_str());
                            This->buffer->append("\n");
                            This->display->buffer(This->buffer);
                            This->display->redraw();
                        }
                        This->movePoint->Link = StationPoint(fakePoint.x, fakePoint.y, 4, 0, 0);
                    } else if (This->lastDistance[i] <= 40 && This->lastDistance[i] != -1) {
                        string str = to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                     to_string(This->movePoint->y) + ") 离开" + to_string(fakePoint.id) + "号伪基站";
                        This->buffer->append(str.c_str());
                        This->buffer->append("\n");
                        This->display->buffer(This->buffer);
                        This->display->redraw();
                    }
                    This->lastDistance[i] = NowDistance;
                    i++;
                }
            }
        }
        static MovePoint temp = *This->rootMovePoint;
        This->movePoint = &temp;
        This->fakePoints = This->rootFake;
    }, this);
    // 过程模拟
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask1Windows *) data;
        // 运算查看是否遇见伪基站
        if (!This->movePoint->head) {
            This->flag = false;
        } else if (This->movePoint->head && This->flag1) {
            auto last = *This->movePoint;
            This->movePoint->Move(This->speedTimes, This->root);
            int i = 0;
            for (auto &fakePoint: This->fakePoints) {
                if (fakePoint.NowTime < This->movePoint->NowTime && fakePoint.speed != 0) {
                    fakePoint.Move(This->movePoint->NowTime - fakePoint.NowTime, This->root);
                    auto NowDistance = distance(*This->movePoint, fakePoint);
                    if (NowDistance <= 40 && This->lastDistance[i] != -1) {
                        if (This->lastDistance[i] >= 40) {
                            string str =
                                    to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                    to_string(This->movePoint->y) + ") 遇见" + to_string(fakePoint.id) + "号伪基站";
                            This->buffer->append(str.c_str());
                            This->buffer->append("\n");
                            This->display->buffer(This->buffer);
                            This->display->redraw();
                        }
                        This->movePoint->Link = StationPoint(fakePoint.x, fakePoint.y, 4, 0, 0);
                    } else if (This->lastDistance[i] <= 40 && This->lastDistance[i] != -1) {
                        string str = to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                     to_string(This->movePoint->y) + ") 离开" + to_string(fakePoint.id) + "号伪基站";
                        This->buffer->append(str.c_str());
                        This->buffer->append("\n");
                        This->display->buffer(This->buffer);
                        This->display->redraw();
                    }
                    This->lastDistance[i] = NowDistance;
                    i++;
                }
            }
            // 绘制地图
            This->plot = new PlotPoint(This->root, This->movePoint, This->Times * 8 , 0.5);
            This->plot->PlotMovePoint(*This->movePoint);
            for (auto fakePoint: This->fakePoints) {
                if (fakePoint.speed != 0 && fakePoint.NowTime <= This->movePoint->NowTime) {
                    This->plot->PlotFakePoint(fakePoint);
                }
            }

            // 获取除当前外强度最强点
            This->signal = 0;
            This->stationPoints = This->root->FindStrongestPoints(*This->movePoint, 1, This->signal,
                                                                  This->movePoint->Link.id);
            //画当前连接线
            if (This->movePoint->Link.type != 0 && This->movePoint->Link.type != 4) {
                This->plot->PlotLink(*This->movePoint, This->movePoint->Link, 1);
            } else if (This->movePoint->Link.type == 4) {
                This->plot->PlotLink(*This->movePoint, This->movePoint->Link, 2);
            }
            This->plot->PlotLink(*This->movePoint, This->stationPoints);
            // 转换图像
            cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
            This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                           This->plot->image.rows, 3);
            This->box1->image(This->image);
            This->box1->redraw();
        } else if (This->movePoint->head) {
            auto cur = This->movePoint->rule;
            while (cur->id != 12) {
                cur = cur->next;
            }
            while (This->movePoint->NowTime <= cur->beginTime) {
                This->movePoint->Move(This->speedTimes, This->root);
            }
            This->flag1 = true;
        }
        if (This->flag) {
            Fl::add_timeout(0.05 / This->speedTimes, [](void *data) {
                auto This = (PTask1Windows *) data;
                This->btn2->do_callback();
            } , This);
        } else {
            This->flag = true;
            static MovePoint temp = *This->rootMovePoint;
            This->movePoint = &temp;
            This->fakePoints = This->rootFake;
        }
    }, this);
    // 放大
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask1Windows *) data;
        This->Times *= 2;
    }, this);
    // 缩小
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask1Windows *) data;
        This->Times /= 2;
    }, this);
    // 加速
    btn5->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask1Windows *) data;
        This->speedTimes *= 2;
    }, this);
    // 减速
    btn6->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask1Windows *) data;
        This->speedTimes /= 2;
    }, this);
    // 结束
    btn7->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask1Windows *) data;
        This->flag = false;
        This->windows->hide();
    }, this);
    this->windows->end();
    this->windows->show();
}

PTask2Windows::PTask2Windows(QuadTreeNode *root, MovePoint *movePoint, vector<FakePoint >fakePoint) {
    this->root = root;
    rootMovePoint = movePoint;
    rootFake = std::move(fakePoint);
    // 任务2窗口
    windows = new Fl_Window(1200, 300, "升级任务1");
    btn1 = new Fl_Button(20, 20, 100, 30, "运行");
    btn2 = new Fl_Button(20, 50, 100, 30, "过程模拟");
    btn3 = new Fl_Button(120, 20, 100, 30, "放大");
    btn4 = new Fl_Button(120, 50, 100, 30, "缩小");
    btn5 = new Fl_Button(220, 20, 100, 30, "加速");
    btn6 = new Fl_Button(220, 50, 100, 30, "减速");
    btn7 = new Fl_Button(320, 20, 100, 30, "完成");
    buffer = new Fl_Text_Buffer();
    display = new Fl_Text_Display(20, 150, 400, 100);
    // 绘制地图区域
    box1 = new Fl_Box(500, 20, 700, 250);
}

void PTask2Windows::show() {
    static MovePoint temp = *rootMovePoint;
    movePoint = &temp;
    fakePoints = rootFake;
    btn1->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask2Windows *) data;
        //进入第九阶段
        auto cur = This->movePoint->rule;
        while (cur->id != 9) {
            cur = cur->next;
        }
        while (This->movePoint->NowTime <= cur->beginTime) {
            This->movePoint->Move(This->speedTimes,This->root);
        }
        // 运算查看是否遇见伪基站
        while (This->movePoint->head->id == 9) {
            auto last = *This->movePoint;
            This->movePoint->Move(This->speedTimes, This->root);
            int i = 0;
            for (auto &fakePoint : This->fakePoints) {
                if (fakePoint.NowTime < This->movePoint->NowTime && fakePoint.speed != 0) {
                    fakePoint.Move(This->movePoint->NowTime - fakePoint.NowTime, This->root);
                    auto NowDistance = distance(*This->movePoint, fakePoint);
                    if (NowDistance <= 40 && This->lastDistance[i] != -1) {
                        if (This->lastDistance[i] > 40) {
                            string str =
                                    to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                    to_string(This->movePoint->y) + ") 遇见" + to_string(fakePoint.id) + "号伪基站";
                            This->buffer->append(str.c_str());
                            This->buffer->append("\n");
                            This->display->buffer(This->buffer);
                            This->display->redraw();
                        }
                        This->movePoint->Link = StationPoint(fakePoint.x, fakePoint.y, 4, 0, 0);
                    } else if (This->lastDistance[i] <= 40 && This->lastDistance[i] != -1) {
                        string str = to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                     to_string(This->movePoint->y) + ") 离开" + to_string(fakePoint.id) + "号伪基站";
                        This->buffer->append(str.c_str());
                        This->buffer->append("\n");
                        This->display->buffer(This->buffer);
                        This->display->redraw();
                    }
                    This->lastDistance[i] = NowDistance;
                    i++;
                }
            }
        }
        static MovePoint temp = *This->rootMovePoint;
        This->movePoint = &temp;
        This->fakePoints = This->rootFake;
    }, this);
    // 过程模拟
    btn2->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask2Windows *) data;
        // 运算查看是否遇见伪基站
        if (!This->movePoint->head) {
            This->flag = false;
        } else if (This->movePoint->head && This->flag1) {
            auto last = *This->movePoint;
            This->movePoint->Move(This->speedTimes, This->root);
            int i = 0;
            for (auto &fakePoint: This->fakePoints) {
                if (fakePoint.NowTime < This->movePoint->NowTime && fakePoint.speed != 0) {
                    fakePoint.Move(This->movePoint->NowTime - fakePoint.NowTime, This->root);
                    auto NowDistance = distance(*This->movePoint, fakePoint);
                    if (NowDistance <= 40 && This->lastDistance[i] != -1) {
                        if (This->lastDistance[i] >= 40) {
                            string str =
                                    to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                    to_string(This->movePoint->y) + ") 遇见" + to_string(fakePoint.id) + "号伪基站";
                            This->buffer->append(str.c_str());
                            This->buffer->append("\n");
                            This->display->buffer(This->buffer);
                            This->display->redraw();
                        }
                        This->movePoint->Link = StationPoint(fakePoint.x, fakePoint.y, 4, 0, 0);
                    } else if (This->lastDistance[i] <= 40 && This->lastDistance[i] != -1) {
                        string str = to_string(This->movePoint->NowTime) + "s (" + to_string(This->movePoint->x) + "," +
                                     to_string(This->movePoint->y) + ") 离开" + to_string(fakePoint.id) + "号伪基站";
                        This->buffer->append(str.c_str());
                        This->buffer->append("\n");
                        This->display->buffer(This->buffer);
                        This->display->redraw();
                    }
                    This->lastDistance[i] = NowDistance;
                    i++;
                }
            }
            // 绘制地图
            This->plot = new PlotPoint(This->root, This->movePoint, This->Times * 8 , 0.5);
            This->plot->PlotMovePoint(*This->movePoint);
            for (auto fakePoint: This->fakePoints) {
                if (fakePoint.speed != 0 && fakePoint.NowTime <= This->movePoint->NowTime) {
                    This->plot->PlotFakePoint(fakePoint);
                }
            }

            // 获取除当前外强度最强点
            This->signal = 0;
            This->stationPoints = This->root->FindStrongestPoints(*This->movePoint, 1, This->signal,
                                                                  This->movePoint->Link.id);
            //画当前连接线
            if (This->movePoint->Link.type != 0 && This->movePoint->Link.type != 4) {
                This->plot->PlotLink(*This->movePoint, This->movePoint->Link, 1);
            } else if (This->movePoint->Link.type == 4) {
                This->plot->PlotLink(*This->movePoint, This->movePoint->Link, 2);
            }
            This->plot->PlotLink(*This->movePoint, This->stationPoints);
            // 转换图像
            cvtColor(This->plot->image, This->plot->image, COLOR_BGR2RGB);
            This->image = new Fl_RGB_Image(This->plot->image.data, This->plot->image.cols,
                                           This->plot->image.rows, 3);
            This->box1->image(This->image);
            This->box1->redraw();
        } else if (This->movePoint->head) {
            auto cur = This->movePoint->rule;
            while (cur->id != 9) {
                cur = cur->next;
            }
            while (This->movePoint->NowTime <= cur->beginTime) {
                This->movePoint->Move(This->speedTimes, This->root);
            }
            This->flag1 = true;
        }
        if (This->flag) {
            Fl::add_timeout(0.05 / This->speedTimes, [](void *data) {
                auto This = (PTask2Windows *) data;
                This->btn2->do_callback();
            } , This);
        } else {
            This->flag = true;
            static MovePoint temp = *This->rootMovePoint;
            This->movePoint = &temp;
            This->fakePoints = This->rootFake;
        }
    }, this);
    // 放大
    btn3->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask2Windows *) data;
        This->Times *= 2;
    }, this);
    // 缩小
    btn4->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask2Windows *) data;
        This->Times /= 2;
    }, this);
    // 加速
    btn5->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask2Windows *) data;
        This->speedTimes *= 2;
    }, this);
    // 减速
    btn6->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask2Windows *) data;
        This->speedTimes /= 2;
    }, this);
    // 结束
    btn7->callback([](Fl_Widget *w, void *data) {
        auto *This = (PTask2Windows *) data;
        This->flag = false;
        This->windows->hide();
    }, this);
    this->windows->end();
    this->windows->show();
}

