/* 
 * 文件名 : new_value.cpp
 * 描述	: 评估局面价值
 * 作者 : yuanthree
 * 最后更新时间: 21.05.07
 */
#include "value.h"

const int ROOK_VALUE = 6;
const int HORSE_CANNON_VALUE = 3;
const int OTHER_VALUE = 1;
const int TOTAL_MIDGAME_VALUE = ROOK_VALUE * 4 + HORSE_CANNON_VALUE * 8 + OTHER_VALUE * 18;
const int ADVISOR_BISHOP_ATTACKLESS_VALUE = 80;
const int TOTAL_ADVANCED_VALUE = 4;
const int TOTAL_ATTACK_VALUE = 8;
const int TOTAL_ADVISOR_LEAKAGE = 80;

//将棋子与位置数组的序号对应
const int PIECE_NUM_TO_TYPE[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6,
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 6};

// 空头炮的威胁价值，参考方是对红方来说的行号。
const int HOLLOW_THREAT_VALUE[16] = {
    0, 0, 0, 0, 0, 0, 60, 65, 70, 75, 80, 80, 80, 0, 0, 0};

// 炮镇窝心马的威胁价值，指标同上。
const int CENTRAL_THREAT_VALUE[16] = {
    0, 0, 0, 0, 0, 0, 50, 45, 40, 35, 30, 30, 30, 0, 0, 0};

// 沉底炮的威胁价值，下标是列号。
const int BOTTOM_THREAT[16] = {
    0, 0, 0, 40, 30, 0, 0, 0, 0, 0, 30, 40, 0, 0, 0, 0};

// 偷懒评价的边界
const int VAL_BOUND1 = 160;
const int VAL_BOUND2 = 80;
const int VAL_BOUND3 = 40;
const int VAL_BOUND4 = 20;

/*以下为不同局面不同棋子的位置价值*/
// 1. 开中局且处于进攻状态的帅和兵
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

// 2. 开中局且不处于进攻状态的帅和兵
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

// 3. 残局且处于进攻状态的帅和兵
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

// 4. 残局且不处于进攻状态的帅和兵
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

// 5. 没受威胁的士和象
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

// 6. 受到威胁的士和象
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

// 9. 开中局的車
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

// 10. 残局的車
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

int PiecesValue[2][7][256];
int BlackAdvisorLeakage, RedAdvisorLeakage;         //缺士
int HollowThreat[16], CentralThreat[16];            //空头炮、中炮
int RedBottomThreat[16], BlackBottomThreat[16];     //沉底炮
int AdvancedValue;                                  //预估值

/* 
 * 函数名：RedHalf
 * 描述：判断棋子是否位于红方半区
 * 入参：
 * - int i：棋盘上的位置
 * 返回值：
 * - bool 是否位于红方
 * 最后更新时间: 21.05.23
 */
bool RedHalf(int i)
{
    return (i & 128) != 0;
}

/* 
 * 函数名：BlackHalf
 * 描述：判断棋子是否位于黑方半区
 * 入参：
 * - int i：棋盘上的位置
 * 返回值：
 * - bool 是否位于黑方
 * 最后更新时间: 21.05.23
 */
bool BlackHalf(int i)
{
    return (i & 128) == 0;
}

/* 
 * 函数名：SideValue
 * 描述：返回计算分数差的顺序
 * 入参：
 * - int player：行走方
 * - int value：红与黑的分数差
 * 返回值：
 * - int 按照行走方重新计算的分数差
 * 最后更新时间: 21.05.23
 */
int SideValue(int player, int value)
{
    return (player == 0 ? value : -value);
}

/* 
 * 函数名：PreEvaluate
 * 描述：实现预评估，即子力的评估
 * 入参：
 * - const Situation & s：当前局面
 * 返回值：
 * - void
 * 最后更新时间: 21.05.23
 */
