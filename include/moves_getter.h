/* 
 * 文件名： define.h
 * 描述	着法生成器 生成所有的着法 吃子着法 不吃子着法 好的吃子着法
 * 作者 : yao
 * 最后更新时间: 21.02.23
 */
#include "define.h"
#include "representation.h"

#ifndef MOVES_GETTER_H
#define MOVES_GETTER_H

extern int HORSE_CAN_GET[256][10];          // 马在各个位置所能到的地方
extern int HORSE_LEG[256][10];              // 马在各个位置马腿所在的地方
extern int KING_CAN_GET[256][2][10];       	// 将(帅)在各个位置所能到的地方
extern int ADVISOR_CAN_GET[256][2][10];   	// 士(仕)在各个位置所能到的地方
extern int PAWN_CAN_GET[256][2][10];        // 兵(卒)在各个位置所能到的地方
extern int BISHOP_CAN_GET[256][2][10];     	// 象在各个位置所能到的地方
extern int BISHOP_EYE[256][2][10];        	// 象在各个位置象眼所在的地方

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

/* 
 * 函数名：
 * 描述：生成预置数组
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.03.24
 */
void InitPresetArray();

/* 
 * 函数名：GetAllMovements
 * 描述：生成所有着法
 * 入参：
 * - const Situation & situation：当前局面
 * - int & num_of_all_movements：当前着法数
 * - Movement* all_movements ：	当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.03.24
 */
void GetAllMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements);

/* 
 * 函数名：GetAllMovements
 * 描述：生成所有吃子着法
 * 入参：
 * - const Situation & situation：当前局面
 * - int & num_of_all_movements：当前着法数
 * - Movement* all_movements ：	当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.03.24
 */
void GetAllCaptureMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements);

/* 
 * 函数名：GetAllMovements
 * 描述：生成所有吃子着法
 * 入参：
 * - const Situation & situation：当前局面
 * - int & num_of_all_movements：当前着法数
 * - Movement* all_movements ：	当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.03.24
 */
void GetAllNotCaptureMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements);

#endif