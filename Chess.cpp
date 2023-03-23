//
// Created by sansan on 2022/6/20.
//

#include "Chess.h"

#pragma comment(lib, "winmm.lib")


// 设计图片背景为透明函数
void putimagePNG(int x, int y, IMAGE *picture) //x为载入图片的X坐标，y为Y坐标
{
    // 变量初始化
    DWORD *dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
    DWORD *draw = GetImageBuffer();
    DWORD *src = GetImageBuffer(picture); //获取picture的显存指针
    int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
    int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
    int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
    int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
    int dstX = 0;    //在显存里像素的角标

    // 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
    for (int iy = 0; iy < picture_height; iy++) {
        for (int ix = 0; ix < picture_width; ix++) {
            int srcX = ix + iy * picture_width; //在显存里像素的角标
            int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
            int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight) {
                dstX = (ix + x) + (iy + y) * graphWidth; //在显存里像素的角标
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255)
                        << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
                             | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
                             | (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
            }
        }
    }
}

Chess::Chess(int gradesize, int marginX, int marginY, float chesssize) :
        boardSize(gradesize), margin_x(marginX), margin_y(marginY), chessSize(chesssize) {
    playerFlag = CHESS_BLACK;           // 黑棋先手
    // 棋盘置零
    for (int i = 0; i != boardSize; ++i) {
        std::vector<int> row(boardSize, 0);
        chessMap.push_back(row);
    }

    // 加载棋子图片   将棋子图片加载至内存 放至构造函数中，在棋局开始初把棋子图片加入到内存中
    // 亲测 此条在init成员函数中无法正确落子
    loadimage(&chessBlackImgMove, _T("res/black2.png"), chessSize, chessSize, true);
    loadimage(&chessWhiteImgMove, _T("res/white2.png"), chessSize, chessSize, true);
    loadimage(&chessBlackImgDown, _T("res/black.png"), chessSize, chessSize, true);
    loadimage(&chessWhiteImgDown, _T("res/white.png"), chessSize, chessSize, true);
    loadimage(&chessRedImg, _T("res/red.png"), chessSize, chessSize, true);
}

Chess &Chess::operator=(const Chess &chess) {
    boardSize = chess.boardSize;
    margin_x = chess.margin_x;
    margin_y = chess.margin_y;
    chessSize = chess.chessSize;
    playerFlag = chess.playerFlag;
    chessMap = chess.chessMap;
    chessBlackImgMove = chess.chessBlackImgMove;
    chessWhiteImgMove = chess.chessWhiteImgMove;
    chessBlackImgDown = chess.chessBlackImgDown;
    chessWhiteImgDown = chess.chessWhiteImgDown;
    chessRedImg = chess.chessRedImg;
    return *this;
}

Chess::Chess(const Chess &chess) {
    boardSize = chess.boardSize;
    margin_x = chess.margin_x;
    margin_y = chess.margin_y;
    chessSize = chess.chessSize;
    playerFlag = chess.playerFlag;
    chessMap = chess.chessMap;
    chessBlackImgMove = chess.chessBlackImgMove;
    chessWhiteImgMove = chess.chessWhiteImgMove;
    chessBlackImgDown = chess.chessBlackImgDown;
    chessWhiteImgDown = chess.chessWhiteImgDown;
    chessRedImg = chess.chessRedImg;
}

void Chess::init() {
    // 创建游戏窗口
    initgraph(640, 638);
    // 显示棋盘图片
    loadimage(NULL, "res/checkerboard2.jpg");
    // 播放开始提示音
    mciSendString("play res/start.WAV", 0, 0, 0);

    HCURSOR hcur = (HCURSOR) LoadImage(NULL, "res/Vert_Hnd.cur", IMAGE_CURSOR, 0, 0,
                                       LR_LOADFROMFILE);      // 加载图片库鼠标鼠标样式
    HWND hwnd = GetHWnd();                                               // 获取绘图窗口句柄
    SetClassLongPtr(hwnd, GCLP_HCURSOR, (long) hcur);// 设置窗口鼠标样式

    // 清空棋盘
    auto cleanChekerBoard = [](std::vector<int> &vi) { std::fill(vi.begin(), vi.end(), 0); };
    std::for_each(chessMap.begin(), chessMap.end(), cleanChekerBoard);

    playerFlag = true;
}

