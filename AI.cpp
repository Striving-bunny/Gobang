//
// Created by sansan on 2022/6/20.
//

#include "AI.h"

void endgame(int &chessNum, int &emptyNum, const int &row, const int &col,
             const chess_kind &kind, int &i, const int &j, Chess *const chess, const std::string &op) {
    int size = chess->getBoradSize();
    // 黑棋/白棋在当前位置落子将会构成的棋局形式   从当前位置出发的连续的四个点进行计算
    for (int x = 1; x <= 4; ++x) {
        int curRow = (op == "+" ? row + x * j : row - x * j);
        int curCol = (op == "+" ? col + x * i : col - x * i);
        // 超出棋盘范围的点跳过
        if (curRow < 0 || curCol < 0 || curRow >= size || curCol >= size) continue;
        // 如果存在连续的黑棋personNum 加一
        if (chess->getChessData(curRow, curCol) == kind) ++chessNum;
            // 如果当前位置没有棋子 emptyNum加一
        else if (!chess->getChessData(curRow, curCol)) {
            ++emptyNum;
            break;
        }
            // 当前位置为白棋/黑棋的情况
        else
            break;
    }
}

AI::AI(Chess *chess) {
    this->chess = chess;
    int chekerBoardSize = this->chess->getBoradSize();
    for (int i = 0; i != chekerBoardSize; ++i) {
        std::vector<int> row(chekerBoardSize, 0);
        scoreMap.push_back(row);
    }
}

std::pair<int, int> AI::go(std::pair<int, int> prePos) {
    ChessPos pos = think();
    chess->ChessDown(prePos, pos, CHESS_WHITE);
    return std::make_pair(pos.row, pos.col);
}

