/* 
 * 文件名：moves_sort.cpp
 * 描述	：着法的排序
 * 作者 : saltgrey
 * 最后更新时间: 03.26.20
 */
#include <algorithm>
#include "moves_sort.h"
#include "representation.h"
int HistoryTable[256][256];      // 历史表
Movement KillerTable[256][2];    // 杀手表 

void ResetHistory(){
   memset(HistoryTable, 256 * 256, sizeof(int));
   memset(KillerTable, 256 * 2, sizeof(Movement));
}

/* 
 * 函数名：SaveHistoryTable
 * 描述：写入历史表
 * 入参: 
 * - const Movement& move   着法
 * - int depth              深度
 * 返回值：
 * - void
 * 最后更新时间: 21.03.25
 */
void SaveHistoryTable(const Movement& move, int depth){
   HistoryTable[move.from][move.to] += depth;
}

/* 
 * 函数名：SaveKiller
 * 描述：更新杀手着法
 * 入参: 
 * - const Movement& move   着法
 * - int step               搜索步数
 * 返回值：
 * - void
 * 最后更新时间: 21.04.06
 */
void SaveKiller(const Movement& move, int step){
   if(KillerTable[step][0].value < move.value){
      KillerTable[step][0] = move;
   }
   else if(KillerTable[step][1].value < move.value){
      KillerTable[step][1] = move;
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
      all_movements[i].value = 
         MVV_VALUES[situation.current_board[all_movements[i].to]] - 
         IfProtected(1 - situation.current_player, all_movements[i].to, situation) ? MVV_VALUES[situation.current_board[all_movements[i].from]] : 0;
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
   int i;
   for(i = flag; i < num_of_all_movements; i ++){
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
   if((!hash_move.from && !hash_move.to) || !MovementsLegal(hash_move, situation))
      no_hash_movement ++;
   if((!KillerTable[step][0].from && !KillerTable[step][0].to) || !MovementsLegal(KillerTable[step][0], situation))
      no_killer_1_movement ++;
   if((!KillerTable[step][1].from && !KillerTable[step][1].to) || !MovementsLegal(KillerTable[step][1], situation))
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
