/* 
 * 文件名： hash_table.h
 * 描述	：置换表
 * 作者 : oscar
 * 最后更新时间: 21.03.25 
 */
#include "define.h"
#include "representation.h"

#ifndef HASHTABLE_H
#define HASHTABLE_H

extern const UINT8 hashEXACT;           // PV结点类型
extern const UINT8 hashALPHA;           // ALPHA结点类型
extern const UINT8 hashBETA;            // BETA结点类型
extern const UINT32 HASHTABLE_SIZE;     // 置换表大小 4MB
extern const UINT32 HASHTABLE_MASK;

extern const int NONE_VALUE;            // 空价值
extern const Movement NONE_MOVE;        // 空着

// 置换表结点
struct HashNode{
    UINT64 check;           // 校验值
    int depth;              // 深度
    int value;              // 价值
    int type;               // 结点类型
    Movement good_move;     // 最佳走法
};
struct RepeatNode{
    UINT64 check;           // 校验值
    int step;               // 走棋数
};
extern HashNode* HashTable;                // 置换表
extern UINT64 ZobristKey;                  // 当前局面键值
extern UINT64 ZobristKeyCheck;             // 当前局面校验值
extern UINT64 ZobristPlayer;               // 走棋方的键值
extern UINT64 ZobristPlayerCheck;          // 走棋方校验值
extern UINT64 ZobristTable[48][256];       // 棋子在棋盘各个位置的键值
extern UINT64 ZobristTableCheck[48][256];  // 棋子在棋盘各个位置的校验值

void InitHashTable();
void ResetHashTable();
void DeleteHashTable();
void ResetZobristKey();
int ReadHashTable(int depth, int alpha, int beta, Movement& move);
void SaveHashTable(int depth, int value, UINT8 node_type, Movement move);

void SignSituation(int step);
void UnsignSituation(int step);
bool CheckRepeat(Situation& situation, int step);

#endif