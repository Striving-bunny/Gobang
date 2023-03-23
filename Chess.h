// 棋盘、棋子类
// Created by sansan on 2022/6/20.
//

#pragma once
#ifndef GOBANGPROJECT_CHESS_H
#define GOBANGPROJECT_CHESS_H

#ifndef INF
#define INF 0x3f3f3f3f
#endif

#include <cmath>
#include <graphics.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <mmsystem.h>

// 设计图片背景为透明函数
void putimagePNG(int x, int y, IMAGE *picture); //x为载入图片的X坐标，y为Y坐标

struct ChessPos {
    int row;
    int col;
};

typedef enum chess_white_black {
    CHESS_WHITE = -1,     // 白棋
    CHESS_BLACK = 1       // 黑棋
} chess_kind;

class Chess {
public:
    // 构造函数
    Chess(int gradesize, int marginX, int marginY, float chesssize);

    // 拷贝构造函数
    Chess(const Chess &chess);

    // 拷贝赋值
    Chess &operator=(const Chess &chess);
    // 析构函数
    ~Chess() = default;
public:
    // 初始化操作 加载图片资源，初始化棋盘的相关数据
    void init();

    // 根据鼠标点击位置返回正确棋盘坐标（即要落子位置）
    bool clickBorad(const int &x, const int &y, ChessPos &pos);

    // 棋子落子位置
    void ChessDown(const std::pair<int,int>&prePos,const ChessPos &pos, const chess_kind &kind);

    // 获取棋盘大小 13线 15线 19线
    int getBoradSize() const;

    // 获取指定位置是黑棋、白棋还是空白
    int getChessData(const ChessPos &pos) const;

    int getChessData(const int &row, const int &col) const;

    // 检查棋局是否结束
    bool checkOver(const std::pair<int,int>&pos,const int &kind);

private:
    IMAGE chessBlackImgMove;
    IMAGE chessWhiteImgMove;
    IMAGE chessBlackImgDown;    // 黑棋棋子
    IMAGE chessWhiteImgDown;    // 白棋棋子
    IMAGE chessRedImg;      // 赢棋填充红色棋子

    int boardSize;          // 棋盘大小 13 15 17 19
    int margin_x, margin_y;  // 棋盘的左侧边界  棋盘的顶部边界
    float chessSize;        // 棋子的大小
    // 存储当前棋局的棋子分布数据 0：空白 1：黑棋 -1：白棋
    std::vector<std::vector<int>> chessMap;

    // 谁的回合
    bool playerFlag;        // true：黑棋  false：白棋
    // 更新棋盘
    void updateChessMap(const ChessPos &pos);
};


#endif //GOBANGPROJECT_CHESS_H
