/* 
 * 文件名 : new_value.cpp
 * 描述	: 评估局面价值
 * 作者 : yuanthree
 * 最后更新时间: 21.05.07
 */
#include "value.h"

//将棋子与位置数组的序号对应
const int PIECE_NUM_TO_TYPE[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6};

/*以下为不同局面不同棋子的位置价值*/
// 1. 开中局、有进攻机会的帅(将)和兵(卒)
const int KING_PAWN_MIDGAME_ATTACKING_VALUE[256] = {
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
const int KING_PAWN_MIDGAME_ATTACKLESS_VALUE[256] = {
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
const int KING_PAWN_ENDGAME_ATTACKING_VALUE[256] = {
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
const int KING_PAWN_ENDGAME_ATTACKLESS_VALUE[256] = {
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
const int THREATLESS_ADVISOR_BISHOP_VALUE[256] = {
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
const int THREATENED_ADVISOR_BISHOP_VALUE[256] = {
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
const int HORSE_MIDGAME_VALUE[256] = {
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
const int HORSE_ENDGAME_VALUE[256] = {
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
const int ROOK_MIDGAME_VALUE[256] = {
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
const int ROOK_ENDGAME_VALUE[256] = {
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
const int CANNON_MIDGAME_VALUE[256] = {
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
const int CANNON_ENDGAME_VALUE[256] = {
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
const int HOLLOW_THREAT_VALUE[16] = {
    0, 0, 0, 0, 0, 0, 60, 65, 70, 75, 80, 80, 80, 0, 0, 0};

// 炮镇窝心马的威胁分值，指标同上，大体上高度越低威胁越大，没有窝心马时可取四分之一。进入残局时，取值似乎不应变化。
const int CENTRAL_THREAT_VALUE[16] = {
    0, 0, 0, 0, 0, 0, 50, 45, 40, 35, 30, 30, 30, 0, 0, 0};

// 沉底炮的威胁分值，指标是列号，大体上越靠近边线威胁越大。威胁减少时，该值要相应减少。
const int BOTTOM_THREAT[16] = {
    0, 0, 0, 40, 30, 0, 0, 0, 0, 0, 30, 40, 0, 0, 0, 0};

/* 局面预评价就是初始化局面预评价数据(PreEval和PreEvalEx)的过程。
 * ElephantEye的局面预评价主要分以下两个方面：
 * 1. 判断局势处于开中局还是残局阶段；
 * 2. 判断每一方是否对对方形成威胁。*/

const int ROOK_VALUE = 6;
const int HORSE_CANNON_VALUE = 3;
const int OTHER_VALUE = 1;
const int TOTAL_MIDGAME_VALUE = ROOK_VALUE * 4 + HORSE_CANNON_VALUE * 8 + OTHER_VALUE * 18;
const int TOTAL_AdvancedValue = 4;
const int TOTAL_ATTACK_VALUE = 8;
const int ADVISOR_BISHOP_ATTACKLESS_VALUE = 80;
const int TOTAL_ADVISOR_LEAKAGE = 80;

// 偷懒评价的边界
const int EVAL_MARGIN1 = 160;
const int EVAL_MARGIN2 = 80;
const int EVAL_MARGIN3 = 40;
const int EVAL_MARGIN4 = 20;

int PiecesValue[2][7][256];
int BlackAdvisorLeakage, WhiteAdvisorLeakage;     //缺士
int HollowThreat[16], CentralThreat[16];          //空头炮、中炮
int WhiteBottomThreat[16], BlackBottomThreat[16]; //沉底炮
int AdvancedValue;                                //预估值

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

void PreEvaluate(Situation &situation)
{
    int side_tag;                             //用于标志行走方
    int black_value = 0, red_value = 0;       //记录价值
    int red_attacks = 0, black_attacks = 0;   //双方威胁值
    int red_simple_value, black_simple_value; //双方轻子价值
    int midgame_value = 0;
    int attacking_pawn_value[256], attackless_pawn_value[256];

    //首先判断局势处于开中局还是残局阶段，方法是计算各种棋子的数量，按照车=6、马炮=3、其它=1相加。
    //统计三类棋子数量
    int rook_nums = 0, horse_cannon_nums = 0, others_nums = 0;
    int red_rook_nums = 0, black_rook_nums = 0, red_horse_cannon_nums = 0, black_horsecannon_nums = 0;
    for (int r = 0; r < 2; r++)
    {
        side_tag = 16 + (r << 4);
        for (int i = 0; i < 16; i++)
        {
            if (i == 7 || i == 8) //车
            {
                if (situation.current_pieces[side_tag + i])
                {
                    rook_nums++;
                    if (r == 0)
                        red_rook_nums++;
                    else
                        black_rook_nums++;
                }
            }
            else if (i == 5 || i == 6 || i == 9 || i == 10) //马炮
            {
                if (situation.current_pieces[side_tag + i])
                {
                    horse_cannon_nums++;
                    if (r == 0)
                        red_horse_cannon_nums++;
                    else
                        black_horsecannon_nums++;
                }
            }
            else
            {
                if (situation.current_pieces[side_tag + i])
                    others_nums++;
            }
        }
    }
    midgame_value += rook_nums * ROOK_VALUE;
    midgame_value += horse_cannon_nums * HORSE_CANNON_VALUE;
    midgame_value += others_nums * OTHER_VALUE;
    //使用二次函数，子力很少时才认为接近残局
    midgame_value = (2 * TOTAL_MIDGAME_VALUE - midgame_value) * midgame_value / TOTAL_MIDGAME_VALUE;
    situation.banNullMove = midgame_value < 40;
    AdvancedValue = (TOTAL_AdvancedValue * midgame_value + TOTAL_AdvancedValue / 2) / TOTAL_MIDGAME_VALUE;
    //计算将车马炮的价值
    for (int i = 0; i < 256; i++)
    {
        if (ON_BOARD[i])
        {
            PiecesValue[RED][0][i] = PiecesValue[BLACK][0][254 - i] = ((KING_PAWN_MIDGAME_ATTACKING_VALUE[i] * midgame_value + KING_PAWN_ENDGAME_ATTACKING_VALUE[i] * (TOTAL_MIDGAME_VALUE - midgame_value)) / TOTAL_MIDGAME_VALUE);
            PiecesValue[RED][3][i] = PiecesValue[BLACK][3][254 - i] = ((HORSE_MIDGAME_VALUE[i] * midgame_value + HORSE_ENDGAME_VALUE[i] * (TOTAL_MIDGAME_VALUE - midgame_value)) / TOTAL_MIDGAME_VALUE);
            PiecesValue[RED][4][i] = PiecesValue[BLACK][4][254 - i] = ((ROOK_MIDGAME_VALUE[i] * midgame_value + ROOK_ENDGAME_VALUE[i] * (TOTAL_MIDGAME_VALUE - midgame_value)) / TOTAL_MIDGAME_VALUE);
            PiecesValue[RED][5][i] = PiecesValue[BLACK][5][254 - i] = ((CANNON_MIDGAME_VALUE[i] * midgame_value + CANNON_ENDGAME_VALUE[i] * (TOTAL_MIDGAME_VALUE - midgame_value)) / TOTAL_MIDGAME_VALUE);
            attacking_pawn_value[i] = PiecesValue[RED][0][i];
            attackless_pawn_value[i] = ((KING_PAWN_MIDGAME_ATTACKLESS_VALUE[i] * midgame_value + KING_PAWN_ENDGAME_ATTACKLESS_VALUE[i] * (TOTAL_MIDGAME_VALUE - midgame_value)) / TOTAL_MIDGAME_VALUE);
        }
    }

    //计算空头炮和中炮威胁值
    for (int i = 0; i < 16; i++)
    {
        HollowThreat[i] = HOLLOW_THREAT_VALUE[i] * (midgame_value + TOTAL_MIDGAME_VALUE) / (TOTAL_MIDGAME_VALUE * 2);
        CentralThreat[i] = CENTRAL_THREAT_VALUE[i];
    }

    //然后判断各方是否处于进攻状态，方法是计算各种过河棋子的数量，按照车马2炮兵1相加。
    for (int i = 16 + 5; i <= 16 + 8; i++)
    {
        if (situation.current_pieces[i] && BlackHalf(situation.current_pieces[i]))
            red_attacks += 2;
    }
    for (int i = 16 + 9; i <= 16 + 15; i++)
    {
        if (situation.current_pieces[i] && BlackHalf(situation.current_pieces[i]))
            red_attacks++;
    }
    for (int i = 32 + 5; i <= 32 + 8; i++)
    {
        if (situation.current_pieces[i] && WhiteHalf(situation.current_pieces[i]))
            black_attacks += 2;
    }
    for (int i = 32 + 9; i <= 32 + 15; i++)
    {
        if (situation.current_pieces[i] && WhiteHalf(situation.current_pieces[i]))
            black_attacks++;
    }

    //如果本方轻子数比对方多，那么每多一个轻子(车算2个轻子)威胁值加2。威胁值最多不超过8。
    red_simple_value = red_rook_nums * 2 + red_horse_cannon_nums;
    black_simple_value = black_rook_nums * 2 + black_horsecannon_nums;
    if (red_simple_value > black_simple_value)
    {
        red_attacks += (red_simple_value - black_simple_value) * 2;
    }
    else
    {
        black_attacks += (black_simple_value - red_simple_value) * 2;
    }
    red_attacks = std::min(red_attacks, TOTAL_ATTACK_VALUE);
    black_attacks = std::min(black_attacks, TOTAL_ATTACK_VALUE);

    //计算士的缺值
    BlackAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * red_attacks / TOTAL_ATTACK_VALUE;
    WhiteAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * black_attacks / TOTAL_ATTACK_VALUE;

    //计算士象兵价值
    for (int i = 0; i < 256; i++)
    {
        if (ON_BOARD[i])
        {
            PiecesValue[RED][1][i] = PiecesValue[RED][2][i] = ((THREATENED_ADVISOR_BISHOP_VALUE[i] * black_attacks +
                                                                THREATLESS_ADVISOR_BISHOP_VALUE[i] * (TOTAL_ATTACK_VALUE - black_attacks)) /
                                                               TOTAL_ATTACK_VALUE);
            PiecesValue[BLACK][1][i] = PiecesValue[BLACK][2][i] = ((THREATENED_ADVISOR_BISHOP_VALUE[254 - i] * red_attacks +
                                                                    THREATLESS_ADVISOR_BISHOP_VALUE[254 - i] * (TOTAL_ATTACK_VALUE - red_attacks)) /
                                                                   TOTAL_ATTACK_VALUE);
            PiecesValue[RED][6][i] = ((attacking_pawn_value[i] * red_attacks +
                                       attackless_pawn_value[i] * (TOTAL_ATTACK_VALUE - red_attacks)) /
                                      TOTAL_ATTACK_VALUE);
            PiecesValue[BLACK][6][i] = ((attacking_pawn_value[254 - i] * black_attacks +
                                         attackless_pawn_value[254 - i] * (TOTAL_ATTACK_VALUE - black_attacks)) /
                                        TOTAL_ATTACK_VALUE);
        }
    }

    //计算沉底炮威胁值
    for (int i = 0; i < 16; i++)
    {
        WhiteBottomThreat[i] = BOTTOM_THREAT[i] * black_attacks / TOTAL_ATTACK_VALUE;
        BlackBottomThreat[i] = BOTTOM_THREAT[i] * red_attacks / TOTAL_ATTACK_VALUE;
    }

    // 调整不受威胁方少掉的仕(士)相(象)分值
    red_value = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - black_attacks) / TOTAL_ATTACK_VALUE;
    black_value = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - red_attacks) / TOTAL_ATTACK_VALUE;
    // 最后重新计算子力位置分
    for (int i = 16; i < 32; i++)
    {
        int pos = situation.current_pieces[i];
        if (pos)
            red_value += PiecesValue[RED][PIECE_NUM_TO_TYPE[i]][pos];
    }
    for (int i = 32; i < 48; i++)
    {
        int pos = situation.current_pieces[i];
        if (pos)
            black_value += PiecesValue[BLACK][PIECE_NUM_TO_TYPE[i]][pos];
    }

    situation.value[RED] = red_value;
    situation.value[BLACK] = black_value;
}

/* 以下是第一部分，特殊棋型的评价 */

/* 仕(士)的形状对于局面评价，特别是判断空头炮、沉底炮等棋型有重大作用，为此ElephantEye给出四种形状：
 * 1. 帅(将)在原位，双仕(士)都在底线，编为1号，这种情况要判断空头炮和炮镇窝心马；
 * 2. 帅(将)在原位，双仕(士)从左边包围帅(将)，编为2号，这种情况要判断右边的沉底炮和车封右边的帅(将)门；
 * 3. 帅(将)在原位，双仕(士)从右边包围帅(将)，编为3号，这种情况要判断左边的沉底炮和车封左边的帅(将)门；
 * 4. 其他情况，包括帅(将)不在原位或缺仕(士)，都编号0。
 * 注：以“红下黑上”这个固定的棋盘方位来规定左右。
 */
const int RANK_TOP = 3;
const int RANK_BOTTOM = 12;
const int FILE_LEFT = 3;
const int FILE_CENTER = 7;
const int FILE_RIGHT = 11;

const int SHAPE_NONE = 0;
const int SHAPE_CENTER = 1;
const int SHAPE_LEFT = 2;
const int SHAPE_RIGHT = 3;

int AdvisorShape(Situation &s)
{
    int pc_cannon, pc_rook, pos, adv1, adv2, x, y, shape;
    int white_penalty_value, black_penalty_value;
    white_penalty_value = black_penalty_value = 0;

    //红方
    if (s.current_pieces[16 + 1] && s.current_pieces[16 + 2]) //双士健在
    {
        if (s.current_pieces[16] == 0xc7)
        {
            adv1 = s.current_pieces[16 + 1];
            adv2 = s.current_pieces[16 + 2];
            if (adv1 == 0xc6) // 红方一个仕在左侧底线
                shape = ((adv2 == 0xc8) ? SHAPE_CENTER : ((adv2 == 0xb7) ? SHAPE_LEFT : SHAPE_NONE));
            else if (adv1 == 0xc8) // 红方一个仕在右侧底线
                shape = ((adv2 == 0xc6) ? SHAPE_CENTER : ((adv2 == 0xb7) ? SHAPE_RIGHT : SHAPE_NONE));
            else if (adv1 == 0xb7) // 红方一个仕在花心
                shape = ((adv2 == 0xc6) ? SHAPE_LEFT : ((adv2 == 0xc8) ? SHAPE_RIGHT : SHAPE_NONE));
            else
                shape = SHAPE_NONE;

            switch (shape)
            {
            case SHAPE_NONE:
                break;
            case SHAPE_CENTER:
                for (pc_cannon = 32 + 9; pc_cannon <= 32 + 10; pc_cannon++)
                {
                    pos = s.current_pieces[pc_cannon];
                    if (pos)
                    {
                        x = GetCol(pos);
                        if (x == FILE_CENTER)
                        {
                            y = GetRow(pos);
                            if ((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].rook_capture & 512) != 0) // 计算空头炮的威胁
                                white_penalty_value += HollowThreat[15 - y];
                            else if (((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].supercannon_capture & 512) != 0) && (s.current_board[0xb7] == 21 || s.current_board[0xb7] == 22)) // 计算炮镇窝心马的威胁
                                white_penalty_value += CentralThreat[15 - y];
                        }
                    }
                }
            case SHAPE_LEFT:
            case SHAPE_RIGHT:
                for (pc_cannon = 32 + 9; pc_cannon <= 32 + 10; pc_cannon++)
                {
                    pos = s.current_pieces[pc_cannon];
                    if (pos)
                    {
                        x = GetCol(pos);
                        y = GetRow(pos);
                        if (x == FILE_CENTER)
                        {
                            if ((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].supercannon_capture & 512) != 0)
                            {
                                // 计算一般中炮的威胁，帅(将)门被对方控制的还有额外罚分
                                white_penalty_value += ((CentralThreat[15 - y] >> 2) +
                                                        (IfProtected(1, shape == SHAPE_LEFT ? 0xc8 : 0xc6, s) ? 20 : 0));
                                // 如果车在底线保护帅(将)，则给予更大的罚分
                                for (pc_rook = 16 + 7; pc_rook <= 16 + 8; pc_rook++)
                                {
                                    pos = s.current_pieces[pc_rook];
                                    if (pos)
                                    {
                                        y = GetRow(pos);
                                        if (y == RANK_BOTTOM)
                                        {
                                            if ((ROOK_CANNON_CAN_GET_ROW_MASK[x - 3][s.bit_row[y]].rook_capture & 16) != 0)
                                                white_penalty_value += 80;
                                        }
                                    }
                                }
                            }
                        }
                        else if (y == RANK_BOTTOM) // 计算沉底炮的威胁
                        {
                            if ((ROOK_CANNON_CAN_GET_ROW_MASK[x - 3][s.bit_row[y]].rook_capture & 16) != 0)
                                white_penalty_value += WhiteBottomThreat[x];
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
        else if (s.current_pieces[16] == 0xb7) // 有双仕(士)但花心被帅(将)占领，要罚分
            white_penalty_value += 20;
    }
    else
    {
        if (s.current_pieces[32 + 7] && s.current_pieces[32 + 8]) // 缺仕(士)怕双车，有罚分
            white_penalty_value += WhiteAdvisorLeakage;
    }

    //黑方
    if (s.current_pieces[32 + 1] && s.current_pieces[32 + 2]) //双士健在
    {
        if (s.current_pieces[32] == 0xc7)
        {
            adv1 = s.current_pieces[32 + 1];
            adv2 = s.current_pieces[32 + 2];
            if (adv1 == 0x36) // 红方一个仕在左侧底线
                shape = ((adv2 == 0x38) ? SHAPE_CENTER : ((adv2 == 0x47) ? SHAPE_LEFT : SHAPE_NONE));
            else if (adv1 == 0x38) // 红方一个仕在右侧底线
                shape = ((adv2 == 0x36) ? SHAPE_CENTER : ((adv2 == 0x47) ? SHAPE_RIGHT : SHAPE_NONE));
            else if (adv1 == 0x47) // 红方一个仕在花心
                shape = ((adv2 == 0x36) ? SHAPE_LEFT : ((adv2 == 0x38) ? SHAPE_RIGHT : SHAPE_NONE));
            else
                shape = SHAPE_NONE;

            switch (shape)
            {
            case SHAPE_NONE:
                break;
            case SHAPE_CENTER:
                for (pc_cannon = 16 + 9; pc_cannon <= 16 + 10; pc_cannon++)
                {
                    pos = s.current_pieces[pc_cannon];
                    if (pos)
                    {
                        x = GetCol(pos);
                        if (x == FILE_CENTER)
                        {
                            y = GetRow(pos);
                            if ((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].rook_capture & 1) != 0) // 计算空头炮的威胁
                                black_penalty_value += HollowThreat[y];
                            else if (((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].supercannon_capture & 1) != 0) && (s.current_board[0x47] == 37 || s.current_board[0x47] == 38)) // 计算炮镇窝心马的威胁
                                black_penalty_value += CentralThreat[y];
                        }
                    }
                }
            case SHAPE_LEFT:
            case SHAPE_RIGHT:
                for (pc_cannon = 16 + 9; pc_cannon <= 16 + 10; pc_cannon++)
                {
                    pos = s.current_pieces[pc_cannon];
                    if (pos)
                    {
                        x = GetCol(pos);
                        y = GetRow(pos);
                        if (x == FILE_CENTER)
                        {
                            if ((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].supercannon_capture & 1) != 0)
                            {
                                // 计算一般中炮的威胁，帅(将)门被对方控制的还有额外罚分
                                black_penalty_value += ((CentralThreat[y] >> 2) +
                                                        (IfProtected(0, shape == SHAPE_LEFT ? 0x38 : 0x36, s) ? 20 : 0));
                                // 如果车在底线保护帅(将)，则给予更大的罚分
                                for (pc_rook = 32 + 7; pc_rook <= 32 + 8; pc_rook++)
                                {
                                    pos = s.current_pieces[pc_rook];
                                    if (pos)
                                    {
                                        y = GetRow(pos);
                                        if (y == RANK_TOP)
                                        {
                                            if ((ROOK_CANNON_CAN_GET_ROW_MASK[x - 3][s.bit_row[y]].rook_capture & 16) != 0)
                                                black_penalty_value += 80;
                                        }
                                    }
                                }
                            }
                        }
                        else if (y == RANK_TOP) // 计算沉底炮的威胁
                        {
                            if ((ROOK_CANNON_CAN_GET_ROW_MASK[x - 3][s.bit_row[y]].rook_capture & 16) != 0)
                                black_penalty_value += BlackBottomThreat[x];
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
        else if (s.current_pieces[32] == 0x47) // 有双仕(士)但花心被帅(将)占领，要罚分
            black_penalty_value += 20;
    }
    else
    {
        if (s.current_pieces[32 + 7] && s.current_pieces[32 + 8]) // 缺仕(士)怕双车，有罚分
            black_penalty_value += BlackAdvisorLeakage;
    }

    return SideValue(s.current_player, black_penalty_value - white_penalty_value);
}

/* 以下是第二部分，牵制的评价 */

// 常数表"VALUABLE_STRING_PIECES"用判断牵制是否有价值
// 大于0的项是对于车来说的，牵制马和炮(被牵制)都有价值，大于1的项是对于炮来说只有牵制马才有价值
static const int VALUABLE_STRING_PIECES[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0};

// 中间子和被牵制子的距离越近，牵制的价值就越大
const char STRING_VALUE_TAB[512] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 36, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0,
    12, 16, 20, 24, 28, 32, 36, 0, 36, 32, 28, 24, 20, 16, 12, 0,
    0, 0, 0, 0, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 36, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0};

// 车或炮牵制帅(将)或车的棋型的评价
int StringHold(Situation &s)
{
    int dir, pos_from, pos_to, pos_str;
    int x, y, side_tag, opp_side_tag;
    int string_value[2];

    for (int r = 0; r < 2; r++)
    {
        string_value[r] = 0;
        side_tag = 16 + (r << 4);
        opp_side_tag = 32 - (r << 4);
        //考察用車牵制的情况
        for (int i = 7; i <= 8; i++)
        {
            pos_from = s.current_pieces[side_tag + i];
            if (pos_from)
            {
                //考察牵制目标是帅(将)的情况
                pos_to = s.current_pieces[opp_side_tag];
                if (pos_to)
                {
                    x = GetCol(pos_from);
                    y = GetRow(pos_from);
                    if (x == GetCol(pos_to)) //纵向牵制
                    {
                        dir = (pos_from < pos_to ? 0 : 1);
                        // 如果车用炮的吃法(炮用超级炮的着法)能吃到目标子"sqDst"，牵制就成立了，下同
                        int capture_row = ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].cannon_capture[dir];
                        if (capture_row == GetRow(pos_to))
                        {
                            pos_str = x + (ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].rook_capture[dir] << 4); //计算被牵制的子位置，是车(炮)本身能吃到的棋子
                            if (s.current_board[pos_str] & opp_side_tag)                                         //被牵制子必须是对方的子
                            {
                                if (VALUABLE_STRING_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to)) // 如果被牵制子是有价值的，而且被牵制子没有保护(被目标子保护不算)，那么牵制是有价值的
                                {
                                    string_value[r] += STRING_VALUE_TAB[pos_to - pos_str + 256];
                                }
                            }
                        }
                    }
                    else if (y == GetRow(pos_to)) //横向牵制
                    {
                        dir = (pos_from < pos_to ? 0 : 1);
                        int capture_col = ROOK_CANNON_CAN_GET_ROW[x - 3][s.bit_col[y]].cannon_capture[dir];
                        if (capture_col == GetCol(pos_to))
                        {
                            pos_str = (y << 4) + ROOK_CANNON_CAN_GET_ROW[x - 3][s.bit_col[y]].rook_capture[dir];
                            if (s.current_board[pos_str] & opp_side_tag)
                            {
                                if (VALUABLE_STRING_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                {
                                    string_value[r] += STRING_VALUE_TAB[pos_to - pos_str + 256];
                                }
                            }
                        }
                    }
                }

                // 考查牵制目标是车的情况
                for (int i = 7; i <= 8; i++)
                {
                    pos_to = s.current_pieces[opp_side_tag + i];
                    if (pos_to)
                    {
                        x = GetCol(pos_from);
                        y = GetRow(pos_from);
                        if (x == GetCol(pos_to))
                        {
                            dir = (pos_from < pos_to ? 0 : 1);
                            int capture_row = ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].cannon_capture[dir];
                            if (capture_row == GetRow(pos_to))
                            {
                                pos_str = x + (ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].rook_capture[dir] << 4);
                                if (s.current_board[pos_str] & opp_side_tag)
                                {
                                    if (VALUABLE_STRING_PIECES[s.current_board[pos_str]] > 0 && IfProtected(1 - r, pos_to, s) && !IfProtected(1 - r, pos_str, s, pos_to))
                                    {
                                        string_value[r] += STRING_VALUE_TAB[pos_to - pos_str + 256];
                                    }
                                }
                            }
                        }
                        else if (y == GetRow(pos_to))
                        {
                            dir = (pos_from < pos_to ? 0 : 1);
                            int capture_col = ROOK_CANNON_CAN_GET_ROW[x - 3][s.bit_col[y]].cannon_capture[dir];
                            if (capture_col == GetCol(pos_to))
                            {
                                pos_str = (y << 4) + ROOK_CANNON_CAN_GET_ROW[x - 3][s.bit_col[y]].rook_capture[dir];
                                if (s.current_board[pos_str] & opp_side_tag)
                                {
                                    if (VALUABLE_STRING_PIECES[s.current_board[pos_str]] > 0 && IfProtected(1 - r, pos_to, s) && !IfProtected(1 - r, pos_str, s, pos_to))
                                    {
                                        string_value[r] += STRING_VALUE_TAB[pos_to - pos_str + 256];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // 考查用炮来牵制的情况
        for (int i = 9; i <= 10; i++)
        {
            pos_from = s.current_pieces[side_tag + i];
            if (pos_from)
            {
                //考察牵制目标是帅(将)的情况
                pos_to = s.current_pieces[opp_side_tag];
                if (pos_to)
                {
                    x = GetCol(pos_from);
                    y = GetRow(pos_from);
                    if (x == GetCol(pos_to))
                    {
                        dir = (pos_from < pos_to ? 0 : 1);
                        int capture_row = ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].supercannon_capture[dir];
                        if (capture_row == GetRow(pos_to))
                        {
                            pos_str = x + (ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].cannon_capture[dir] << 4);
                            if (s.current_board[pos_str] & opp_side_tag)
                            {
                                if (VALUABLE_STRING_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                {
                                    string_value[r] += STRING_VALUE_TAB[pos_to - pos_str + 256];
                                }
                            }
                        }
                    }
                    else if (y == GetRow(pos_to))
                    {
                        dir = (pos_from < pos_to ? 0 : 1);
                        int capture_col = ROOK_CANNON_CAN_GET_ROW[x - 3][s.bit_col[y]].supercannon_capture[dir];
                        if (capture_col == GetCol(pos_to))
                        {
                            pos_str = (y << 4) + ROOK_CANNON_CAN_GET_ROW[x - 3][s.bit_col[y]].cannon_capture[dir];
                            if (s.current_board[pos_str] & opp_side_tag)
                            {
                                if (VALUABLE_STRING_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                {
                                    string_value[r] += STRING_VALUE_TAB[pos_to - pos_str + 256];
                                }
                            }
                        }
                    }
                }

                // 考查牵制目标是车的情况
                for (int i = 7; i <= 8; i++)
                {
                    pos_to = s.current_pieces[opp_side_tag + i];
                    if (pos_to)
                    {
                        x = GetCol(pos_from);
                        y = GetRow(pos_from);
                        if (x == GetCol(pos_to))
                        {
                            dir = (pos_from < pos_to ? 0 : 1);
                            int capture_row = ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].supercannon_capture[dir];
                            if (capture_row == GetRow(pos_to))
                            {
                                pos_str = x + (ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].cannon_capture[dir] << 4);
                                if (s.current_board[pos_str] & opp_side_tag)
                                {
                                    if (VALUABLE_STRING_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                    {
                                        string_value[r] += STRING_VALUE_TAB[pos_to - pos_str + 256];
                                    }
                                }
                            }
                        }
                        else if (y == GetRow(pos_to))
                        {
                            dir = (pos_from < pos_to ? 0 : 1);
                            int capture_col = ROOK_CANNON_CAN_GET_ROW[x - 3][s.bit_col[y]].supercannon_capture[dir];
                            if (capture_col == GetCol(pos_to))
                            {
                                pos_str = (y << 4) + ROOK_CANNON_CAN_GET_ROW[x - 3][s.bit_col[y]].cannon_capture[dir];
                                if (s.current_board[pos_str] & opp_side_tag)
                                {
                                    if (VALUABLE_STRING_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                    {
                                        string_value[r] += STRING_VALUE_TAB[pos_to - pos_str + 256];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return SideValue(s.current_player, string_value[0] - string_value[1]);
}

/* 以下是第三部分，车的灵活性的评价 */

int RookMobility(Situation &s)
{
    int SideTag;
    int rook_mobility[2] = {0};
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
                rook_mobility[r] += abs(get_col_1 - get_col_2);

                int get_row_1 = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[0]; //下方向
                int get_row_2 = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[1]; //上方向
                rook_mobility[r] += abs(get_row_1 - get_row_2);
            }
        }
    }

    return SideValue(s.current_player, rook_mobility[0] - rook_mobility[1]);
}

/* 以下是第四部分，马受到阻碍的评价 */

// 常数表"EDGE_SQUARES"给定了不利于马的位置，处于棋盘边缘和两个花心位置的马都是坏马
const bool EDGE_SQUARES[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int HorseTrap(Situation &s)
{
    int side_tag;
    int moveble;
    int horse_traps_value[2];
    int *horse_dst, *horse_leg;
    int dst;
    for (int r = 0; r < 2; r++)
    {
        side_tag = 16 + (r << 4);
        horse_traps_value[r] = 0;
        // 考虑马可以走的位置，走到棋盘边缘上，或者走到对方的控制格，都必须排除
        for (int i = 5; i <= 6; i++)
        {
            int pos = s.current_pieces[side_tag + i];
            if (pos != 0)
            {
                moveble = 0;
                horse_dst = HORSE_CAN_GET[pos];
                horse_leg = HORSE_LEG[pos];
                dst = *horse_dst;
                while (dst)
                {
                    if (!EDGE_SQUARES[dst] && !s.current_pieces[dst] && !s.current_pieces[*horse_leg] && !IfProtected(1 - r, dst, s))
                    {
                        moveble++;
                        if (moveble > 1)
                            break;
                    }
                    horse_dst++;
                    dst = *horse_dst;
                    horse_leg++;
                }
                // 没有好的着法的马给予10分罚分，只有一个好的着法的马给予5分罚分
                if (!moveble)
                    horse_traps_value[r] += 10;
                else if (moveble == 1)
                    horse_traps_value[r] += 5;
            }
        }
    }
    return SideValue(s.current_player, horse_traps_value[1] - horse_traps_value[0]);
}

// 局面评价过程
int Evaluate(Situation &situation)
{
    int value = 0;
    // 偷懒的局面评价函数分以下几个层次：
    value += SideValue(situation.current_player, situation.value[RED] - situation.value[BLACK]) + AdvancedValue;

    // 1. 三级偷懒评价，包括特殊棋型；
    value += AdvisorShape(situation);

    // 2. 二级偷懒评价，包括牵制；
    value += StringHold(situation);

    // 3. 一级偷懒评价，包括车的灵活性；
    value += RookMobility(situation);

    // 4. 零级偷懒评价(完全评价)，包括马的阻碍。
    value += HorseTrap(situation);

    return value;
}
