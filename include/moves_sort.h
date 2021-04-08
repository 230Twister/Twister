#include "representation.h"

#ifndef MOVESORT_H
#define MOVESORT_H

void ResetHistory();

void SaveHistoryTable(const Movement& move, int depth);
void SaveKiller(const Movement& move, int step);

void MoveSort(const Situation & situation, int & num_of_all_movements, Movement* all_movements, Movement hash_move, int step);
void CaptureMoveSort(const Situation & situation, int & num_of_all_movements, Movement* all_movements);

#endif