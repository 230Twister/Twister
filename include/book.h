/* 
 * 文件名： book.h
 * 描述	：哈希表
 * 作者 : saltgrey
 * 最后更新时间: 21.05.03 
 */

#include "define.h"
#include <string>
using namespace std;

#ifndef BOOK_H
#define BOOK_H

// 置换表结点
struct BookHashNode{
    UINT64 check;                               // 校验值
    Movement move;                              // 走法
};

extern BookHashNode* BookHashTable;             // 哈希表
extern UINT64 BookZobristKey;                   // 当前局面键值
extern UINT64 BookZobristKeyCheck;              // 当前局面校验值

void FENToHash(const char* fen, string move);
void LoadBookHashTable();
Movement ReadBookTable(Situation& situation);
void DeleteBook();

#endif