/* 
 * 文件名： search.h
 * 描述	：搜索算法
 * 作者 : oscar
 * 最后更新时间: 23.03.23 
 */

#ifndef SEARCH_H
#define SEARCH_H

struct Situation;
int PVSearch(Situation& situation, int depth, int alpha, int beta, bool allowNullMove);
int QuiescentSearch(Situation& situation, int alpha, int beta);
void ComputerThink(Situation& situation);

#endif