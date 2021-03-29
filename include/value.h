/* 
 * 文件名： value.h
 * 描述	：棋子在棋盘各位置的价值表示
 * 作者 : yuanthree
 * 最后更新时间: 21.03.25
 */
#include "preset.h"
#ifndef VALUE_H
#define VALUE_H

extern const char PieceNumToType[48];
extern const UINT8 PositionValues[2][7][256];
void Eval(Situation &s);

#endif