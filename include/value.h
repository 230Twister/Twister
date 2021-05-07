/* 
 * 文件名： value.h
 * 描述	：棋子在棋盘各位置的价值表示
 * 作者 : yuanthree
 * 最后更新时间: 21.04.18
 */
#include "preset.h"
#ifndef VALUE_H
#define VALUE_H

extern const int PIECE_NUM_TO_TYPE[48];
extern int PiecesValue[2][7][256];

void PreEvaluate(Situation &s);
int AdvisorShape(Situation& s);
int StringHold(Situation& s);
int RookMobility(Situation &s);
int Evaluate(Situation &s);

#endif