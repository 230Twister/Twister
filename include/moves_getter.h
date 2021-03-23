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

/* 
 * 函数名：
 * 描述：生成预置数组
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.03.23
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
 * 最后更新时间: 21.03.22
 */
void GetAllMovements(const Situation & situation, int & num_of_all_movements, int* all_movements);

/* 
 * 函数名：GetAllMovements
 * 描述：生成所有吃子着法
 * 入参：
 * - const Situation & situation：当前局面
 * - int & num_of_all_movements：当前着法数
 * - Movement* all_movements ：	当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.03.22
 */
void GetAllCaptureMovements(const Situation & situation, int & num_of_all_movements, int* all_movements);

/* 
 * 函数名：GetAllMovements
 * 描述：生成所有吃子着法
 * 入参：
 * - const Situation & situation：当前局面
 * - int & num_of_all_movements：当前着法数
 * - Movement* all_movements ：	当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.03.22
 */
void GetAllNotCaptureMovements(const Situation & situation, int & num_of_all_movements, int* all_movements);

#endif