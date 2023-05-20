#include <fstream>
#include <vector>
#include "QuadTree.h"

#define _MAX 1000000000
#define _MIN -1000000000

// 声明 FileReader 类
class FileReader {
private:
    ifstream file;   // 定义私有成员变量 file，表示文件流
    string readLine();// 读取一行文本
    bool endOfFile(); // 检查文件是否到达末尾
public:
    int file_type; //文件类型
    FileReader(const string& filename);// 声明构造函数，传入文件名
    ~FileReader();// 析构函数，用于在对象销毁时关闭文件流
    vector<StationPoint*> StationDataProcess(int &x_min, int &x_max, int &y_min, int &y_max); //将读取到的字符串转化为基站数据
    Rule* RuleDataProcess(); //将读取到的字符串转化为规则数据
    vector<pair<Rule, int>> FakeRuleDataProcess(); //将读取到的字符串转化为伪基站规则数据
};