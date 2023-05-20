#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../FileProcessing.h"

#define MAX_ 1000000000
#define MIN_ (-1000000000)

using namespace std;

// 声明构造函数，传入文件名
FileReader::FileReader(const string& filename) {
    file.open(filename);        // 打开文件
    if (!file.is_open()) {      // 如果文件未打开
        cerr << "Failed to open file: " << filename << endl;  // 输出错误信息
        file_type = 0;
    } else {
        file_type = 1;
    }
}

// 析构函数，用于在对象销毁时关闭文件流
FileReader::~FileReader() {
    if (file.is_open()) {
        file.close();  // 关闭文件流
    }
}

// 声明 readLine 方法，用于读取一行文本
string FileReader::readLine() {
    string line;
    getline(file, line);  // 从文件流中读取一行文本到 line 变量
    return line;          // 返回 line 变量
}

// 声明 endOfFile 方法，用于检查文件是否到达末尾
bool FileReader::endOfFile() {
    return file.eof();   // 返回文件流的 eof 标志
}

//将读取到的字符串转化为基站数据存储到地图内
vector<StationPoint*> FileReader::StationDataProcess(int &x_min, int &x_max, int &y_min, int &y_max) {
    //判断文件类型
    string Type = readLine();
    if (Type.find("JZ") == string::npos) {
        return {}; //返回空
    }
    //循环读取文件
    vector<StationPoint*> Points;
    for(string input = readLine(); !endOfFile();input = readLine()) {
        string tokens[5];
        istringstream ss(input);
        string token;
        int i = 0;
        while (getline(ss, token, ' ')) {
            if (token.empty()) continue;
            istringstream sss(token);
            while (getline(sss, token, ',')){
                tokens[i++] = token;
            }
        }
        //判断是否符合数据条件
        if (!tokens[4].empty()) {
            int type;
            if (tokens[2] == "\xb3\xc7\xc7\xf8") {
                type = 1;
            } else if (tokens[2] == "\xcf\xe7\xd5\xf2") {
                type = 2;
            } else {
                type = 3;
            }
            int x, y;
            x = stoi(tokens[0]);
            y = stoi(tokens[1]);
            if (x < x_min) x_min = x;
            if (x > x_max) x_max = x;
            if (y < y_min) y_min = y;
            if (y > y_max) y_max = y;
            auto p = new StationPoint(x, y, type, stod(tokens[3]), stoi(tokens[4]));
            Points.push_back(p);
        }
    }

    return Points;
}

//将读取到的字符串转化为规则数据
Rule* FileReader::RuleDataProcess() {
    //判断文件类型
    string Type = readLine();
    if (Type.find("YD") == string::npos) {
        return nullptr; //返回空
    }
    Rule* head = nullptr;
    int id = 1;
    for (string input = readLine(); !endOfFile(); input = readLine()) {
        string tokens[7];
        istringstream ss(input);
        string token;
        int i = 0;
        while (getline(ss, token, ',')) {
            tokens[i++] = token;
        }
        //判断是否符合数据条件
        if (!tokens[3].empty()) {
            Point beganPoint(stoi(tokens[0]), stoi(tokens[1])), endPoint(stoi(tokens[2]), stoi(tokens[3]));
            auto Time = stod(tokens[5]) * 60 * 60 + stod(tokens[6]) * 60;
            auto p = new Rule(id++,beganPoint, endPoint, stod(tokens[4]) * 0.2778, Time);
            if (head == nullptr) {
                head = p;
            } else {
                head->Insert(p);
            }
        }
    }
    return head;
}

//将读取到的字符串转化为伪基站规则数据
vector<pair<Rule, int>> FileReader::FakeRuleDataProcess() {
    //判断文件类型
    string Type = readLine();
    if (Type.find("WZ") == string::npos) {
        return {}; //返回空
    }
    vector<pair<Rule, int>> Rules;
    for (string input = readLine(); !endOfFile(); input = readLine()) {
        string tokens[8];
        istringstream ss(input);
        string token;
        int i = 0;
        while (getline(ss, token, ',')) {
            tokens[i++] = token;
        }
        //判断是否符合数据条件
        if (!tokens[3].empty()) {
            Point beganPoint(stoi(tokens[0]), stoi(tokens[1])), endPoint(stoi(tokens[2]), stoi(tokens[3]));
            auto Time = stod(tokens[5]) * 60 * 60 + stod(tokens[6]) * 60;
            auto p = new Rule(0, beganPoint, endPoint, stod(tokens[4]) * 0.2778, Time);
            Rules.emplace_back(*p, stoi(tokens[7]));
        }
    }
    return Rules;
}