void AI::calculateScore() {
    int personNum = 0;  // 棋手方（黑色）有多少个连续棋子
    int aiNum = 0;      // AI方（白色）有多少个连续棋子
    int emptyNum = 0;   // 该方向上空白位置的个数

    // 评分向量数组清零
    auto cleanScoreMap = [](std::vector<int> &row) { std::fill(row.begin(), row.end(), 0); };
    std::for_each(scoreMap.begin(), scoreMap.end(), cleanScoreMap);

    int size = chess->getBoradSize();
    for (int row = 0; row != size; ++row) {
        for (int col = 0; col != size; ++col) {
            // 如果当前位置已经落子 跳过
            if (chess->getChessData(row, col)) continue;
            // 对当前点的8个方向进行判断 i表示行 j表示列
            for (int i = -1; i != 2; ++i) {
                for (int j = -1; j != 2; ++j) {
                    personNum = 0;
                    aiNum = 0;
                    emptyNum = 0;
                    if (!i && !j) continue;
                    // 黑棋在当前位置落子将会构成的棋局形式   从当前位置出发的连续的四个点进行计算
                    endgame(personNum, emptyNum, row, col, CHESS_BLACK, i, j, chess, "+");
                    /*
                    // 黑棋在当前位置落子将会构成的棋局形式   从当前位置出发的连续的四个点进行计算
                    for (int x = 1; x <= 4; ++x) {
                        int curRow = row + x * j;
                        int curCol = col + x * i;
                        // 超出棋盘范围的点跳过
                        if (curRow < 0 || curCol < 0 || curRow >= size || curCol >= size) continue;
                        // 如果存在连续的黑棋 personNum加一
                        if (chess->getChessData(curRow, curCol) == CHESS_BLACK) ++personNum;
                            // 如果当前位置没有棋子 emptyNum加一 并退出黑棋在此方向的判断
                        else if (!chess->getChessData(curRow, curCol)) {
                            ++emptyNum;
                            break;
                        }
                            // 当前位置为白子的情况 直接退出
                        else break;
                    }
                     */
                    // 反向计算
                    endgame(personNum, emptyNum, row, col, CHESS_BLACK, i, j, chess, "-");
                    /*
                    // 反向计算
                    for (int x = 1; x <= 4; ++x) {
                        int curRow = row - x * j;
                        int curCol = col - x * i;
                        // 超出棋盘范围的点跳过
                        if (curRow < 0 || curCol < 0 || curRow >= size || curCol >= size) continue;
                        // 如果存在连续的黑棋 personNum加一
                        if (chess->getChessData(curRow, curCol) == CHESS_BLACK) ++personNum;
                            // 如果当前位置没有棋子 emptyNum加一 并退出黑棋在此方向的判断
                        else if (!chess->getChessData(curRow, curCol)) {
                            ++emptyNum;
                            break;
                        }
                            // 当前位置为白子的情况 直接退出
                        else break;
                    }
                     */

                    // 黑棋在当前位置落子累分
                    switch (personNum) {
                        case 1:     // 连二
                            scoreMap[row][col] += 10;
                            break;
                        case 2:     // 连三
                            if (emptyNum == 1)     // 死三
                            {
                                scoreMap[row][col] += 30;
                            } else if (emptyNum == 2)    // 活三
                            {
                                scoreMap[row][col] += 40;
                            }
                            break;
                        case 3:     // 连四
                            if (emptyNum == 1)     // 死四
                            {
                                scoreMap[row][col] += 60;
                            } else if (emptyNum == 2)    // 活四
                            {
                                scoreMap[row][col] += 200;
                            }
                            break;
                        case 4:     // 五连子 黑棋赢
                            scoreMap[row][col] += 20000;
                            break;
                        default:
                            break;
                    }
                    /*
                    if (personNum == 1) {// 连二
                        scoreMap[row][col] += 10;
                    } else if (personNum == 2) {// 连三
                        if (emptyNum == 1)     // 死三
                        {
                            scoreMap[row][col] += 30;
                        } else if (emptyNum == 2)    // 活三
                        {
                            scoreMap[row][col] += 40;
                        }
                    } else if (personNum == 3) {// 连四
                        if (emptyNum == 1)     // 死四
                        {
                            scoreMap[row][col] += 60;
                        } else if (emptyNum == 2)    // 活四
                        {
                            scoreMap[row][col] += 200;
                        }
                    } else if (personNum == 4)   // 五连子  黑棋赢棋
                    {
                        scoreMap[row][col] += 20000;
                    }
                     */

                    // 白棋在当前位置落子将会构成的棋局形式   从当前位置出发的连续的四个点进行计算
                    aiNum = 0;
                    emptyNum = 0;
                    endgame(aiNum, emptyNum, row, col, CHESS_WHITE, i, j, chess, "+");
                    /*
                    // 白棋在当前位置落子将会构成的棋局形式   从当前位置出发的连续的四个点进行计算
                    emptyNum = 0;

                    for (int x = 1; x <= 4; ++x) {
                        int curRow = row + x * j;
                        int curCol = col + x * i;// 超出棋盘范围的点跳过
                        if (curRow < 0 || curCol < 0 || curRow >= size || curCol >= size) continue;
                        // 如果存在连续的黑棋 personNum加一
                        if (chess->getChessData(curRow, curCol) == CHESS_WHITE) ++aiNum;
                        else if (!chess->getChessData(curRow, curCol)) {
                            ++emptyNum;
                            break;
                        } else break;
                    }

                    for (int x = 1; x <= 4; ++x) {
                        int curRow = row - x * j;
                        int curCol = col - x * i;// 超出棋盘范围的点跳过
                        if (curRow < 0 || curCol < 0 || curRow >= size || curCol >= size) continue;
                        // 如果存在连续的黑棋 personNum加一
                        if (chess->getChessData(curRow, curCol) == CHESS_WHITE) ++aiNum;
                        else if (!chess->getChessData(curRow, curCol)) {
                            ++emptyNum;
                            break;
                        } else break;
                    }
                     */
                    // 反向计算
                    endgame(aiNum, emptyNum, row, col, CHESS_WHITE, i, j, chess, "-");

                    // 白棋在当前位置落子累分
                    switch (aiNum) {
                        case 0:
                            scoreMap[row][col] += 5;
                        case 1:     // ai连二
                            scoreMap[row][col] += 10;
                            break;
                        case 2:     // ai连三
                            if (emptyNum == 1)     // 死三
                            {
                                scoreMap[row][col] += 25;
                            } else if (emptyNum == 2)    // 活三
                            {
                                scoreMap[row][col] += 80;
                            }
                            break;
                        case 3:     // ai连四
                            if (emptyNum == 1)     // 死四
                            {
                                scoreMap[row][col] += 500;
                            } else if (emptyNum == 2)    // 活四
                            {
                                scoreMap[row][col] += 1000;
                            }
                            break;
                        case 4:     // 五连子 ai赢
                            scoreMap[row][col] += 30000;
                            break;
                        default:
                            break;
                    }
                    /*
                    if (!aiNum) { scoreMap[row][col] += 5; }
                    else if (aiNum == 1) {// ai连二
                        scoreMap[row][col] += 10;
                    } else if (aiNum == 2) {// ai连三
                        if (emptyNum == 1)    // 死三
                        {
                            scoreMap[row][col] += 25;
                        } else if (emptyNum == 2)    // 活三
                        { scoreMap[row][col] += 50; }
                    } else if (aiNum == 3) {// 连四
                        if (emptyNum == 1)     // 死四
                        {
                            scoreMap[row][col] += 55;
                        } else if (emptyNum == 2) {
                            scoreMap[row][col] += 10000;
                        }
                    } else if (aiNum >= 4) {
                        scoreMap[row][col] += 30000;
                    }
                     */
                }
            }
        }
    }
}

ChessPos AI::think() {
    calculateScore();
    int size = chess->getBoradSize();

    std::vector<std::pair<int, int>> maxPoints;
    int maxScore = 0;
    for (int row = 0; row != size; ++row) {
        for (int col = 0; col != size; ++col) {
            // 如果当前位置没有放置棋子
            if (!chess->getChessData(row, col)) {
                // 选出评估分数最高的点
                if (scoreMap[row][col] > maxScore) {
                    maxPoints.clear();
                    maxScore = scoreMap[row][col];
                    maxPoints.emplace_back(row, col);
                } else if (scoreMap[row][col] == maxScore) {
                    maxPoints.emplace_back(row, col);
                }
            }
        }
    }
    // 在所有最高的相同分数值的随机返回一个
    static std::default_random_engine e;
    int idx = e() % maxPoints.size();
    ChessPos ret{maxPoints[idx].first, maxPoints[idx].second};
    return ret;
}
