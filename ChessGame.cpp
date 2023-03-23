//
// Created by sansan on 2022/6/20.
//

#include "ChessGame.h"


// 开始对局
void ChessGame::play() {
    chess->init();
    IMAGE again,fagain;
    loadimage(&again,"res/s02_zlyja.png");
    loadimage(&again,"res/s02_zlyja.png");
    std::pair<int, int> manPos,aiPos{-100,-100};
    while (1) {
        // 玩家回合  玩家先手
        manPos = man->go(aiPos);
        if (chess->checkOver(manPos, CHESS_BLACK)) {
            IMAGE win,bk;
            loadimage(&win,"res/s01_sl.png");
            loadimage(&bk,"res/s01_ms.png");
            putimagePNG(175,175,&bk);
            putimagePNG(95,204,&win);
            // 播放胜利提示音
            mciSendString("play res/victory2.mp3", 0, 0, 0);
            putimagePNG(193,423,&again);
            MOUSEMSG msg ;
            while (1) {
                msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN &&
                    msg.x > 193 && msg.x < 443
                    && msg.y > 423 && msg.y < 513) {
                    mciSendString("play res/click.mp3", 0, 0, 0);
                    Sleep(500);
                    break;
                }
            }
            chess->init();
            aiPos = {-100,-100};
            continue;
        }

        // AI 回合
        aiPos = ai->go(manPos);
        if (chess->checkOver(aiPos, CHESS_WHITE)) {
            IMAGE lose,bk;
            loadimage(&lose,"res/s01_sb.png");
            loadimage(&bk,"res/s01_ms.png");
            putimagePNG(175,131,&bk);
            putimagePNG(95,204,&lose);
            // 播放失败提示音
            mciSendString("play res/defeat.mp3", 0, 0, 0);
            putimagePNG(193,423,&again);
            MOUSEMSG msg;
            while (1) {
                msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN &&
                    msg.x > 193 && msg.x < 443
                    && msg.y > 423 && msg.y < 513) {
                    mciSendString("play res/click.mp3", 0, 0, 0);
                    Sleep(500);
                    break;
                }
            }
            chess->init();
            aiPos = {-100,-100};
            continue;
        }
    }
}
