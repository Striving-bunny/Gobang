//
// Created by sansan on 2022/6/20.
//

#include "Man.h"


std::pair<int,int> Man::go(std::pair<int,int> prePos) {
    MOUSEMSG msg;
    ChessPos pos;

    while (1) {
        // 获取鼠标信息
        msg = GetMouseMsg();
        // 判断落子位置 鼠标单击操作是否有效
        if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBorad(msg.x, msg.y, pos))     // 左键点击 并且点击有效
        {
            break;
        }
    }

//    std::cout << "finished, the location is: " << std::endl;
//    printf("%d, %d\n", pos.row, pos.col);

    // 落子
    chess->ChessDown(prePos,pos, CHESS_BLACK);
    return std::make_pair(pos.row,pos.col);
}