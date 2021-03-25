/* 
 * 文件名： hash_table.h
 * 描述	：置换表
 * 作者 : oscar
 * 最后更新时间: 25.03.11 
 */
#include "define.h"
#include "representation.h"

#ifndef HASHTABLE_H
#define HASHTABLE_H

const UINT8 hashEXACT = 0;                  // PV结点类型
const UINT8 hashALPHA = 1;                  // ALPHA结点类型
const UINT8 hashBETA = 2;                   // BETA结点类型
const UINT32 HASHTABLE_SIZE = 1 << 22;      // 置换表大小 4MB
const UINT32 HASHTABLE_MASK = 1 << 22 - 1;

const int NONE_VALUE = 1 << 20;             // 空价值
const Movement NONE_MOVE = {0, 0, 0};       // 空着

// 置换表结点
struct HashNode{
    UINT64 check;           // 校验值
    int depth;              // 深度
    int value;              // 价值
    int type;               // 结点类型
    Movement good_move;     // 最佳走法
};
HashNode* HashTable;                // 置换表
UINT64 ZobristKey;                  // 当前局面键值
UINT64 ZobristKeyCheck;             // 当前局面校验值
UINT64 ZobristPlayer;               // 走棋方的键值
UINT64 ZobristPlayerCheck;          // 走棋方校验值
UINT64 ZobristTable[32][256];       // 棋子在棋盘各个位置的键值
UINT64 ZobristTableCheck[32][256];  // 棋子在棋盘各个位置的校验值

void InitHashTable();
int ReadHashTable(int depth, int alpha, int beta, Movement& move);
void SaveHashTable(int depth, int value, UINT8 node_type, Movement move);

#endif