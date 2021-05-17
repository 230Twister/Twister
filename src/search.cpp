#include "search.h"
#include "hash_table.h"
#include "moves_sort.h"
#include "representation.h"
#include "moves_sort.h"
#include "value.h"
#include "book.h"
#include <ctime>
#include <fstream>

Movement BestMove;                                      // 当前局面的最好走法
int step = 0;                                           // 搜索步数
time_t StartTime;                                       // 开始搜索的时间
bool isTimeLimit = 0;                                   // 时间是否超限
bool UseBook = true;
int NowMaxDepth;
const int NULL_REDUCTION = 2;                           // 空着裁剪度
const int DRAW_VALUE = -10;                             // 和棋分数
const UINT16 MAX_DEPTH = 24;                            // 最大搜索深度
const time_t MAX_TIME = 40000;                          // 最大消耗时间(ms)
extern const int MAX_VALUE = 10000;                     // 最大价值，胜利局面绝对分数
extern const int WIN_VALUE = MAX_VALUE - MAX_DEPTH;     // 胜利局面的相对分数

int debug_value;
int all = 0;

int SearchCut(Situation& situation, int depth, int beta, bool allowNullMove = false){
    int value;                      // 下一着法的分值
    int best;                       // 所有着法中的最佳分值
    Movement move_list[128];        // 当前所有着法
    Movement move;                  // 当前着法
    Movement good_move;             // 当前局面最佳着法
    move = good_move = NONE_MOVE;
    best = -NONE_VALUE;

    // 到达搜索深度
    if(depth <= 0){
        // 静态搜索评估
        return QuiescentSearch(situation, beta - 1, beta);
    }

    if(step - MAX_VALUE >= beta)
        return beta;

    // 重复裁剪
    if(CheckRepeat(situation, step)) 
        return DRAW_VALUE;

    // 置换表裁剪
    value = ReadHashTable(depth, beta - 1, beta, move);
    if(value != NONE_VALUE){
        return value;
    }

    // 空着裁剪(带检验)
    if(!situation.banNullMove && allowNullMove && !BeChecked(situation)){
        MakeNullMove(situation);
        value = -SearchCut(situation, depth - 1 - NULL_REDUCTION, 1 - beta);
        UnMakeNullMove(situation);

        if(value >= beta && SearchCut(situation, depth - NULL_REDUCTION, beta) >= beta){
            SaveHashTable(max(depth, NULL_REDUCTION), beta, hashBETA, NONE_MOVE);
            return beta;
        }
    }

    all++;

    // 时间检测，避免超限
    if(isTimeLimit || clock() - StartTime > MAX_TIME){
        isTimeLimit = 1;
        return NONE_VALUE;
    }

    int move_num = 0;       // 着法数量
    // 生成着法
    MoveSort(situation, move_num, move_list, move, step);
    for(int i = 0; i < move_num; i++){
        move = move_list[i];
        if(move.from == 0 && move.to == 0) continue;
        // 下子
        if(MakeAMove(situation, move)){
            value = -MAX_VALUE + step;
        }
        else{
            value = -SearchCut(situation, depth - 1, 1 - beta, true);
        }
        // 回溯
        UnMakeAMove(situation);

        if(value > best){
            best = value;           // 更新最佳分数
            good_move = move;       // 更新最佳着法
            if(value >= beta){
                // 此着法是好着法，记录进历史表和杀手表
                SetBestMove(move, step, depth);
                SaveHashTable(depth, beta, hashBETA, move);
                return beta;
            }
        }
    }

    SaveHashTable(depth, beta - 1, hashALPHA, good_move);
    return beta - 1;
}

/* 
 * 函数名：PVSearch
 * 描述：带置换表的主要变例搜索
 * 入参: 
 * - int depth  深度
 * - int alpha  alpha值
 * - int beta   beta值
 * 返回值：
 * - int 最佳分值
 * 最后更新时间: 21.04.05
 */
