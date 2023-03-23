#pragma once

#include "ChessGame.h"
#include <conio.h>

int main() {
//    Man man;
//    Chess chess;
    Chess chess(13, 31, 31, 48);
    Man man(&chess);
    AI ai(&chess);
    ChessGame game(&man, &ai, &chess);

    game.play();
}


/*
#include <set>
#include <queue>

class Node {
public:
    bool is_max_node();

    void evaluate();

    unsigned int board_identify();

    void addNewNode();
public:
    // 若为叶子，记录估值得分  若为MAX节点，记录α值  若为MIN节点，记录β值
    int value;
    unsigned int depth;     // 记录当前节点深度，根节点深度为0
    unsigned int cntX, cntY;     // 记录落子点坐标
    Node *father;       // 记录当前节点的父节点
    bool (*cmp)(Node * ,Node *) = [](Node *node1,Node *node2){return node1->value<node2->value;};
    std::priority_queue<Node *,std::vector<Node*>,decltype(cmp)> children;       // 记录当前节点的子节点
    std::vector<std::vector<char>> board;
};



#define MAX 0x3f3f3f
#define MIN (-0x3f3f3f)

int alpha = MIN, beta = MAX;        // 将alpha初始化为负无穷    beta初始化为正无穷
// player：己方还是敌方  node：当前节点
int ab(int player, Node *node, int alpha, int beta) {
    if (node->children.size() == 0) return node->value;
    // 己方回合     从所有情况中寻找最大值
    if (player == 1) {
        for (auto &ele = node->children.top();!node->children.empty();node->children.pop()) {
            ele->addNewNode();
            int val = ab(~player, ele, alpha, beta);       // 将产生的局面交给对方
            if (val > alpha)
                alpha = val;
            if (alpha > beta)       // 将当前层的可取得的最大值返回
                return alpha;
        }
        return alpha;
    } else {
        for (auto &ele = node->children.top();!node->children.empty();node->children.pop()) {
            ele->addNewNode();
            int val = ab(~player, ele, alpha, beta);
            if (val < beta)
                beta = val;
            if (alpha > beta)
                return beta;    // 无论当前beta层取值多大，反正它只会将当前层的最小的值返回给alpha层
                                // 所以如果此时取到的一个任意值都比alpha小的话，alpha都不会选择这个值，而是保持原值，因此
                                // 可以停止继续在beta层搜索 相当于是剪枝的操作
                                // 这里用优先队列存储子节点，每次弹出value最小的子节点 加快减枝
        }
        return beta;
    }
}
 */