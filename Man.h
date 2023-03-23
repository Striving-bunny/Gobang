// 玩家/棋手类
// Created by sansan on 2022/6/20.
//

#pragma once
#ifndef GOBANGPROJECT_MAN_H
#define GOBANGPROJECT_MAN_H

#include <iostream>
#include "Chess.h"

class Man {
public:
//    void init(Chess *chess);
    explicit Man(Chess * chess)
    {
        this->chess = chess;
    }
    // 下棋操作
    std::pair<int,int> go(std::pair<int,int> prePos);
private:
    Chess *chess = nullptr;
};


#endif //GOBANGPROJECT_MAN_H
