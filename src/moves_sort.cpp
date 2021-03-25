#include "moves_sort.h"

int HistoryTable[256][256];     // 历史表

/* 
 * 函数名：SaveHistoryTable
 * 描述：写入历史表
 * 入参: 
 * - const Movement& move   着法
 * - int depth              深度
 * 返回值：
 * - void
 * 最后更新时间: 25.03.13
 */
void SaveHistoryTable(const Movement& move, int depth){
   HistoryTable[move.from][move.to] += depth;
}