int PVSearch(Situation& situation, int depth, int alpha, int beta, Movement& inspire){
    int value;                      // 下一着法的分值
    int best;                       // 所有着法中的最佳分值
    Movement move_list[128];        // 当前所有着法
    Movement move;                  // 当前着法
    Movement good_move;             // 当前局面最佳着法
    Movement inspire_move;          // 内部迭代启发着法
    move = good_move = inspire_move = NONE_MOVE;

    // 到达搜索深度
    if(depth <= 0){
        // 静态搜索评估
        return QuiescentSearch(situation, alpha, beta);
    }

    if(step - MAX_VALUE >= beta)
        return beta;

    // 重复裁剪
    if(CheckRepeat(situation, step)) 
        return DRAW_VALUE;

    // 置换表裁剪
    value = ReadHashTable(depth, alpha, beta, move);
    if(value != NONE_VALUE && value > -MAX_VALUE){
        return value;
    }

    // 内部迭代加深启发(在置换表未能给出启发走法时使用)
    if(depth > 2 && move.from == 0){
        value = PVSearch(situation, depth / 2, alpha, beta, inspire_move);
        if(value <= alpha){
            value = PVSearch(situation, depth / 2, -NONE_VALUE, beta, inspire_move);
        }
        move = inspire_move;
    }

    all++;

    // 时间检测，避免超限
    if(isTimeLimit || clock() - StartTime > MAX_TIME){
        isTimeLimit = 1;
        return NONE_VALUE;
    }
    
    best = -NONE_VALUE;
    bool isAlpha = true;    // 当前结点是否为Alpha结点
    bool PVflag = false;

    int move_num = 0;       // 着法数量
    // 生成着法
    MoveSort(situation, move_num, move_list, move, step);
    for(int i = 0; i < move_num; i++){
        move = move_list[i];
        if(move.from == 0 && move.to == 0) continue;
        // 下子
        if(MakeAMove(situation, move)){
            value = -MAX_VALUE + step;
        }
        else{
            if(PVflag){
                value = -SearchCut(situation, depth - 1, -alpha);
                if(value > alpha && value < beta)
                    value = -PVSearch(situation, depth - 1, -beta, -alpha, inspire);
            }
            else
                value = -PVSearch(situation, depth - 1, -beta, -alpha, inspire);
        }
        // 回溯
        UnMakeAMove(situation);

        // 当前为beta结点，执行剪枝
        if(value >= beta){
            inspire = move;
            // 此着法是好着法，记录进历史表和杀手表
            SetBestMove(move, step, depth);
            SaveHashTable(depth, beta, hashBETA, move);
            return beta;
        }
        if(value > best){
            best = value;           // 更新最佳分数
            good_move = move;       // 更新最佳着法
            inspire = good_move;
            if(value > alpha){
                isAlpha = false;
                PVflag = true;
                alpha = value;
            }
        }
    }

    // 当前结点为alpha结点
    if(isAlpha){
        SaveHashTable(depth, alpha, hashALPHA, good_move);
    }
    // 当前结点为PV结点
    else{
        // 将PV结点中的最好着法，记录进历史表和杀手表
        SetBestMove(good_move, step, depth);
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
 * 最后更新时间: 21.03.26
 */
int QuiescentSearch(Situation& situation, int alpha, int beta){
    int value;                  // 下一着法的分值
    int best;                   // 所有着法中的最佳分值
    Movement move;              // 当前着法
    Movement move_list[64];     // 当前所有着法

    best = step - MAX_VALUE;
    // 必输局面直接返回
    if(best >= beta){
        return beta;
    }
    
    // 调用评估函数进行评估
    value = Evaluate(situation, alpha, beta);
    if(value > beta)
        return beta;
    if(value > alpha)
        alpha = value;
    
    int move_num = 0;       // 着法数量
    // 生成所有吃子着法
    CaptureMoveSort(situation, move_num, move_list);
    for(int i = 0; i < move_num; i++){
        move = move_list[i];
        // 下子
        if(MakeAMove(situation, move)){
            value = -MAX_VALUE + step;
        }
        else{
            value = -QuiescentSearch(situation, -beta, -alpha);
        }
        // 回溯
        UnMakeAMove(situation);
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

int SearchRoot(Situation& situation, int depth){
    int value;                      // 下一着法的分值
    int best;                       // 所有着法中的最佳分值
    Movement move_list[128];        // 当前所有着法
    Movement move;                  // 当前着法
    Movement good_move;             // 当前局面最佳着法
    Movement inspire;
    move = good_move = inspire = NONE_MOVE;
    best = -NONE_VALUE;

    int move_num = 0;               // 着法数量
    // 生成着法
    MoveSort(situation, move_num, move_list, move, step);
    for(int i = 0; i < move_num; i++){
        move = move_list[i];
        if(move.from == 0 && move.to == 0) continue;
        // 下子
        if(MakeAMove(situation, move)){
            value = -MAX_VALUE + step;
        }
        else{
            if(best != -NONE_VALUE){
                value = -SearchCut(situation, depth - 1, -best);
                if(value > best)
                    value = -PVSearch(situation, depth - 1, -NONE_VALUE, -best, inspire);
            }
            else
                value = -PVSearch(situation, depth - 1, -NONE_VALUE, NONE_VALUE, inspire);
        }
        // 回溯
        UnMakeAMove(situation);

        if(value > best){
            best = value;           // 更新最佳分数
            good_move = move;       // 更新最佳着法
        }
    }

    BestMove = good_move;
    return best;
}

/* 
 * 函数名：ComputerThink
 * 描述：电脑迭代加深思考，获取最佳走法
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.04.05
 */
void ComputerThink(Situation& situation){
    int value, max_depth;
    Movement best_move_save;
    isTimeLimit = false;
    step = 0;
    ResetHashTable();   // 清空置换表
    ResetHistory();     // 清空历史表和杀手表
    StartTime = clock();

    // 读取开局库
    if(UseBook){
        Movement book_move = ReadBookTable(situation);
        if(book_move.from != 0 && MovementsLegal(book_move, situation)){
            printf ("bestmove %s\n", MovementToStr(book_move).c_str());
		    fflush (stdout);
            return;
        }
        else{
            UseBook = false;
        }
    }

    // 迭代加深搜索
    for(max_depth = 1; max_depth <= MAX_DEPTH; max_depth++){
        NowMaxDepth = max_depth;
        value = SearchRoot(situation, max_depth);
        if(isTimeLimit){
            break;
        }
        else{
            best_move_save = BestMove;
            debug_value = value;
            // 输出日志文件(每层搜索结果)
            std::ofstream f;
            f.open("debug.log", std::ios::app | std::ios::out);
            f << "Depth: " << max_depth << " Time: " << clock() - StartTime << "ms \n";
            f.close();
        }
        if(value > WIN_VALUE) break;
    }

    // 必败 认输
    if(best_move_save.from == best_move_save.to){
        printf ( "nobestmove\n" ); // 认输
		fflush (stdout);
    }
    // 输出最优着法
    else{
        printf ("bestmove %s\n", MovementToStr(best_move_save).c_str());
		fflush (stdout);
    }

    // 输出日志文件(返回最佳着法)
    std::ofstream f;
    f.open("debug.log", std::ios::app | std::ios::out);
    f << "Bestmove's value: " << debug_value << '\n';
    f << "=========================================\n";
    f.close();
    cout << all << endl;
    return ;
}