void PreEvaluate(Situation &situation)
{
    int side_tag;                             //用于标志行走方
    int black_value = 0, red_value = 0;       //记录价值
    int red_attacks = 0, black_attacks = 0;   //双方威胁值
    int red_attack_value, black_attack_value; //双方轻子价值
    int midgame_value = 0;
    int attacking_pawn_value[256] = {0}, attackless_pawn_value[256] = {0};

    //首先计算当前的局势值，用于判断由开局向残局演变的程度。这个值由棋子的权值和决定，其中车=6、马炮=3、其它=1。
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
    midgame_value = (2 * TOTAL_MIDGAME_VALUE - midgame_value) * midgame_value / TOTAL_MIDGAME_VALUE;
    AdvancedValue = (TOTAL_ADVANCED_VALUE * midgame_value + TOTAL_ADVANCED_VALUE / 2) / TOTAL_MIDGAME_VALUE;
    situation.UseBook = midgame_value > 62;

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

    //计算各种过河棋子的数量来判断各方是否处于进攻状态，其中车马为2，炮兵为1。
    for (int i = 16 + 5; i <= 16 + 8; i++)
        if (situation.current_pieces[i] && BlackHalf(situation.current_pieces[i]))
            red_attacks += 2;
    for (int i = 16 + 9; i <= 16 + 15; i++)
        if (situation.current_pieces[i] && BlackHalf(situation.current_pieces[i]))
            red_attacks++;
    for (int i = 32 + 5; i <= 32 + 8; i++)
        if (situation.current_pieces[i] && RedHalf(situation.current_pieces[i]))
            black_attacks += 2;
    for (int i = 32 + 9; i <= 32 + 15; i++)
        if (situation.current_pieces[i] && RedHalf(situation.current_pieces[i]))
            black_attacks++;
    red_attack_value = red_rook_nums * 2 + red_horse_cannon_nums;
    black_attack_value = black_rook_nums * 2 + black_horsecannon_nums;

    //如果有一方进攻子数比对方多，那么每多一个进攻值再加2，最多不超过8。
    if (red_attack_value > black_attack_value)
        red_attacks += (red_attack_value - black_attack_value) * 2;
    else
        black_attacks += (black_attack_value - red_attack_value) * 2;
    red_attacks = std::min(red_attacks, TOTAL_ATTACK_VALUE);
    black_attacks = std::min(black_attacks, TOTAL_ATTACK_VALUE);

    //计算士的缺值
    BlackAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * red_attacks / TOTAL_ATTACK_VALUE;
    RedAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * black_attacks / TOTAL_ATTACK_VALUE;

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
        RedBottomThreat[i] = BOTTOM_THREAT[i] * black_attacks / TOTAL_ATTACK_VALUE;
        BlackBottomThreat[i] = BOTTOM_THREAT[i] * red_attacks / TOTAL_ATTACK_VALUE;
    }

    // 如果这一方不受威胁，那么把少掉的士和象的价值再加回来
    red_value = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - black_attacks) / TOTAL_ATTACK_VALUE;
    black_value = ADVISOR_BISHOP_ATTACKLESS_VALUE * (TOTAL_ATTACK_VALUE - red_attacks) / TOTAL_ATTACK_VALUE;
    
    // 最后计算两方的总价值
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

const int TOP = 3;
const int BOTTOM = 12;
const int LEFT = 3;
const int CENTER = 7;
const int RIGHT = 11;

const int TYPE_NONE = 0;
const int TYPE_CENTER = 1;
const int TYPE_LEFT = 2;
const int TYPE_RIGHT = 3;

/* 
 * 函数名：SpecialChessType
 * 描述：实现对特殊棋型的评价，包括：
 * 1. 将在原位，双士都在底线，要判断空头炮和炮镇窝心马；
 * 2. 将在原位，双士从左边包围将，要判断右边的沉底炮；
 * 3. 将在原位，双士从右边包围将，要判断左边的沉底炮；
 * 0. 其他情况，包括将不在原位或缺士。
 * 入参：
 * - Situation & s：当前局面
 * 返回值：
 * - int 双方分数差
 * 最后更新时间: 21.05.23
 */
