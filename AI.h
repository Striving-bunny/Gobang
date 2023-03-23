// AI类
// Created by sansan on 2022/6/20.
//

#pragma once
#ifndef GOBANGPROJECT_AI_H
#define GOBANGPROJECT_AI_H


#include "Chess.h"

class AI {
public:
    // 初始化
    explicit AI(Chess * chess);
    // 下棋操作
    std::pair<int,int> go(std::pair<int,int> prePos);
private:
    Chess *chess = nullptr;
    std::vector<std::vector<int>> scoreMap;
private:
    void calculateScore();
    ChessPos think();
};

#endif //GOBANGPROJECT_AI_H
