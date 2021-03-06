/* 
 * 文件名： representation.h
 * 描述	：数据的表示，包括棋盘 棋子 局面 着法等数据的表示
 * 作者 : yao
 * 最后更新时间: 21.02.24
 */
#include "define.h"
#include <vector>
#ifndef REPRESENTATION_H
#define REPRESENTATION_H


// =====================================================================================
// 相关常量定义
const int RED = 0;			// 0 表示红
const int BLACK = 1;		// 1 表示黑

// 位置是否在棋盘上的检测数组
const UINT8 ON_BOARD[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
// 各棋子合理位置检测数组 [0]为红方 [1]为黑方
const UINT8 LEGAL_POSITION[2][256] = {												
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
const UINT8 POSITION_MASK[7] = {2, 4, 16, 1, 1, 1, 8};									// 各子力的特征值 将士相车马炮卒
const int BOARD_FIRST_POSITION = 0x33;													// 位于棋盘上的第一个位置
const int KING_DIRECTION[4] = {+0x10, -0x10, +0x1, -0x1};								// 将可走的四个方向
const int HORSE_DIRECTION[8] = {-0x21, -0x1f, -0x12, +0xe, +0x21, +0x1f, -0xe, +0x12};	// 马可走的八个方向
const int HORSE_LEG_DIRECTION[8] = {-0x10, -0x10, -0x1, -0x1, 0x10, 0x10, 0x1, 0x1};	// 马可走方向对应的马脚方向
const int BISHOP_DIRECTION[4] = {-0x1e, -0x22, +0x1e, +0x22};							// 象可走的四个方向
const int BISHOP_EYE_DIRECTION[4] = {-0xf, -0x11, +0xf, +0x11};							// 象可走方向对应的象眼方向
const int ADVISOR_DIRECTION[4] = {-0x11, -0xf, +0x11, +0xf};							// 士可走的方向
const int PAWN_DIRECTION[2][3] = {{-0x10, +0x1, -0x1}, {+0x10, +0x1, -0x1}};			// 双方的兵可走的方向
const int MVV_VALUES[48] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2,
    5, 1, 1, 1, 1, 3, 3, 4, 4, 3, 3, 2, 2, 2, 2, 2,
};																						// 被吃子的价值
const char FEN_OF_PIECE[2][16] = {
	{'K', 'A', 'A', 'B', 'B', 'N', 'N', 'R', 'R', 'C', 'C', 'P', 'P', 'P', 'P', 'P'}, 
	{'k', 'a', 'a', 'b', 'b', 'n', 'n', 'r', 'r', 'c', 'c', 'p', 'p', 'p', 'p', 'p'}
};																						// 棋子序号对应的FEN


// ======================================================================================
// 局面表示
struct Movement;
struct Situation{
	int current_player;					// 当前执棋的玩家 用 0 表示红方 1 表示黑方
	UINT8 current_board[256];			// 当前的棋盘数组，256个位置 0 表示无棋子 其余表示有棋子
	UINT8 current_pieces[48];			// 当前棋子所在的位置 16-31表示红方棋子 32-47表示黑方棋子
	UINT16 bit_row[16];					// 位行 用于车和炮着法的生成
	UINT16 bit_col[16];					// 位列 用于车和炮着法的生成
	int value[2];
	char current_fen[120];				// 当前局面的FEN格式串
	std::vector<Movement> moves_stack;	// 着法栈
	bool UseBook;
};
// 行列 <-> 位置转换
inline int GetRow(const int position){
	return position >> 4;
}
inline int GetCol(const int position){
	return position & 0xf;
}
inline int GetPosition(const int position_col, const int position_row){
	return position_row << 4 | position_col;
}
// 行位列移位时的调整
inline int RowBitOpration(int col){
	return 8 - col;
}
inline int ColBitOpration(int row){
	return 9 - row;
}
// 棋盘上的下一个位置
inline int NextBoardPosition(const int position){
	return (GetCol(position) == 11) ? ( position == 0xcb ? 0 : position + 8 ) : position + 1;
}
// 玩家相关操作与判断
inline void ChangePlayer(int & current_player){
	current_player = 1 - current_player;
}
inline int OpponentPlayer(int current_player){
	return 1 - current_player;
}
inline int GetPlayerFlag(const int current_player){
	return (current_player << 4) + 16;
}
inline int ColorOfPiece(const int piece_id){
	return (piece_id == 0) ? 0 : ((piece_id & GetPlayerFlag(0)) ? RED : BLACK);
}
inline bool InHomeHalf(int pos, int player){
	return (pos & 0x80) != (player << 7);
}
inline int SquareBack(int pos, int player){
	return pos + 16 - (player << 5);
}
// 禁用空着裁剪的条件
inline bool BanNullMove(Situation& situation){
    return situation.value[situation.current_player] <= 250;
}

// 局面操作
void InitSituation(Situation & situation);
int PieceOfFen(const char fen_char);
void AddPiece(int piece_position, int piece_id, Situation & situation);
void DeletePiece(int piece_position, int piece_id, Situation & situation);
void FenToSituation(Situation & situation, const char* fen, const int num_of_movements, const char *movements);
void SituationToFen(Situation & situation, char* fen);
bool MakeAMove (Situation & situation, Movement & move, bool isbegin = 0);
void UnMakeAMove (Situation & situation);
void MakeNullMove(Situation & situation);
void UnMakeNullMove(Situation & situation);
std::string MovementToStr (const Movement move);
Movement StrToMovement ( const std::string move );

// ==========================================================================================
// 着法相关数据类型与函数
struct Movement{
	UINT8 from;				// 着法的起点
	UINT8 to;				// 着法的终点
	UINT8 capture;			// 着法是否吃子 0代表不吃子 其余代表吃的子的类型
	UINT32 value;			// 着法的价值
	UINT8 catc;				// 抓子
	UINT8 movec;			// 抓子的棋子
};
inline bool isMoveEqual(const Movement& move1, const Movement& move2){
	return move1.from == move2.from && move1.to == move2.to && move1.capture == move2.capture;
}
inline void ClearAllMovements(int & num_of_movements){
	num_of_movements = 0;
}
void GetAllMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements);
void GetAllCaptureMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements);
void GetAllNotCaptureMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements);
bool MovementsLegal(const Movement move, const Situation & situation);
int Catch(const Situation & situation, const Movement & move);
// ============================================================================================
// 局面状态检测相关函数
bool IfProtected(int player, const int to, const Situation & situation, int exp = 0);
int CheckOpponent(const Situation & situation);
bool BeChecked(const Situation & situation);
bool KingFacing(const Situation & situation);
// ============================================================================================

#endif