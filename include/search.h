/* 
 * 文件名： search.h
 * 描述	：搜索算法
 * 作者 : oscar
 * 最后更新时间: 23.03.23 
 */

#ifndef SEARCH_H
#define SEARCH_H

struct Situation;
struct Movement;
int SearchCut(Situation& situation, int depth, int beta, bool allowNullMove);
int PVSearch(Situation& situation, int depth, int alpha, int beta, Movement& inspire);
int QuiescentSearch(Situation& situation, int alpha, int beta);
int SearchRoot(Situation& situation, int depth, Movement* move_list, int move_num);
void ComputerThink(Situation& situation);

#endif