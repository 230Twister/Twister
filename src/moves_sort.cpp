/* 
 * 文件名：moves_sort.cpp
 * 描述	：着法的排序
 * 作者 : saltgrey
 * 最后更新时间: 03.26.20
 */
#include <algorithm>
#include "moves_sort.h"
#include "representation.h"

UINT32 HistoryTable[256][256];      // 历史表
Movement KillerTable[256][2];    // 杀手表 

const int HASH_GET = 1;
const int CAP_GEN = 2;
const int CAP_GET = 3;
const int KILL1_GET = 4;
const int KILL2_GET = 5;
const int NOCAP_GEN = 6;
const int NOCAP_GET = 7;
const UINT32 MAX_MOVE_VALUE = 1 << 31;

void ResetHistory(){
    memset(HistoryTable, 0, 256 * 256 * sizeof(int));
    memset(KillerTable, 0, 256 * 2 * sizeof(Movement));
}

/* 
 * 函数名：SetBestMove
 * 描述：保存好着法进杀手表和历史表
 * 入参: 
 * - const Movement& move   着法
 * - int depth              深度
 * 返回值：
 * - void
 * 最后更新时间: 21.05.16
 */
void SetBestMove(const Movement& move, int step, int depth){
    HistoryTable[move.from][move.to] += depth * depth;
    if(!isMoveEqual(move, KillerTable[step][0])){
        KillerTable[step][1] = KillerTable[step][0];
        KillerTable[step][0] = move;
    }
}

/* 
 * 函数名：CaptureValue
 * 描述：吃子走法价值评估
 * 入参: 
 * - const Situation & situation       局面
 * - int & num_of_all_movements        当前着法数
 * - Movement* all_movements           当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 03.26.20
 */
void CaptureValue(const Situation & situation, int & num_of_all_movements, Movement* all_movements){
    int i;
    for(i = 0; i < num_of_all_movements; i ++){
        int mvv =  MVV_VALUES[situation.current_board[all_movements[i].to]];
        int lva = IfProtected(1 - situation.current_player, all_movements[i].to, situation) ? MVV_VALUES[situation.current_board[all_movements[i].from]] : 0;
        if(mvv >= lva){
            all_movements[i].value = mvv - lva + 1;
        }
        else{
            all_movements[i].value = (mvv >= 3 || InHomeHalf(all_movements[i].to, situation.current_player)) ? 1 : 0;
        }
    }
}

/* 
 * 函数名：NoCaptureValue
 * 描述：不吃子走法价值评估
 * 入参: 
 * - const Situation & situation       局面
 * - int & num_of_all_movements        当前着法数
 * - Movement* all_movements           当前着法数组，存储当前所有着法
 * - int flag                          中间标记
 * 返回值：
 * - void
 * 最后更新时间: 03.26.20
 */
void NoCaptureValue(const Situation & situation, int & num_of_all_movements, Movement* all_movements, int flag){
    for(int i = flag; i < num_of_all_movements; i ++){
        all_movements[i].value = HistoryTable[all_movements[i].from][all_movements[i].to];
    }
}

bool cmp(Movement& a, Movement& b){
    return a.value > b.value;
}

/* 
 * 函数名：MoveSort
 * 描述：着法排序
 * 入参: 
 * - const Situation & situation       局面
 * - int & num_of_all_movements        当前着法数
 * - Movement* all_movements           当前着法数组，存储当前所有着法
 * - Movement hash_move                置换表
 * - int step                          搜索步数
 * 返回值：
 * - void
 * 最后更新时间: 03.26.20
 */
