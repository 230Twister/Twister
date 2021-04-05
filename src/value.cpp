/* 
 * 文件名： value.cpp
 * 描述	：评估局面价值
 * 作者 : yuanthree
 * 最后更新时间: 21.03.26
 */
#include "value.h"

//将棋子与位置数组的序号对应
const char PieceNumToType[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6};
//不同棋子在棋盘各位置的价值
const UINT8 PositionValues[2][7][256] =
    {
        {{//红帅
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 15, 20, 15, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//红仕
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 30, 0, 30, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 30, 0, 30, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//红相
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 25, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 20, 0, 0, 0, 35, 0, 0, 0, 20, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//红马
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 70, 80, 90, 80, 70, 80, 90, 80, 70, 0, 0, 0, 0,
          0, 0, 0, 80, 110, 125, 90, 70, 90, 125, 110, 80, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 120, 125, 120, 125, 120, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 120, 130, 110, 130, 120, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 110, 110, 120, 100, 120, 110, 110, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 100, 110, 100, 110, 100, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 80, 90, 100, 100, 90, 100, 100, 90, 80, 0, 0, 0, 0,
          0, 0, 0, 80, 80, 90, 90, 80, 90, 90, 80, 80, 0, 0, 0, 0,
          0, 0, 0, 70, 75, 75, 70, 50, 70, 75, 75, 70, 0, 0, 0, 0,
          0, 0, 0, 60, 70, 75, 70, 60, 70, 75, 70, 60, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//红車
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 160, 170, 160, 150, 150, 150, 160, 170, 160, 0, 0, 0, 0,
          0, 0, 0, 170, 180, 170, 190, 250, 190, 170, 180, 170, 0, 0, 0, 0,
          0, 0, 0, 170, 190, 200, 220, 240, 220, 200, 190, 170, 0, 0, 0, 0,
          0, 0, 0, 180, 220, 210, 240, 250, 240, 210, 220, 180, 0, 0, 0, 0,
          0, 0, 0, 180, 220, 210, 240, 250, 240, 210, 220, 180, 0, 0, 0, 0,
          0, 0, 0, 180, 220, 210, 240, 250, 240, 210, 220, 180, 0, 0, 0, 0,
          0, 0, 0, 170, 190, 180, 220, 240, 220, 180, 190, 170, 0, 0, 0, 0,
          0, 0, 0, 170, 180, 170, 170, 160, 170, 170, 180, 170, 0, 0, 0, 0,
          0, 0, 0, 160, 170, 160, 160, 150, 160, 160, 170, 160, 0, 0, 0, 0,
          0, 0, 0, 150, 160, 150, 160, 150, 160, 150, 160, 150, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//红炮
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 125, 130, 100, 70, 60, 70, 100, 130, 125, 0, 0, 0, 0,
          0, 0, 0, 110, 125, 100, 70, 60, 70, 100, 125, 110, 0, 0, 0, 0,
          0, 0, 0, 100, 120, 90, 80, 80, 80, 90, 120, 100, 0, 0, 0, 0,
          0, 0, 0, 90, 110, 90, 110, 130, 110, 90, 110, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 110, 90, 110, 130, 110, 90, 110, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 90, 110, 130, 110, 90, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 90, 90, 110, 90, 90, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 80, 80, 70, 80, 80, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 80, 90, 80, 70, 65, 70, 80, 90, 80, 0, 0, 0, 0,
          0, 0, 0, 80, 90, 80, 70, 60, 70, 80, 90, 80, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//红兵
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 10, 10, 10, 20, 25, 20, 10, 10, 10, 0, 0, 0, 0,
          0, 0, 0, 25, 30, 40, 50, 60, 50, 40, 30, 25, 0, 0, 0, 0,
          0, 0, 0, 25, 30, 30, 40, 40, 40, 30, 30, 25, 0, 0, 0, 0,
          0, 0, 0, 20, 25, 25, 30, 30, 30, 25, 25, 20, 0, 0, 0, 0,
          0, 0, 0, 15, 20, 20, 20, 20, 20, 20, 20, 15, 0, 0, 0, 0,
          0, 0, 0, 10, 0, 15, 0, 15, 0, 15, 0, 10, 0, 0, 0, 0,
          0, 0, 0, 10, 0, 10, 0, 15, 0, 10, 0, 10, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
        {{//黑将
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 15, 20, 15, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//黑士
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 30, 0, 30, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 22, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 30, 0, 30, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//黑象
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 30, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 20, 0, 0, 0, 35, 0, 0, 0, 20, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 25, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//黑马
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 60, 70, 75, 70, 60, 70, 75, 70, 60, 0, 0, 0, 0,
          0, 0, 0, 70, 75, 75, 70, 50, 70, 75, 75, 70, 0, 0, 0, 0,
          0, 0, 0, 80, 80, 90, 90, 80, 90, 90, 80, 80, 0, 0, 0, 0,
          0, 0, 0, 80, 90, 100, 100, 90, 100, 100, 90, 80, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 100, 110, 100, 110, 100, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 110, 110, 120, 100, 120, 110, 110, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 120, 130, 110, 130, 120, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 120, 125, 120, 125, 120, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 80, 110, 125, 90, 70, 90, 125, 110, 80, 0, 0, 0, 0,
          0, 0, 0, 70, 80, 90, 80, 70, 80, 90, 80, 70, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//黑車
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 150, 160, 150, 160, 150, 160, 150, 160, 150, 0, 0, 0, 0,
          0, 0, 0, 160, 170, 160, 160, 150, 160, 160, 170, 160, 0, 0, 0, 0,
          0, 0, 0, 170, 180, 170, 170, 160, 170, 170, 180, 170, 0, 0, 0, 0,
          0, 0, 0, 170, 190, 180, 220, 240, 220, 180, 190, 170, 0, 0, 0, 0,
          0, 0, 0, 180, 220, 210, 240, 250, 240, 210, 220, 180, 0, 0, 0, 0,
          0, 0, 0, 180, 220, 210, 240, 250, 240, 210, 220, 180, 0, 0, 0, 0,
          0, 0, 0, 180, 220, 210, 240, 250, 240, 210, 220, 180, 0, 0, 0, 0,
          0, 0, 0, 170, 190, 200, 220, 240, 220, 200, 190, 170, 0, 0, 0, 0,
          0, 0, 0, 170, 180, 170, 190, 250, 190, 170, 180, 170, 0, 0, 0, 0,
          0, 0, 0, 160, 170, 160, 150, 150, 150, 160, 170, 160, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//黑炮
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 80, 90, 80, 70, 60, 70, 80, 90, 80, 0, 0, 0, 0,
          0, 0, 0, 80, 90, 80, 70, 65, 70, 80, 90, 80, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 80, 80, 70, 80, 80, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 90, 90, 110, 90, 90, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 100, 90, 110, 130, 110, 90, 100, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 110, 90, 110, 130, 110, 90, 110, 90, 0, 0, 0, 0,
          0, 0, 0, 90, 110, 90, 110, 130, 110, 90, 110, 90, 0, 0, 0, 0,
          0, 0, 0, 100, 120, 90, 80, 80, 80, 90, 120, 100, 0, 0, 0, 0,
          0, 0, 0, 110, 125, 100, 70, 60, 70, 100, 125, 110, 0, 0, 0, 0,
          0, 0, 0, 125, 130, 100, 70, 60, 70, 100, 130, 125, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
         {//黑卒
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 10, 0, 10, 0, 15, 0, 10, 0, 10, 0, 0, 0, 0,
          0, 0, 0, 10, 0, 15, 0, 15, 0, 15, 0, 10, 0, 0, 0, 0,
          0, 0, 0, 15, 20, 20, 20, 20, 20, 20, 20, 15, 0, 0, 0, 0,
          0, 0, 0, 20, 25, 25, 30, 30, 30, 25, 25, 20, 0, 0, 0, 0,
          0, 0, 0, 25, 30, 30, 40, 40, 40, 30, 30, 25, 0, 0, 0, 0,
          0, 0, 0, 25, 30, 40, 50, 60, 50, 40, 30, 25, 0, 0, 0, 0,
          0, 0, 0, 10, 10, 10, 20, 25, 20, 10, 10, 10, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};

void Eval(Situation &s)
{
    INT16 i, j, k, r;
    UINT8 p;      //棋子位置
    UINT8 n;      //用于记录下一步的可能位置
    UINT8 m;      //用于记录马腿象眼位置
    int SideTag;  //用于标志行走方
    int OverFlag; //炮的翻山标志
    INT16 bValue, wValue = 0;
    INT16 fValue[2] = {0, 0};

    //计算位置价值
    for (i = 16; i < 32; i++)
    {
        if (s.current_pieces[i] > 0)
            wValue += PositionValues[0][PieceNumToType[i]][s.current_pieces[i]];
    }
    for (i = 32; i < 48; i++)
    {
        if (s.current_pieces[i] > 0)
            bValue += PositionValues[1][PieceNumToType[i]][s.current_pieces[i]];
    }

    for (r = 0; r <= 1; r++)
    {
        SideTag = 16 + r << 4;

        //将的灵活性
        for (i = 0; i <= 0; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int can_move_counter = 0;
                int piece_to = KING_CAN_GET[piece_from][r][can_move_counter];
                while (piece_to != 0)
                {
                    fValue[r] += 2;
                    can_move_counter++;
                    piece_to = KING_CAN_GET[piece_from][r][can_move_counter];
                }
            }
        }

        //士的灵活性
        for (i = 1; i <= 2; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int can_move_counter = 0;
                int piece_to = ADVISOR_CAN_GET[piece_from][r][can_move_counter];
                while (piece_to != 0)
                {
                    fValue[r] += 2;
                    can_move_counter++;
                    piece_to = ADVISOR_CAN_GET[piece_from][r][can_move_counter];
                }
            }
        }

        //象的灵活性
        for (i = 3; i <= 4; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int can_move_counter = 0;
                int piece_to = BISHOP_CAN_GET[piece_from][r][can_move_counter];
                while (piece_to != 0)
                {
                    int bishop_eye = BISHOP_EYE[piece_from][r][can_move_counter];
                    if (s.current_board[bishop_eye] == 0)
                        fValue[r] += 2;
                    can_move_counter++;
                    piece_to = BISHOP_CAN_GET[piece_from][r][can_move_counter];
                }
            }
        }

        //马的灵活性
        for (i = 5; i <= 6; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int can_move_counter = 0;
                int piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
                while (piece_to != 0)
                {
                    int horse_leg = HORSE_LEG[piece_from][can_move_counter];
                    if (s.current_board[horse_leg] == 0)
                        fValue[r] += 5;
                    can_move_counter++;
                    piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
                }
            }
        }

        //車的吃子灵活性
        for (i = 7; i <= 8; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int col = GetCol(piece_from), row = GetRow(piece_from);
                // 右 左 方向的吃子
                for (int j = 0; j < 2; j++)
                {
                    int capture_col = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].rook_capture[j];
                    if (capture_col != col)
                        fValue[r] += 4 * abs(capture_col - col);
                }
                // 下 上 方向的吃子
                for (int j = 0; j < 2; j++)
                {
                    int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].rook_capture[j];
                    if (capture_row != row)
                        fValue[r] += 4 * abs(capture_row - row);
                }
            }
        }

        //炮的吃子灵活性
        for (i = 9; i <= 10; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int col = GetCol(piece_from), row = GetRow(piece_from);
                // 右 左 方向的吃子
                for (int j = 0; j < 2; j++)
                {
                    int capture_col = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].connon_capture[j];
                    if (capture_col != col)
                        fValue[r] += 3 * abs(capture_col - col);
                }
                // 下 上 方向的吃子
                for (int j = 0; j < 2; j++)
                {
                    int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].connon_capture[j];
                    if (capture_row != row)
                        fValue[r] += 3 * abs(capture_row - row);
                }
            }
        }

        //車和炮的不吃子灵活性
        for (i = 7; i <= 10; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int col = GetCol(piece_from), row = GetRow(piece_from);
                // 右方向
                int get_col = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].no_capture[0];
                while (get_col != col)
                {
                    if (i <= 8)
                        fValue[r] += 4;
                    else
                        fValue[r] += 3;
                    get_col--;
                }

                // 左方向
                get_col = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].no_capture[1];
                while (get_col != col)
                {
                    if (i <= 8)
                        fValue[r] += 4;
                    else
                        fValue[r] += 3;
                    get_col++;
                }

                // 下方向
                int get_row = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[0];
                while (get_row != row)
                {
                    if (i <= 8)
                        fValue[r] += 4;
                    else
                        fValue[r] += 3;
                    get_row--;
                }

                // 上方向
                get_row = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[1];
                while (get_row != row)
                {
                    if (i <= 8)
                        fValue[r] += 4;
                    else
                        fValue[r] += 3;
                    get_row++;
                }
            }
        }

        //兵的灵活性
        for (i = 11; i <= 15; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int can_move_counter = 0;
                int piece_to = PAWN_CAN_GET[piece_from][r][can_move_counter];
                while (piece_to != 0)
                {
                    fValue[r] += 2;
                    can_move_counter++;
                    piece_to = PAWN_CAN_GET[piece_from][r][can_move_counter];
                }
            }
        }
    }

    s.value_red = fValue[0] + wValue;
    s.value_black = fValue[1] + bValue;
}