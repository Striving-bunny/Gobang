// 控制类
// Created by sansan on 2022/6/20.
//

#pragma once
#ifndef GOBANGPROJECT_CHESSGAME_H
#define GOBANGPROJECT_CHESSGAME_H


#include "Man.h"
#include "AI.h"

class ChessGame {
public:
    // 构造函数
    ChessGame(Man *man, AI *ai, Chess *chess) :
            man(man), ai(ai), chess(chess){};

public:
    // 开始棋局
    void play();

private:
    Man *man;
    AI *ai;
    Chess *chess;
};


#endif //GOBANGPROJECT_CHESSGAME_H