void MoveSort(const Situation & situation, int & num_of_all_movements, Movement* all_movements, Movement hash_move, int step){
    int flag;
    int no_hash_movement = 0, no_killer_1_movement = 0, no_killer_2_movement = 0, no_movement = 0;

    // 判断置换表和杀手着法是否为空
    if(!MovementsLegal(hash_move, situation))
        no_hash_movement ++;
    if(!MovementsLegal(KillerTable[step][0], situation))
        no_killer_1_movement ++;
    if(!MovementsLegal(KillerTable[step][1], situation))
        no_killer_2_movement ++;
    no_movement = no_hash_movement + no_killer_1_movement + no_killer_2_movement;

    if(!no_hash_movement)
        all_movements[0] = hash_move;
    if(!no_killer_1_movement)
        all_movements[1-no_hash_movement] = KillerTable[step][0];
    if(!no_killer_2_movement)
        all_movements[2-no_hash_movement-no_killer_1_movement] = KillerTable[step][1];
    // 更新着法总数
    num_of_all_movements += (3 - no_movement);

    // 吃子着法排序
    GetAllCaptureMovements(situation, num_of_all_movements, all_movements);
    CaptureValue(situation, num_of_all_movements, all_movements);
    std::sort(all_movements + 3 - no_movement, all_movements + num_of_all_movements, cmp);
    // 不吃子着法排序
    flag = num_of_all_movements;
    // while(all_movements[flag - 1].value < 1 && flag > (3 - no_movement)) flag--;
    GetAllNotCaptureMovements(situation, num_of_all_movements, all_movements);
    NoCaptureValue(situation, num_of_all_movements, all_movements, flag);
    std::sort(all_movements + flag, all_movements + num_of_all_movements, cmp);

    // 过滤可能的重复走法
    if(no_movement < 3){
        for(int i = 3 - no_movement; i < num_of_all_movements; i++){
            for(int j = 0; j < 3 - no_movement; j++){
                if(isMoveEqual(all_movements[i], all_movements[j])){
                // 出现重复需标记删除
                all_movements[i].from = all_movements[i].to = 0;
                break;
                }
            }
        }
    }
}

/* 
 * 函数名：CaptureMoveSort
 * 描述：吃子着法排序
 * 入参: 
 * - const Situation & situation       局面
 * - int & num_of_all_movements        当前着法数
 * - Movement* all_movements           当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.04.06
 */
void CaptureMoveSort(const Situation& situation, int& num_of_all_movements, Movement* all_movements){
    // 吃子着法排序
    GetAllCaptureMovements(situation, num_of_all_movements, all_movements);
    CaptureValue(situation, num_of_all_movements, all_movements);
    std::sort(all_movements, all_movements + num_of_all_movements, cmp);
}

/* 
 * 函数名：InitRootMove
 * 描述：生成根节点着法
 * 入参: 
 * - Situation & situation             局面
 * - int & num_of_all_movements        当前着法数
 * - Movement* all_movements           当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.05.26
 */
void InitRootMove(Situation& situation, int& num_of_all_movements, Movement* all_movements){
    // 吃子着法
    GetAllCaptureMovements(situation, num_of_all_movements, all_movements);
    CaptureValue(situation, num_of_all_movements, all_movements);
    std::sort(all_movements, all_movements + num_of_all_movements, cmp);
    int numofnocap = num_of_all_movements;
    // 不吃子着法
    GetAllNotCaptureMovements(situation, num_of_all_movements, all_movements);
    for(int i = numofnocap; i < num_of_all_movements; i ++){
        // 每个着法都进行试探，过滤不合法着法
        if(MakeAMove(situation, all_movements[i], 1)){
            // std::cout << "remove " << int(all_movements[i].from) << " " << int(all_movements[i].to) << " \n";
            for(int j = i; j < num_of_all_movements - 1; j ++){
                all_movements[j] = all_movements[j + 1];
            }
            num_of_all_movements --;
            i--;
        }
        UnMakeAMove(situation);
    }
    situation.moves_stack.clear();
}

/* 
 * 函数名：SortRootMove
 * 描述：对根节点着法进行排序
 * 入参: 
 * - int   num_of_all_movements        当前着法数
 * - Movement* all_movements           当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.05.26
 */
void SortRootMove(int num_of_all_movements, Movement* all_movements){
    // 对着法做历史表启发
    for(int i = 0; i < num_of_all_movements; i++){
        if(all_movements[i].value < MAX_MOVE_VALUE - 24){
            all_movements[i].value = HistoryTable[all_movements[i].from][all_movements[i].to];
        }
    }
    std::sort(all_movements, all_movements + num_of_all_movements, cmp);
}

/* 
 * 函数名：UpdateRootMove
 * 描述：更新根节点着法的价值
 * 入参: 
 * - int   num_of_all_movements        当前着法数
 * - Movement* all_movements           当前着法数组，存储当前所有着法
 * - Movement& move                    需要给予最大分值，在下次迭代中首先搜索的着法
 * 返回值：
 * - void
 * 最后更新时间: 21.05.26
 */
void UpdateRootMove(int num_of_all_movements, Movement* all_movements, Movement& move){
    for(int i = 0; i < num_of_all_movements; i++){
        if(isMoveEqual(move, all_movements[i])){
            move.value = MAX_MOVE_VALUE;
        }
        else if(move.value){
            move.value--;
        }
    }
}
