/* 
 * 文件名： value.h
 * 描述	：棋子在棋盘各位置的价值表示
 * 作者 : yuanthree
 * 最后更新时间: 21.04.18
 */
#include "preset.h"
#ifndef VALUE_H
#define VALUE_H

extern const int PieceNumToType[48];
extern const int cvlKingPawnMidgameAttacking[256];
extern const int cvlKingPawnMidgameAttackless[256];
extern const int cvlKingPawnEndgameAttacking[256];
extern const int cvlKingPawnEndgameAttackless[256];
extern const int cvlAdvisorBishopThreatless[256];
extern const int cvlAdvisorBishopThreatened[256];
extern const int cvlHorseMidgame[256];
extern const int cvlHorseEndgame[256];
extern const int cvlRookMidgame[256];
extern const int cvlRookEndgame[256];
extern const int cvlCannonMidgame[256];
extern const int cvlCannonEndgame[256];
extern const int cvlHollowThreat[16];
extern const int cvlCentralThreat[16];
extern const int cvlBottomThreat[16];

bool WhiteHalf(int i);
bool BlackHalf(int i);
int SideValue(int sd, int vl);
void PreEvaluate(Situation &s);
void RookMobility(Situation &s);
void Evaluate(Situation &s);

#endif