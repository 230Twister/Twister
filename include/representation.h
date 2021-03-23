/* 
 * 文件名： representation.h
 * 描述	：数据的表示，包括棋盘 棋子 局面 着法等数据的表示
 * 作者 : yao
 * 最后更新时间: 21.02.22 
 */
#include "define.h"
#ifndef REPRESENTATION_H
#define REPRESENTATION_H


const int RED = 0;			// 0 表示红
const int BLACK = 1;		// 1 表示黑

const short LEGAL_POSITION[2][256] = {												// 各棋子合理位置检测数组 [0]为红方 [1]为黑方
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 1,25, 1, 9, 1,25, 1, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 1, 9, 1, 9, 1, 9, 1, 9, 0, 0, 0, 0,
		0, 0, 0,17, 1, 1, 7,19, 7, 1, 1,17, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 3, 7, 3, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1,17, 7, 3, 7,17, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	},
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 1,17, 7, 3, 7,17, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 3, 7, 3, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0,17, 1, 1, 7,19, 7, 1, 1,17, 0, 0, 0, 0,
		0, 0, 0, 9, 1, 9, 1, 9, 1, 9, 1, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 1,25, 1, 9, 1,25, 1, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	}
};
const short POSITION_MASK[7] = {2, 4, 16, 1, 1, 1, 8};	// 各子力的特征值 将士相车马炮卒

const int BOARD_FIRST_POSITION = 0x33;													// 位于棋盘上的第一个位置
const int KING_DIRECTION[4] = {+0x10, -0x10, +0x1, -0x1};								// 将可走的四个方向
const int HORSE_DIRECTION[8] = {-0x21, -0x1f, -0x12, +0xe, +0x21, +0x1f, -0xe, +0x12};	// 马可走的八个方向
const int HORSE_LEG_DIRECTION[8] = {-0x10, -0x10, -0x1, -0x1, 0x10, 0x10, 0x1, 0x1};	// 马可走方向对应的马脚方向
const int BISHOP_DIRECTION[4] = {-0x1e, -0x22, +0x1e, +0x22};							// 象可走的四个方向
const int BISHOP_EYE_DIRECTION[4] = {-0xf, -0x11, +0xf, +0x11};							// 象可走方向对应的象眼方向
const int ADVISOR_DIRECTION[4] = {-0x11, -0xf, +0x11, +0xf};							// 士可走的方向
const int PAWN_DIRECTION[2][3] = {{-0x10, +0x1, -0x1}, {+0x10, +0x1, -0x1}};			// 双方的兵可走的方向

/* 
 * 函数名：GetRow GetCol GetPosition
 * 描述: 位置于其对应行列的相互转换与获取
 * 入参：
 * - 行 列 或者 位置
 * 返回值：
 * - 行 列 或者 位置
 * 最后更新时间: 21.03.23
 */
inline int GetRow(const int position){
	return position >> 4;
}
inline int GetCol(const int position){
	return position & 0xf;
}
inline int GetPosition(const int position_col, const int position_row){
	return position_row << 4 | position_col;
}

/* 
 * 函数名：NextBoardPosition
 * 描述：位于棋盘上的下一个位置
 * 入参：
 * - const int position 当前的位置
 * 返回值：
 * - int x :下一个在棋盘上的位置
 * 最后更新时间: 21.03.22
 */
inline int NextBoardPosition(const int position){
	return (GetCol(position) == 11) ? ( position == 0xcb ? 0 : position + 8 ) : position + 1;
}

// 局面表示
struct Situation{
	int current_player;			// 当前执棋的玩家 用 0 表示红方 1 表示黑方
	UINT8 current_board[256];	// 当前的棋盘数组，256个位置 0 表示无棋子 其余表示有棋子
	UINT8 current_chess[48];	// 当前棋子所在的位置 16-31表示红方棋子 32-47表示黑方棋子

	int value_red;				// 局面评估值（红）
	int value_black;			// 局面评估值（黑）

	char current_fen[120];		// 当前局面的FEN格式串
};

/* 
 * 函数名：ChangePlayer
 * 描述：交换执棋玩家 1->0 0->1
 * 入参：
 * - int & current_player: 当前执棋玩家
 * 返回值：
 * - void
 * 最后更新时间: 21.03.22
 */
inline void ChangePlayer(int & current_player){
	current_player = 1 - current_player;
}

/* 
 * 函数名：InitSituation
 * 描述：根据FEN串初始化棋盘，包括初始化棋盘棋子数组，执棋玩家，局面对应的FEN串
 * 入参：
 * - Situation & situation : 当前的局面
 * - const char* fen : 传入的FEN串
 * 返回值：
 * - void
 * 最后更新时间: 21.03.22
 */
void InitSituation(Situation & situation, const char* fen);

/* 
 * 函数名：FenToBoard
 * 描述：FEN串转棋盘数组
 * 入参：
 * - UINT8 board[]：棋盘数组
 * - const char* fen : 传入的FEN串
 * 返回值：
 * - void
 * 最后更新时间: 21.03.22
 */
void FenToBoard(UINT8 board[], const char* fen);

/* 
 * 函数名：AddPiece
 * 描述：向棋盘中添加棋子
 * 入参：
 * - int piece_position 棋子的位置
 * - int piece_id 棋子的编号
 * 返回值：
 * - void
 * 最后更新时间: 21.03.23
 */
void AddPiece(int piece_position, int piece_id);

/* 
 * 函数名：FenToBoard
 * 描述：FEN串转棋盘数组
 * 入参：
 * - UINT8 board[]：需转的棋盘数组
 * - char* fen : 输出的FEN串
 * 返回值：
 * - void
 * 最后更新时间: 21.03.22
 */
void BoardToFen(UINT8 board[], char* fen);

// // 着法表示
// struct Movement{
// 	UINT8 sourc;				// 着法的起点
// 	UINT8 destination;			// 着法的终点
// };

/* 
 * 函数名：ClearAllMovements
 * 描述：清空所有着法
 * 入参：
 * - int & num_of_movements：当前着法数
 * 返回值：
 * - void
 * 最后更新时间: 21.03.22
 */
inline void ClearAllMovements(int & num_of_movements){
	num_of_movements = 0;
}

#endif