bool Chess::clickBorad(const int &x, const int &y, ChessPos &pos) {
    bool ret = false;
    // 计算点击位置与所在格子的四个顶点的距离函数
    auto calcuLen = [&](int &PointX, int &PointY) {
        return sqrt((x - PointX) * (x - PointX) + (y - PointY) * (y - PointY));
    };
    // 计算棋子所在行列
    int col = (x - margin_x) / chessSize;
    int row = (y - margin_y) / chessSize;
    double halfDiagonal = sqrt(2.0) * chessSize / 2, offset = chessSize * 0.4;         // 对角线长度的一半，作为判定落子的条件
    int PointX, PointY, toX, toY;
    double minimumLen = INF, Len;
    // 计算距离四个顶点的距离 将离对角线最小的点
    for (int i = 0; i != 2; ++i) {
        for (int j = 0; j != 2; ++j) {
            // 分别计算四个顶点在图片上的像素位置 选出距离最小的点放入pos
            PointX = margin_x + chessSize * (col + j);
            PointY = margin_y + chessSize * (row + i);
            Len = calcuLen(PointX, PointY);
            // 如果点击位置处到这个顶点的距离没有大于偏移值 则判定下一个顶点
            if (abs(x - PointX) > offset || abs(y - PointY) > offset)
                continue;
            // 如果所在位置没有棋子占位 说明该位置合法 将pos设置为该位置 并将值返回
            if (Len < halfDiagonal && minimumLen > Len && !chessMap[row + i][col + j]) {
                minimumLen = Len;
                pos.row = row + i;
                pos.col = col + j;
                ret = true;
            }
        }
    }
    return ret;
}

void Chess::ChessDown(const std::pair<int,int>&prePos,const ChessPos &pos, const chess_kind &kind) {
    int pre_x = margin_x + prePos.second * chessSize - 0.5 * chessSize;
    int pre_y = margin_y + prePos.first * chessSize - 0.5 * chessSize;

    int x = margin_x + pos.col * chessSize - 0.5 * chessSize;
    int y = margin_y + pos.row * chessSize - 0.5 * chessSize;

    if (kind == CHESS_BLACK) {
        if(pre_x > 0 && pre_y > 0)
            putimagePNG(pre_x, pre_y, &chessWhiteImgDown);
        putimagePNG(x, y, &chessBlackImgMove);
    } else {
        Sleep(1000);
        putimagePNG(pre_x, pre_y, &chessBlackImgDown);
        putimagePNG(x, y, &chessWhiteImgMove);
    }
    // 落子音
    mciSendString("play res/UI_Button.wav", 0, 0, 0);
    updateChessMap(pos);
}

bool Chess::checkOver(const std::pair<int, int> &pos, const int &kind) {
    int chessNum;
    int row = pos.first, col = pos.second;
    int size = getBoradSize();
    for (int x = -1; x != 2; ++x) {
        for (int y = -1; y != 2; ++y) {
            chessNum = 1;
            std::vector<std::pair<int,int>> winOrLose{std::make_pair(row,col)};
            if (!x && !y)continue;
            for (int i = 1; i <= 4; ++i) {
                int curRow = row + i * y;
                int curCol = col + i * x;
                // 超出棋盘范围的点跳过
                if (curRow < 0 || curCol < 0 || curRow >= size || curCol >= size )
                    continue;
                // 如果存在连续的黑棋/白棋 chessNum 加一
                if (getChessData(curRow,curCol) == kind) {
                    ++chessNum;
                    winOrLose.emplace_back(curRow,curCol);
                    if (chessNum >= 5)
                    {
                        for (auto &point:winOrLose) {
                            int x = margin_x + point.second * chessSize - 0.5 * chessSize;
                            int y = margin_y + point.first * chessSize - 0.5 * chessSize;
                            putimagePNG(x,y,&chessRedImg);
                        }
                        Sleep(2000);
                        return true;
                    }
                }
                else break;
//                printf("%d\n",chessNum);
            }
            for (int j = 1; j <= 4; ++j) {
                int curRow = row - j * y;
                int curCol = col - j * x;
                // 超出棋盘范围的点跳过
                if (curRow < 0 || curCol < 0 || curRow >= size || curCol >= size )
                    continue;
                // 如果存在连续的黑棋/白棋 chessNum 加一
                if (getChessData(curRow,curCol) == kind) {
                    ++chessNum;
                    winOrLose.emplace_back(curRow,curCol);
                    if (chessNum >= 5) {
                        for (auto &point:winOrLose) {
                            int x = margin_x + point.second * chessSize - 0.5 * chessSize;
                            int y = margin_y + point.first * chessSize - 0.5 * chessSize;
                            putimagePNG(x,y,&chessRedImg);
                        }
                        Sleep(2000);
                        return true;
                    }
                }
                else break;
            }
        }
    }
    return false;
}

int Chess::getChessData(const int &row, const int &col) const {
    return chessMap[row][col];
}

int Chess::getChessData(const ChessPos &pos) const {
    return chessMap[pos.row][pos.col];
}

int Chess::getBoradSize() const {
    return boardSize;
}

void Chess::updateChessMap(const ChessPos &pos) {
    chessMap[pos.row][pos.col] = (playerFlag ? CHESS_BLACK : CHESS_WHITE);
    playerFlag = !playerFlag;       // 交换行棋
}
