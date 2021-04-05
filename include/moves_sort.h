#include "representation.h"

#ifndef MOVESORT_H
#define MOVESORT_H

extern int HistoryTable[256][256];     // 历史表

void SaveHistoryTable(const Movement& move, int depth);

void MoveSort(const Situation & situation, int & num_of_all_movements, Movement* all_movements, Movement hash_move, int step);

#endif