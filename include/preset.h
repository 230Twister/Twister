/* 
 * 文件名： define.h
 * 描述	包含预置数组相关定义
 * 作者 : yao
 * 最后更新时间: 21.02.24
 */
#include "define.h"
#include "representation.h"

#ifndef PRESET_H
#define PRESET_H

extern int HORSE_CAN_GET[256][10];          // 马在各个位置所能到的地方
extern int HORSE_LEG[256][10];              // 马在各个位置马腿所在的地方
extern int KING_CAN_GET[256][2][10];       	// 将(帅)在各个位置所能到的地方
extern int ADVISOR_CAN_GET[256][2][10];   	// 士(仕)在各个位置所能到的地方
extern int PAWN_CAN_GET[256][2][10];        // 兵(卒)在各个位置所能到的地方
extern int BISHOP_CAN_GET[256][2][10];     	// 象在各个位置所能到的地方
extern int BISHOP_EYE[256][2][10];        	// 象在各个位置象眼所在的地方

extern const UINT8 LEGAL_MOVE[512];
extern const UINT8 HORSE_LEGAL_MOVE[512];

// 车炮预置数组中的结构表示
struct RookCannonCanGet{
    UINT8 no_capture[2];        // 不吃子的着法 最左(或最上)[0] 最右(或最下)[1]
    UINT8 rook_capture[2];      // 车的吃子着法 最左(或最上)[0] 最右(或最下)[1]
    UINT8 connon_capture[2];    // 炮的吃子着法 最左(或最上)[0] 最右(或最下)[1]
};

// 车炮预置数组中的结构表示
struct RookCannonMask{
    UINT16 no_capture;
    UINT16 rook_capture;
    UINT16 cannon_capture;
};

extern RookCannonCanGet ROOK_CANNON_CAN_GET_ROW[9][1 << 9];         // 车炮在某行方向上的着法
extern RookCannonCanGet ROOK_CANNON_CAN_GET_COL[10][1 << 10];       // 车炮在某列方向上的着法
extern RookCannonMask ROOK_CANNON_CAN_GET_ROW_MASK[9][1 << 9];      // 车炮在某行方向上的着法屏蔽位用于合理性判断
extern RookCannonMask ROOK_CANNON_CAN_GET_COL_MASK[10][1 << 10];    // 车炮在某列方向上的着法屏蔽位用于合理性判断
extern UINT16 BIT_ROW_MASK[256];                                    // 行屏蔽位
extern UINT16 BIT_COL_MASK[256];                                    // 列屏蔽位

// 生成预置数组
void InitPresetArray();

#endif