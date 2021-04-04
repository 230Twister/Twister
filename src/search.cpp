#include "search.h"
#include "hash_table.h"
#include "moves_sort.h"
#include <ctime>

Movement BestMove;                                      // 当前局面的最好走法
int step = 0;                                           // 搜索步数
time_t StartTime;                                       // 开始搜索的时间
bool isTimeLimit = 0;                                   // 时间是否超限
const UINT16 MAX_DEPTH = 8;                             // 最大搜索深度
const UINT32 MAX_TIME = 20000;                          // 最大消耗时间(ms)
extern const UINT32 MAX_VALUE = 10000;                  // 最大价值，胜利局面绝对分数
extern const UINT32 WIN_VALUE = MAX_VALUE - MAX_DEPTH;  // 胜利局面的相对分数
/* 
 * 函数名：AlphaBetaSearch
 * 描述：带置换表的AlphaBeta搜索
 * 入参: 
 * - int depth  深度
 * - int alpha  alpha值
 * - int beta   beta值
 * 返回值：
 * - int 最佳分值
 * 最后更新时间: 26.03.14
 */
int AlphaBetaSearch(int depth, int alpha, int beta){
    int value;                  // 下一着法的分值
    int best;                   // 所有着法中的最佳分值
    Movement move_list[128];    // 当前所有着法
    Movement move;              // 当前着法
    Movement good_move;         // 当前局面最佳着法
    move = good_move = NONE_MOVE;
    // 读取置换表
    value = ReadHashTable(depth, alpha, beta, move);

    // 读取成功
    if(value != NONE_VALUE){
        return value;
    }
    // 时间检测，避免超限
    if(isTimeLimit && clock() - StartTime > MAX_TIME){
        isTimeLimit = 1;
        return NONE_VALUE;
    }
    
    best = -NONE_VALUE;
    bool isAlpha = true;    // 当前结点是否为Alpha结点

    // 到达搜索深度
    if(depth <= 0){
        // 静态搜索评估
        value = QuiescentSearch(alpha, beta);
        // 保存到置换表
        SaveHashTable(depth, value, hashEXACT, NONE_MOVE);
        return value;
    }
    int move_num = 0;       // 着法数量
    // 生成着法
    // do-something
    for(int i = 0; i < move_num; i++){
        move = move_list[i];
        // 下子
        /* do-something */
        value = -AlphaBetaSearch(depth - 1, alpha, beta);
        // 回溯
        /* do-something */

        // 当前为beta结点，执行剪枝
        if(value >= beta){
            // 此着法是好着法，记录进历史表
            SaveHistoryTable(move, depth);
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
                if(depth == MAX_DEPTH)
                    BestMove = move;
            }
        }
    }

    // 当前结点为alpha结点
    if(isAlpha){
        SaveHashTable(depth, alpha, hashALPHA, good_move);
    }
    // 当前结点为PV结点
    else{
        // 将PV结点中的最好着法，记录进历史表
        SaveHistoryTable(good_move, depth);
        SaveHashTable(depth, best, hashEXACT, good_move);
    }
    return alpha;
}

/* 
 * 函数名：QuiescentSearch
 * 描述：静态搜索
 * 入参: 
 * - int depth  深度
 * - int alpha  alpha值
 * - int beta   beta值
 * 返回值：
 * - int 最佳分值
 * 最后更新时间: 26.03.15
 */
int QuiescentSearch(int alpha, int beta){
    int value;                  // 下一着法的分值
    int best;                   // 所有着法中的最佳分值
    Movement move;              // 当前着法
    Movement move_list[128];    // 当前所有着法

    best = step - MAX_VALUE;
    // 必输局面直接返回
    if(best > beta){
        return beta;
    }
    
    // 调用评估函数进行评估
    /* do-something */
    if(value > beta)
        return beta;
    if(value > alpha)
        alpha = value;
    
    int move_num = 0;       // 着法数量
    // 生成所有吃子着法
    /* do-something */
    for(int i = 0; i < move_num; i++){
        move = move_list[i];
        // 下子
        /* do-something */
        value = -QuiescentSearch(alpha, beta);
        // 回溯
        /* do-something */
        if(value >= beta){
            return beta;
        }
        if(value > best){
            best = value;
            if(value > alpha){
                alpha = value;
            }
        }
    }
    return alpha;
}

/* 
 * 函数名：ComputerThink
 * 描述：电脑迭代加深思考，获取最佳走法
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 25.03.21
 */
void ComputerThink(){
    Movement best_move_backup;
    StartTime = clock();
    ResetHashTable();

    for(int max_depth = 1; max_depth <= MAX_DEPTH; max_depth++){
        AlphaBetaSearch(max_depth, -NONE_VALUE, NONE_VALUE);
        if(isTimeLimit){
            break;
        }
        else{
            best_move_backup = BestMove;
        }
    }

    // 正式执行下子
    /* do-something */
}