int SpecialChessType(Situation &s)
{
    int num_cannon, num_rook;    // 炮与車的索引下标
    int pos, pos_adv1, pos_adv2; // 棋子位置和双士位置
    int x, y;                    // 行列
    int type;                    // 当前棋型种类序号
    int red_penalty_value = 0, black_penalty_value = 0; // 双方罚分

    //红方
    if (s.current_pieces[16 + 1] && s.current_pieces[16 + 2]) //双士健在
    {
        if (s.current_pieces[16] == 0xc7)
        {
            pos_adv1 = s.current_pieces[16 + 1];
            pos_adv2 = s.current_pieces[16 + 2];
            if (pos_adv1 == 0xc6) // 红方一个仕在左侧底线
                type = ((pos_adv2 == 0xc8) ? TYPE_CENTER : ((pos_adv2 == 0xb7) ? TYPE_LEFT : TYPE_NONE));
            else if (pos_adv1 == 0xc8) // 红方一个仕在右侧底线
                type = ((pos_adv2 == 0xc6) ? TYPE_CENTER : ((pos_adv2 == 0xb7) ? TYPE_RIGHT : TYPE_NONE));
            else if (pos_adv1 == 0xb7) // 红方一个仕在花心
                type = ((pos_adv2 == 0xc6) ? TYPE_LEFT : ((pos_adv2 == 0xc8) ? TYPE_RIGHT : TYPE_NONE));
            else
                type = TYPE_NONE;

            switch (type)
            {
            case TYPE_NONE:
                break;
            case TYPE_CENTER:
                for (num_cannon = 32 + 9; num_cannon <= 32 + 10; num_cannon++)
                {
                    pos = s.current_pieces[num_cannon];
                    if (pos)
                    {
                        x = GetCol(pos);
                        if (x == CENTER)
                        {
                            y = GetRow(pos);
                            if ((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].rook_capture & 1) != 0) // 计算空头炮的威胁
                                red_penalty_value += HollowThreat[15 - y];
                            else if (((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].supercannon_capture & 1) != 0) && (s.current_board[0xb7] == 21 || s.current_board[0xb7] == 22)) // 计算炮镇窝心马的威胁
                                red_penalty_value += CentralThreat[15 - y];
                        }
                    }
                }
                break;
            case TYPE_LEFT:
            case TYPE_RIGHT:
                for (num_cannon = 32 + 9; num_cannon <= 32 + 10; num_cannon++)
                {
                    pos = s.current_pieces[num_cannon];
                    if (pos)
                    {
                        x = GetCol(pos);
                        y = GetRow(pos);
                        if (x == CENTER)
                        {
                            if ((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].supercannon_capture & 1) != 0)
                            {
                                // 计算中炮的威胁，帅的另一侧出口如果在对方控制内还要有额外罚分
                                red_penalty_value += ((CentralThreat[15 - y] >> 2) +
                                                        (IfProtected(1, type == TYPE_LEFT ? 0xc8 : 0xc6, s) ? 20 : 0));
                                // 如果车在底线保护帅，则罚分要增加
                                for (num_rook = 16 + 7; num_rook <= 16 + 8; num_rook++)
                                {
                                    pos = s.current_pieces[num_rook];
                                    if (pos)
                                    {
                                        x = GetCol(pos);
                                        y = GetRow(pos);
                                        if (y == BOTTOM)
                                            if ((ROOK_CANNON_CAN_GET_ROW_MASK[x - 3][s.bit_row[y]].rook_capture & 16) != 0)
                                                red_penalty_value += 80;
                                    }
                                }
                            }
                        }
                        else if (y == BOTTOM) // 计算沉底炮的威胁
                        {
                            if ((ROOK_CANNON_CAN_GET_ROW_MASK[x - 3][s.bit_row[y]].rook_capture & 16) != 0)
                                red_penalty_value += RedBottomThreat[x];
                        }
                    }
                }
                break;
            default:
                break;
            }
        }
        else if (s.current_pieces[16] == 0xb7) // 有双仕但花心被帅占领，要罚分
            red_penalty_value += 20;
    }
    else
    {
        if (s.current_pieces[32 + 7] && s.current_pieces[32 + 8]) // 如果缺仕，也要罚分
            red_penalty_value += RedAdvisorLeakage;
    }

    //黑方
    if (s.current_pieces[32 + 1] && s.current_pieces[32 + 2])
    {
        if (s.current_pieces[32] == 0x37)
        {
            pos_adv1 = s.current_pieces[32 + 1];
            pos_adv2 = s.current_pieces[32 + 2];
            if (pos_adv1 == 0x36)
                type = ((pos_adv2 == 0x38) ? TYPE_CENTER : ((pos_adv2 == 0x47) ? TYPE_LEFT : TYPE_NONE));
            else if (pos_adv1 == 0x38)
                type = ((pos_adv2 == 0x36) ? TYPE_CENTER : ((pos_adv2 == 0x47) ? TYPE_RIGHT : TYPE_NONE));
            else if (pos_adv1 == 0x47)
                type = ((pos_adv2 == 0x36) ? TYPE_LEFT : ((pos_adv2 == 0x38) ? TYPE_RIGHT : TYPE_NONE));
            else
                type = TYPE_NONE;

            switch (type)
            {
            case TYPE_NONE:
                break;
            case TYPE_CENTER:
                for (num_cannon = 16 + 9; num_cannon <= 16 + 10; num_cannon++)
                {
                    pos = s.current_pieces[num_cannon];
                    if (pos)
                    {
                        x = GetCol(pos);
                        if (x == CENTER)
                        {
                            y = GetRow(pos);
                            if ((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].rook_capture & 512) != 0)
                                black_penalty_value += HollowThreat[y];
                            else if (((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].supercannon_capture & 512) != 0) && (s.current_board[0x47] == 37 || s.current_board[0x47] == 38))
                                black_penalty_value += CentralThreat[y];
                        }
                    }
                }
                break;
            case TYPE_LEFT:
            case TYPE_RIGHT:
                for (num_cannon = 16 + 9; num_cannon <= 16 + 10; num_cannon++)
                {
                    pos = s.current_pieces[num_cannon];
                    if (pos)
                    {
                        x = GetCol(pos);
                        y = GetRow(pos);
                        if (x == CENTER)
                        {
                            if ((ROOK_CANNON_CAN_GET_COL_MASK[y - 3][s.bit_col[x]].supercannon_capture & 512) != 0)
                            {
                                black_penalty_value += ((CentralThreat[y] >> 2) +
                                                        (IfProtected(0, type == TYPE_LEFT ? 0x38 : 0x36, s) ? 20 : 0));
                                for (num_rook = 32 + 7; num_rook <= 32 + 8; num_rook++)
                                {
                                    pos = s.current_pieces[num_rook];
                                    if (pos)
                                    {
                                        x = GetCol(pos);
                                        y = GetRow(pos);
                                        if (y == TOP)
                                        {
                                            if ((ROOK_CANNON_CAN_GET_ROW_MASK[x - 3][s.bit_row[y]].rook_capture & 16) != 0)
                                                black_penalty_value += 80;
                                        }
                                    }
                                }
                            }
                        }
                        else if (y == TOP)
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
        else if (s.current_pieces[32] == 0x47)
            black_penalty_value += 20;
    }
    else
    {
        if (s.current_pieces[16 + 7] && s.current_pieces[16 + 8])
            black_penalty_value += BlackAdvisorLeakage;
    }

    return SideValue(s.current_player, black_penalty_value - red_penalty_value);
}

// 用来判断牵制是否有价值，大于0的位置是指对于车来说牵制马和炮(被牵制)都有价值，大于1的位置是指对于炮来说只有牵制马才有价值
const int VALUABLE_CHECK_PIECES[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0};

// 按照中间子和被牵制子的距离，给予不同的价值
const char CHECK_VALUE[512] = {
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

/* 
 * 函数名：ChessCheck
 * 描述：实现对棋子牵制的评价
 * 入参：
 * - const Situation & s：当前局面
 * 返回值：
 * - int 双方分数差
 * 最后更新时间: 21.05.23
 */
int ChessCheck(Situation &s)
{
    int dir;                           // 牵制方向
    int pos_from, pos_to, pos_str;     // 牵制子、牵制目标、被牵制子的位置
    int x, y;                          // 行列
    int side_tag, opp_side_tag;        // 考察方与对方
    int check_value[2];                // 双方的牵制价值

    for (int r = 0; r < 2; r++)
    {
        check_value[r] = 0;
        side_tag = 16 + (r << 4);
        opp_side_tag = 32 - (r << 4);
        //用車牵制的情况
        for (int i = 7; i <= 8; i++)
        {
            pos_from = s.current_pieces[side_tag + i];
            if (pos_from)
            {
                //牵制目标是将的情况
                pos_to = s.current_pieces[opp_side_tag];
                if (pos_to)
                {
                    x = GetCol(pos_from);
                    y = GetRow(pos_from);
                    if (x == GetCol(pos_to)) //纵向牵制
                    {
                        dir = (pos_from < pos_to ? 0 : 1);
                        // 如果車用炮的吃法、炮用超级炮的着法能吃到目标子，牵制成立
                        int capture_row = ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].cannon_capture[dir];
                        if (capture_row == GetRow(pos_to))
                        {
                            pos_str = x + (ROOK_CANNON_CAN_GET_COL[y - 3][s.bit_col[x]].rook_capture[dir] << 4); //计算被牵制的子位置，是车(炮)本身能吃到的棋子
                            if (s.current_board[pos_str] & opp_side_tag)                                         //被牵制子必须是对方的子
                            {
                                if (VALUABLE_CHECK_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to)) // 被牵制子需要有价值且没有被保护
                                    check_value[r] += CHECK_VALUE[pos_to - pos_str + 256];
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
                                if (VALUABLE_CHECK_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                    check_value[r] += CHECK_VALUE[pos_to - pos_str + 256];
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
                                    if (VALUABLE_CHECK_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_to, s) && !IfProtected(1 - r, pos_str, s, pos_to))
                                    {
                                        check_value[r] += CHECK_VALUE[pos_to - pos_str + 256];
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
                                    if (VALUABLE_CHECK_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_to, s) && !IfProtected(1 - r, pos_str, s, pos_to))
                                    {
                                        check_value[r] += CHECK_VALUE[pos_to - pos_str + 256];
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
                                if (VALUABLE_CHECK_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                {
                                    check_value[r] += CHECK_VALUE[pos_to - pos_str + 256];
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
                                if (VALUABLE_CHECK_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                {
                                    check_value[r] += CHECK_VALUE[pos_to - pos_str + 256];
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
                                    if (VALUABLE_CHECK_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                    {
                                        check_value[r] += CHECK_VALUE[pos_to - pos_str + 256];
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
                                    if (VALUABLE_CHECK_PIECES[s.current_board[pos_str]] > 0 && !IfProtected(1 - r, pos_str, s, pos_to))
                                    {
                                        check_value[r] += CHECK_VALUE[pos_to - pos_str + 256];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return SideValue(s.current_player, check_value[RED] - check_value[BLACK]);
}

/* 
 * 函数名：RookFlexibility
 * 描述：实现对車的灵活性的评价
 * 入参：
 * - const Situation & s：当前局面
 * 返回值：
 * - int 双方分数差
 * 最后更新时间: 21.05.23
 */
int RookFlexibility(Situation &s)
{
    int SideTag;
    int rook_flexibility[2] = {0};
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
                rook_flexibility[r] += abs(get_col_1 - get_col_2);

                int get_row_1 = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[0]; //下方向
                int get_row_2 = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[1]; //上方向
                rook_flexibility[r] += abs(get_row_1 - get_row_2);
            }
        }
    }

    return SideValue(s.current_player, rook_flexibility[RED] - rook_flexibility[BLACK]);
}

// 给定不利于马的位置，为棋盘边缘和两个花心位置
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

/* 
 * 函数名：HorseTrap
 * 描述：实现对马的阻碍的评价
 * 入参：
 * - const Situation & s：当前局面
 * 返回值：
 * - int 双方分数差
 * 最后更新时间: 21.05.23
 */
int HorseTrap(Situation &s)
{
    int side_tag;
    int num_move;               //马能走的位置数
    int horse_traps_value[2];   //双方得分
    int *horse_dst, *horse_leg; //马可能到达的位置与对应的马腿位置
    int dst;                    //到达的位置坐标
    for (int r = 0; r < 2; r++)
    {
        side_tag = 16 + (r << 4);
        horse_traps_value[r] = 0;
        // 统计马可以走的位置，不好的位置或者被对方控制的位置都不能走
        for (int i = 5; i <= 6; i++)
        {
            int pos = s.current_pieces[side_tag + i];
            if (pos != 0)
            {
                num_move = 0;
                horse_dst = HORSE_CAN_GET[pos];
                horse_leg = HORSE_LEG[pos];
                dst = *horse_dst;
                while (dst)
                {
                    if (!EDGE_SQUARES[dst] && !s.current_board[dst] && !s.current_board[*horse_leg] && !IfProtected(1 - r, dst, s))
                    {
                        num_move++;
                        if (num_move > 1)
                            break;
                    }
                    horse_dst++;
                    dst = *horse_dst;
                    horse_leg++;
                }
                // 若没有能走的位置，给予10分罚分，若只有一个能走的位置，给予5分罚分
                if (!num_move)
                    horse_traps_value[r] += 10;
                else if (num_move == 1)
                    horse_traps_value[r] += 5;
            }
        }
    }
    return SideValue(s.current_player, horse_traps_value[1] - horse_traps_value[0]);
}

/* 
 * 函数名：Evaluate
 * 描述：实现完整的评价过程
 * 入参：
 * - const Situation & s：当前局面
 * 返回值：
 * - int 双方分数差
 * 最后更新时间: 21.05.23
 */
int Evaluate(Situation &situation, int alpha, int beta)
{
    int value = 0;
    // 评估函数共有以下几个步骤：

    // 1.子力评估，每次只改变变化了的棋子的价值
    value += SideValue(situation.current_player, situation.value[RED] - situation.value[BLACK]) + AdvancedValue;
    if(value + VAL_BOUND1 <= alpha)
        return value + VAL_BOUND1;
    else if(value - VAL_BOUND1 >= beta)
        return value - VAL_BOUND1;

    // 2.特殊棋型
    value += SpecialChessType(situation);
    if(value + VAL_BOUND2 <= alpha)
        return value + VAL_BOUND2;
    else if(value - VAL_BOUND2 >= beta)
        return value - VAL_BOUND2;

    // 3.牵制
    value += ChessCheck(situation);
    if(value + VAL_BOUND3 <= alpha)
        return value + VAL_BOUND3;
    else if(value - VAL_BOUND3 >= beta)
        return value - VAL_BOUND3;

    // 4.車的灵活性
    value += RookFlexibility(situation);
    if(value + VAL_BOUND4 <= alpha)
        return value + VAL_BOUND4;
    else if(value - VAL_BOUND4 >= beta)
        return value - VAL_BOUND4;

    // 5.马的阻碍
    value += HorseTrap(situation);
    return value;
}
