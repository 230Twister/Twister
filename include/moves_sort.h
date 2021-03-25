#include "representation.h"

#ifndef MOVESORT_H
#define MOVESORT_H

extern int HistoryTable[256][256];     // 历史表

void SaveHistoryTable(const Movement& move, int depth);

#endif