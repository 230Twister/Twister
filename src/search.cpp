#include "search.h"
#include "hash_table.h"

/* 
 * 函数名：AlphaBetaSearch
 * 描述：带置换表的AlphaBeta搜索
 * 入参: 
 * - int depth  深度
 * - int alpha  alpha值
 * - int beta   beta值
 * 返回值：
 * - int 最佳分值
 * 最后更新时间: 25.03.11
 */
int AlphaBetaSearch(int depth, int alpha, int beta){
    int value;                  // 下一着法的分值
    int best;                   // 所有着法中的最佳分值
    Movement move_list[128];    // 当前所有着法
    Movement move;              // 当前着法
    Movement good_move;         // 当前局面最佳着法
    move = good_move = NONE_MOVE;
    // 读取置换表
    value = ReadHashTable(depth, alpha, beta, good_move);

    // 读取成功
    if(value != NONE_VALUE){
        return value;
    }
    
    best = -NONE_VALUE;
    bool isAlpha = true;    // 当前结点是否为Alpha结点

    // 到达搜索深度
    if(depth <= 0){
        // 评估
        // do-something
        // 保存到置换表
        SaveHashTable(depth, value, hashEXACT, NONE_MOVE);
        return value;
    }
    else{
        int move_num = 0;       // 着法数量
        // 生成着法
        // do-something
        for(int i = 0; i < move_num; i++){
            move = move_list[i];
            // 下子 do-something
            value = -AlphaBetaSearch(depth - 1, alpha, beta);
            // 回溯 do-something

            // 当前为beta结点，执行剪枝
            if(value >= beta){
                SaveHashTable(depth, beta, hashBETA, move);
                return beta;
            }
            if(value > best){
                best = value;           // 更新最佳分数
                good_move = move;       // 更新最佳着法
                if(value > alpha){
                    isAlpha = false;
                    alpha = value;
                    // 若为第一层，传出着法
                    // do-something
                }
            }
        }

        // 当前结点为alpha结点
        if(isAlpha){
            SaveHashTable(depth, alpha, hashALPHA, good_move);
        }
        // 当前结点为PV结点
        else{
            SaveHashTable(depth, best, hashEXACT, good_move);
        }
        return alpha;
    }
}