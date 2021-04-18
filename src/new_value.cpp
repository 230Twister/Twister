/* 
 * 文件名 : new_value.cpp
 * 描述	: 评估局面价值
 * 作者 : yuanthree
 * 最后更新时间: 21.04.18
 */
#include "value.h"

//将棋子与位置数组的序号对应
const int PieceNumToType[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6};

/*以下为不同局面不同棋子的位置价值*/
// 1. 开中局、有进攻机会的帅(将)和兵(卒)
const int cvlKingPawnMidgameAttacking[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 9, 9, 9, 11, 13, 11, 9, 9, 9, 0, 0, 0, 0,
    0, 0, 0, 39, 49, 69, 84, 89, 84, 69, 49, 39, 0, 0, 0, 0,
    0, 0, 0, 39, 49, 64, 74, 74, 74, 64, 49, 39, 0, 0, 0, 0,
    0, 0, 0, 39, 46, 54, 59, 61, 59, 54, 46, 39, 0, 0, 0, 0,
    0, 0, 0, 29, 37, 41, 54, 59, 54, 41, 37, 29, 0, 0, 0, 0,
    0, 0, 0, 7, 0, 13, 0, 16, 0, 13, 0, 7, 0, 0, 0, 0,
    0, 0, 0, 7, 0, 7, 0, 15, 0, 7, 0, 7, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 11, 15, 11, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 2. 开中局、没有进攻机会的帅(将)和兵(卒)
const int cvlKingPawnMidgameAttackless[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 9, 9, 9, 11, 13, 11, 9, 9, 9, 0, 0, 0, 0,
    0, 0, 0, 19, 24, 34, 42, 44, 42, 34, 24, 19, 0, 0, 0, 0,
    0, 0, 0, 19, 24, 32, 37, 37, 37, 32, 24, 19, 0, 0, 0, 0,
    0, 0, 0, 19, 23, 27, 29, 30, 29, 27, 23, 19, 0, 0, 0, 0,
    0, 0, 0, 14, 18, 20, 27, 29, 27, 20, 18, 14, 0, 0, 0, 0,
    0, 0, 0, 7, 0, 13, 0, 16, 0, 13, 0, 7, 0, 0, 0, 0,
    0, 0, 0, 7, 0, 7, 0, 15, 0, 7, 0, 7, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 11, 15, 11, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 3. 残局、有进攻机会的帅(将)和兵(卒)
const int cvlKingPawnEndgameAttacking[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 10, 10, 10, 15, 15, 15, 10, 10, 10, 0, 0, 0, 0,
    0, 0, 0, 50, 55, 60, 85, 100, 85, 60, 55, 50, 0, 0, 0, 0,
    0, 0, 0, 65, 70, 70, 75, 75, 75, 70, 70, 65, 0, 0, 0, 0,
    0, 0, 0, 75, 80, 80, 80, 80, 80, 80, 80, 75, 0, 0, 0, 0,
    0, 0, 0, 70, 70, 65, 70, 70, 70, 65, 70, 70, 0, 0, 0, 0,
    0, 0, 0, 45, 0, 40, 45, 45, 45, 40, 0, 45, 0, 0, 0, 0,
    0, 0, 0, 40, 0, 35, 40, 40, 40, 35, 0, 40, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 5, 5, 15, 5, 5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 3, 13, 3, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 11, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 4. 残局、没有进攻机会的帅(将)和兵(卒)
const int cvlKingPawnEndgameAttackless[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 10, 10, 10, 15, 15, 15, 10, 10, 10, 0, 0, 0, 0,
    0, 0, 0, 10, 15, 20, 45, 60, 45, 20, 15, 10, 0, 0, 0, 0,
    0, 0, 0, 25, 30, 30, 35, 35, 35, 30, 30, 25, 0, 0, 0, 0,
    0, 0, 0, 35, 40, 40, 45, 45, 45, 40, 40, 35, 0, 0, 0, 0,
    0, 0, 0, 25, 30, 30, 35, 35, 35, 30, 30, 25, 0, 0, 0, 0,
    0, 0, 0, 25, 0, 25, 25, 25, 25, 25, 0, 25, 0, 0, 0, 0,
    0, 0, 0, 20, 0, 20, 20, 20, 20, 20, 0, 20, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 5, 5, 13, 5, 5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 3, 12, 3, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 11, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 5. 没受威胁的仕(士)和相(象)
const int cvlAdvisorBishopThreatless[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 20, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 18, 0, 0, 20, 23, 20, 0, 0, 18, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 20, 20, 0, 20, 20, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 6. 受到威胁的仕(士)和相(象)
const int cvlAdvisorBishopThreatened[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 40, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 38, 0, 0, 40, 43, 40, 0, 0, 38, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 43, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 40, 40, 0, 40, 40, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 7. 开中局的马
const int cvlHorseMidgame[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 90, 90, 90, 96, 90, 96, 90, 90, 90, 0, 0, 0, 0,
    0, 0, 0, 90, 96, 103, 97, 94, 97, 103, 96, 90, 0, 0, 0, 0,
    0, 0, 0, 92, 98, 99, 103, 99, 103, 99, 98, 92, 0, 0, 0, 0,
    0, 0, 0, 93, 108, 100, 107, 100, 107, 100, 108, 93, 0, 0, 0, 0,
    0, 0, 0, 90, 100, 99, 103, 104, 103, 99, 100, 90, 0, 0, 0, 0,
    0, 0, 0, 90, 98, 101, 102, 103, 102, 101, 98, 90, 0, 0, 0, 0,
    0, 0, 0, 92, 94, 98, 95, 98, 95, 98, 94, 92, 0, 0, 0, 0,
    0, 0, 0, 93, 92, 94, 95, 92, 95, 94, 92, 93, 0, 0, 0, 0,
    0, 0, 0, 85, 90, 92, 93, 78, 93, 92, 90, 85, 0, 0, 0, 0,
    0, 0, 0, 88, 85, 90, 88, 90, 88, 90, 85, 88, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 8. 残局的马
const int cvlHorseEndgame[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 92, 94, 96, 96, 96, 96, 96, 94, 92, 0, 0, 0, 0,
    0, 0, 0, 94, 96, 98, 98, 98, 98, 98, 96, 94, 0, 0, 0, 0,
    0, 0, 0, 96, 98, 100, 100, 100, 100, 100, 98, 96, 0, 0, 0, 0,
    0, 0, 0, 96, 98, 100, 100, 100, 100, 100, 98, 96, 0, 0, 0, 0,
    0, 0, 0, 96, 98, 100, 100, 100, 100, 100, 98, 96, 0, 0, 0, 0,
    0, 0, 0, 94, 96, 98, 98, 98, 98, 98, 96, 94, 0, 0, 0, 0,
    0, 0, 0, 94, 96, 98, 98, 98, 98, 98, 96, 94, 0, 0, 0, 0,
    0, 0, 0, 92, 94, 96, 96, 96, 96, 96, 94, 92, 0, 0, 0, 0,
    0, 0, 0, 90, 92, 94, 92, 92, 92, 94, 92, 90, 0, 0, 0, 0,
    0, 0, 0, 88, 90, 92, 90, 90, 90, 92, 90, 88, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 9. 开中局的车
const int cvlRookMidgame[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 206, 208, 207, 213, 214, 213, 207, 208, 206, 0, 0, 0, 0,
    0, 0, 0, 206, 212, 209, 216, 233, 216, 209, 212, 206, 0, 0, 0, 0,
    0, 0, 0, 206, 208, 207, 214, 216, 214, 207, 208, 206, 0, 0, 0, 0,
    0, 0, 0, 206, 213, 213, 216, 216, 216, 213, 213, 206, 0, 0, 0, 0,
    0, 0, 0, 208, 211, 211, 214, 215, 214, 211, 211, 208, 0, 0, 0, 0,
    0, 0, 0, 208, 212, 212, 214, 215, 214, 212, 212, 208, 0, 0, 0, 0,
    0, 0, 0, 204, 209, 204, 212, 214, 212, 204, 209, 204, 0, 0, 0, 0,
    0, 0, 0, 198, 208, 204, 212, 212, 212, 204, 208, 198, 0, 0, 0, 0,
    0, 0, 0, 200, 208, 206, 212, 200, 212, 206, 208, 200, 0, 0, 0, 0,
    0, 0, 0, 194, 206, 204, 212, 200, 212, 204, 206, 194, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 10. 残局的车
const int cvlRookEndgame[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 182, 182, 182, 184, 186, 184, 182, 182, 182, 0, 0, 0, 0,
    0, 0, 0, 184, 184, 184, 186, 190, 186, 184, 184, 184, 0, 0, 0, 0,
    0, 0, 0, 182, 182, 182, 184, 186, 184, 182, 182, 182, 0, 0, 0, 0,
    0, 0, 0, 180, 180, 180, 182, 184, 182, 180, 180, 180, 0, 0, 0, 0,
    0, 0, 0, 180, 180, 180, 182, 184, 182, 180, 180, 180, 0, 0, 0, 0,
    0, 0, 0, 180, 180, 180, 182, 184, 182, 180, 180, 180, 0, 0, 0, 0,
    0, 0, 0, 180, 180, 180, 182, 184, 182, 180, 180, 180, 0, 0, 0, 0,
    0, 0, 0, 180, 180, 180, 182, 184, 182, 180, 180, 180, 0, 0, 0, 0,
    0, 0, 0, 180, 180, 180, 182, 184, 182, 180, 180, 180, 0, 0, 0, 0,
    0, 0, 0, 180, 180, 180, 182, 184, 182, 180, 180, 180, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 11. 开中局的炮
const int cvlCannonMidgame[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 96, 91, 90, 91, 96, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 98, 98, 96, 92, 89, 92, 96, 98, 98, 0, 0, 0, 0,
    0, 0, 0, 97, 97, 96, 91, 92, 91, 96, 97, 97, 0, 0, 0, 0,
    0, 0, 0, 96, 99, 99, 98, 100, 98, 99, 99, 96, 0, 0, 0, 0,
    0, 0, 0, 96, 96, 96, 96, 100, 96, 96, 96, 96, 0, 0, 0, 0,
    0, 0, 0, 95, 96, 99, 96, 100, 96, 99, 96, 95, 0, 0, 0, 0,
    0, 0, 0, 96, 96, 96, 96, 96, 96, 96, 96, 96, 0, 0, 0, 0,
    0, 0, 0, 97, 96, 100, 99, 101, 99, 100, 96, 97, 0, 0, 0, 0,
    0, 0, 0, 96, 97, 98, 98, 98, 98, 98, 97, 96, 0, 0, 0, 0,
    0, 0, 0, 96, 96, 97, 99, 99, 99, 97, 96, 96, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 12. 残局的炮
const int cvlCannonEndgame[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 102, 104, 102, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 102, 104, 102, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 102, 104, 102, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 102, 104, 102, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 102, 104, 102, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 104, 106, 104, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 100, 100, 100, 104, 106, 104, 100, 100, 100, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// 空头炮的威胁分值，指标是对红方来说的行号(即黑方要用15去减)，大体上空头炮位置越高威胁越大。进入残局时，该值要相应减少。
const int cvlHollowThreat[16] = {
    0, 0, 0, 0, 0, 0, 60, 65, 70, 75, 80, 80, 80, 0, 0, 0};

// 炮镇窝心马的威胁分值，指标同上，大体上高度越低威胁越大，没有窝心马时可取四分之一。进入残局时，取值似乎不应变化。
const int cvlCentralThreat[16] = {
    0, 0, 0, 0, 0, 0, 50, 45, 40, 35, 30, 30, 30, 0, 0, 0};

// 沉底炮的威胁分值，指标是列号，大体上越靠近边线威胁越大。威胁减少时，该值要相应减少。
const int cvlBottomThreat[16] = {
    0, 0, 0, 40, 30, 0, 0, 0, 0, 0, 30, 40, 0, 0, 0, 0};

/* 局面预评价就是初始化局面预评价数据(PreEval和PreEvalEx)的过程。
 * ElephantEye的局面预评价主要分以下两个方面：
 * 1. 判断局势处于开中局还是残局阶段；
 * 2. 判断每一方是否对对方形成威胁。*/

const int ROOK_MIDGAME_VALUE = 6;
const int HORSE_CANNON_MIDGAME_VALUE = 3;
const int OTHER_MIDGAME_VALUE = 1;
const int TOTAL_MIDGAME_VALUE = ROOK_MIDGAME_VALUE * 4 + HORSE_CANNON_MIDGAME_VALUE * 8 + OTHER_MIDGAME_VALUE * 18;
const int TOTAL_ADVANCED_VALUE = 4;
const int TOTAL_ATTACK_VALUE = 8;
const int ADVISOR_BISHOP_ATTACKLESS_VALUE = 80;
const int TOTAL_ADVISOR_LEAKAGE = 80;

// 偷懒评价的边界
const int EVAL_MARGIN1 = 160;
const int EVAL_MARGIN2 = 80;
const int EVAL_MARGIN3 = 40;
const int EVAL_MARGIN4 = 20;

int vlWhitePieces[7][256];
int vlBlackPieces[7][256];
int vlBlackAdvisorLeakage, vlWhiteAdvisorLeakage;     //缺士
int vlHollowThreat[16], vlCentralThreat[16];          //空头炮、中炮
int vlWhiteBottomThreat[16], vlBlackBottomThreat[16]; //沉底炮

bool WhiteHalf(int i)
{
    return (i & 128) != 0;
}

bool BlackHalf(int i)
{
    return (i & 128) == 0;
}

int SideValue(int sd, int vl)
{
    return (sd == 0 ? vl : -vl);
}

void PreEvaluate(Situation &s)
{
    int vlAdvanced;                           //预估值
    int SideTag;                              //用于标志行走方
    int bValue = 0, wValue = 0;               //记录价值
    int nWhiteAttacks = 0, nBlackAttacks = 0; //双方威胁值
    int nWhiteSimpleValue, nBlackSimpleValue; //双方轻子价值
    int nMidgameValue = 0;
    int vlPawnPiecesAttacking[256], vlPawnPiecesAttackless[256];

    //首先判断局势处于开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加。
    //统计三类棋子数量
    int nRook = 0, nHorseCannon = 0, nOthers = 0;
    int nRook_w = 0, nRook_b = 0, nHorseCannon_w = 0, nHorseCannon_b = 0;
    for (int r = 0; r < 2; r++)
    {
        SideTag = 16 + (r << 4);
        for (int i = 0; i < 16; i++)
        {
            if (i == 7 || i == 8) //车
            {
                if (s.current_pieces[SideTag + i])
                {
                    nRook++;
                    if (r == 0)
                        nRook_w++;
                    else
                        nRook_b++;
                }
            }
            else if (i == 5 || i == 6 || i == 9 || i == 10) //马炮
            {
                if (s.current_pieces[SideTag + i])
                {
                    nHorseCannon++;
                    if (r == 0)
                        nHorseCannon_w++;
                    else
                        nHorseCannon_b++;
                }
            }
            else
            {
                if (s.current_pieces[SideTag + i])
                    nOthers++;
            }
        }
    }
    nMidgameValue += nRook * ROOK_MIDGAME_VALUE;
    nMidgameValue += nHorseCannon * HORSE_CANNON_MIDGAME_VALUE;
    nMidgameValue += nOthers * OTHER_MIDGAME_VALUE;
    //使用二次函数，子力很少时才认为接近残局
    nMidgameValue = (2 * TOTAL_MIDGAME_VALUE - nMidgameValue) * nMidgameValue / TOTAL_MIDGAME_VALUE;
    vlAdvanced = (TOTAL_ADVANCED_VALUE * nMidgameValue + TOTAL_ADVANCED_VALUE / 2) / TOTAL_MIDGAME_VALUE;
    //计算将车马炮的价值
    for (int i = 0; i < 256; i++)
    {
        if (ON_BOARD[i])
        {
            vlWhitePieces[0][i] = vlBlackPieces[0][254 - i] = ((cvlKingPawnMidgameAttacking[i] * nMidgameValue + cvlKingPawnEndgameAttacking[i] * (TOTAL_MIDGAME_VALUE - nMidgameValue)) / TOTAL_MIDGAME_VALUE);
            vlWhitePieces[3][i] = vlBlackPieces[3][254 - i] = ((cvlHorseMidgame[i] * nMidgameValue + cvlHorseEndgame[i] * (TOTAL_MIDGAME_VALUE - nMidgameValue)) / TOTAL_MIDGAME_VALUE);
            vlWhitePieces[4][i] = vlBlackPieces[4][254 - i] = ((cvlRookMidgame[i] * nMidgameValue + cvlRookEndgame[i] * (TOTAL_MIDGAME_VALUE - nMidgameValue)) / TOTAL_MIDGAME_VALUE);
            vlWhitePieces[5][i] = vlBlackPieces[5][254 - i] = ((cvlCannonMidgame[i] * nMidgameValue + cvlCannonEndgame[i] * (TOTAL_MIDGAME_VALUE - nMidgameValue)) / TOTAL_MIDGAME_VALUE);
            vlPawnPiecesAttacking[i] = vlWhitePieces[0][i];
            vlPawnPiecesAttackless[i] = ((cvlKingPawnMidgameAttackless[i] * nMidgameValue + cvlKingPawnEndgameAttackless[i] * (TOTAL_MIDGAME_VALUE - nMidgameValue)) / TOTAL_MIDGAME_VALUE);
        }
    }

    //计算空头炮和中炮威胁值
    for (int i = 0; i < 16; i++)
    {
        vlHollowThreat[i] = cvlHollowThreat[i] * (nMidgameValue + TOTAL_MIDGAME_VALUE) / (TOTAL_MIDGAME_VALUE * 2);
        vlCentralThreat[i] = cvlCentralThreat[i];
    }

    //然后判断各方是否处于进攻状态，方法是计算各种过河棋子的数量，按照车马2炮兵1相加。
    for (int i = 16 + 5; i <= 16 + 8; i++)
    {
        if (s.current_pieces[i] && BlackHalf(s.current_pieces[i]))
            nWhiteAttacks += 2;
    }
    for (int i = 16 + 9; i <= 16 + 15; i++)
    {
        if (s.current_pieces[i] && BlackHalf(s.current_pieces[i]))
            nWhiteAttacks++;
    }
    for (int i = 32 + 5; i <= 32 + 8; i++)
    {
        if (s.current_pieces[i] && WhiteHalf(s.current_pieces[i]))
            nBlackAttacks += 2;
    }
    for (int i = 32 + 9; i <= 32 + 15; i++)
    {
        if (s.current_pieces[i] && WhiteHalf(s.current_pieces[i]))
            nBlackAttacks++;
    }

    //如果本方轻子数比对方多，那么每多一个轻子(车算2个轻子)威胁值加2。威胁值最多不超过8。
    nWhiteSimpleValue = nRook_w * 2 + nHorseCannon_w;
    nBlackSimpleValue = nRook_b * 2 + nHorseCannon_b;
    if (nWhiteSimpleValue > nBlackSimpleValue)
    {
        nWhiteAttacks += (nWhiteSimpleValue - nBlackSimpleValue) * 2;
    }
    else
    {
        nBlackAttacks += (nBlackSimpleValue - nWhiteSimpleValue) * 2;
    }
    nWhiteAttacks = std::min(nWhiteAttacks, TOTAL_ATTACK_VALUE);
    nBlackAttacks = std::min(nBlackAttacks, TOTAL_ATTACK_VALUE);

    //计算士的缺值
    vlBlackAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * nWhiteAttacks / TOTAL_ATTACK_VALUE;
    vlWhiteAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * nBlackAttacks / TOTAL_ATTACK_VALUE;

    //计算士象兵价值
    for (int i = 0; i < 256; i++)
    {
        if (ON_BOARD[i])
        {
            vlWhitePieces[1][i] = vlWhitePieces[2][i] = ((cvlAdvisorBishopThreatened[i] * nBlackAttacks +
                                                          cvlAdvisorBishopThreatless[i] * (TOTAL_ATTACK_VALUE - nBlackAttacks)) /
                                                         TOTAL_ATTACK_VALUE);
            vlBlackPieces[1][i] = vlBlackPieces[2][i] = ((cvlAdvisorBishopThreatened[254 - i] * nWhiteAttacks +
                                                          cvlAdvisorBishopThreatless[254 - i] * (TOTAL_ATTACK_VALUE - nWhiteAttacks)) /
                                                         TOTAL_ATTACK_VALUE);
            vlWhitePieces[6][i] = ((vlPawnPiecesAttacking[i] * nWhiteAttacks +
                                    vlPawnPiecesAttackless[i] * (TOTAL_ATTACK_VALUE - nWhiteAttacks)) /
                                   TOTAL_ATTACK_VALUE);
            vlBlackPieces[6][i] = ((vlPawnPiecesAttacking[254 - i] * nBlackAttacks +
                                    vlPawnPiecesAttackless[254 - i] * (TOTAL_ATTACK_VALUE - nBlackAttacks)) /
                                   TOTAL_ATTACK_VALUE);
        }
    }

    //计算沉底炮威胁值
    for (int i = 0; i < 16; i++)
    {
        vlWhiteBottomThreat[i] = cvlBottomThreat[i] * nBlackAttacks / TOTAL_ATTACK_VALUE;
        vlBlackBottomThreat[i] = cvlBottomThreat[i] * nWhiteAttacks / TOTAL_ATTACK_VALUE;
    }

    // 调整不受威胁方少掉的仕(士)相(象)分值
    wValue = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - nBlackAttacks) / TOTAL_ATTACK_VALUE;
    bValue = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - nWhiteAttacks) / TOTAL_ATTACK_VALUE;

    // 最后重新计算子力位置分
    for (int i = 16; i < 32; i++)
    {
        int pos = s.current_pieces[i];
        if (pos)
            wValue += vlWhitePieces[PieceNumToType[i]][pos];
    }
    for (int i = 32; i < 48; i++)
    {
        int pos = s.current_pieces[i];
        if (pos)
            bValue += vlBlackPieces[PieceNumToType[i]][pos];
    }

    s.value += SideValue(s.current_player, wValue - bValue) + vlAdvanced;
}

void RookMobility(Situation &s)
{
    int SideTag;
    int vlRookMobility[2] = {0};
    for (int r = 0; r < 2; r++)
    {
        SideTag = 16 + (r << 4);
        for (int i = 7; i <= 8; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int col = GetCol(piece_from), row = GetRow(piece_from);

                int get_col_1 = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].no_capture[0]; //右方向
                int get_col_2 = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].no_capture[1]; //左方向
                vlRookMobility[r] += abs(get_col_1 - get_col_2);

                int get_row_1 = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[0]; //下方向
                int get_row_2 = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[1]; //上方向
                vlRookMobility[r] += abs(get_row_1 - get_row_2);
            }
        }
    }

    s.value += SideValue(s.current_player, vlRookMobility[0] - vlRookMobility[1]);
}

// 局面评价过程
void Evaluate(Situation &s)
{
    s.value = 0;
    // 偷懒的局面评价函数分以下几个层次：

    // 1. 四级偷懒评价(彻底偷懒评价)，只包括子力平衡；
    PreEvaluate(s);
    // if (s.value + EVAL_MARGIN1 <= vlAlpha)
    // {
    //     s.value += EVAL_MARGIN1;
    // }
    // else if (s.value - EVAL_MARGIN1 >= vlBeta)
    // {
    //     s.value -= EVAL_MARGIN1;
    // }

    // // 2. 三级偷懒评价，包括特殊棋型；
    // AdvisorShape(s);
    // if (s.value + EVAL_MARGIN2 <= vlAlpha)
    // {
    //     s.value += EVAL_MARGIN2;
    // }
    // else if (s.value - EVAL_MARGIN2 >= vlBeta)
    // {
    //     s.value -= EVAL_MARGIN2;
    // }

    // // 3. 二级偷懒评价，包括牵制；
    // StringHold(s);
    // if (s.value + EVAL_MARGIN3 <= vlAlpha)
    // {
    //     s.value += EVAL_MARGIN3;
    // }
    // else if (s.value - EVAL_MARGIN3 >= vlBeta)
    // {
    //     s.value -= EVAL_MARGIN3;
    // }

    // 4. 一级偷懒评价，包括车的灵活性；
    RookMobility(s);
    // if (s.value + EVAL_MARGIN4 <= vlAlpha)
    // {
    //     s.value += EVAL_MARGIN4;
    // }
    // else if (s.value - EVAL_MARGIN4 >= vlBeta)
    // {
    //     s.value -= EVAL_MARGIN4;
    // }

    // 5. 零级偷懒评价(完全评价)，包括马的阻碍。
    //KnightTrap(s);
}
