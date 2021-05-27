#include "representation.h"

#ifndef MOVESORT_H
#define MOVESORT_H

struct MoveGenerate{
    Movement moves[128];
    int index;
    int move_nums;
    int state;
    Movement next;
};

void ResetHistory();
void SetBestMove(const Movement& move, int step, int depth);

bool NextMove(const Situation& situation, MoveGenerate& generate, Movement& hash_move, int step);
void MoveSort(const Situation & situation, int & num_of_all_movements, Movement* all_movements, Movement hash_move, int step);
void CaptureMoveSort(const Situation & situation, int & num_of_all_movements, Movement* all_movements);

void InitRootMove(Situation& situation, int& num_of_all_movements, Movement* all_movements);
void SortRootMove(int num_of_all_movements, Movement* all_movements);
void UpdateRootMove(int num_of_all_movements, Movement* all_movements, Movement& move);

#endif