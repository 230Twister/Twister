/* 
 * 文件名： represention.cpp
 * 描述	：实现局面相关操作
 * 作者 : yao
 * 最后更新时间: 21.02.24
 */
#include "representation.h"
#include "preset.h"

/* 
 * 函数名：InitSituation
 * 描述：根据FEN串初始化棋盘，包括初始化棋盘棋子数组，执棋玩家，局面对应的FEN串
 * 入参：
 * - Situation & situation : 当前的局面
 * 返回值：
 * - void
 * 最后更新时间: 21.03.24
 */
void InitSituation(Situation & situation){
    // 清空棋子棋盘数组
    memset(situation.current_board, 0, sizeof(UINT8) * 256);
    memset(situation.current_pieces, 0, sizeof(UINT8) * 48);
    // 清空位行位列
    memset(situation.bit_col, 0, sizeof(UINT16) * 16);
    memset(situation.bit_row, 0, sizeof(UINT16) * 16);
    // 初始化局面价值 玩家
    situation.current_player = RED;
    situation.value_black = 0;
    situation.value_red = 0;
}

/* 
 * 函数名：AddPiece
 * 描述：向棋盘中添加棋子
 * 入参：
 * - int piece_position 棋子的位置
 * - int piece_id 棋子的编号
 * - Situation & situation 当前局面
 * 返回值：
 * - void 
 * 最后更新时间: 21.03.24
 */
void AddPiece(int piece_position, int piece_id, Situation & situation){
    // 1. 更新棋子棋盘数组
    situation.current_board[piece_position] = piece_id;
    situation.current_pieces[piece_id] = piece_position;
    
    // 2. 更新位行位列
    int row = GetRow(piece_position), col = GetCol(piece_position);
    situation.bit_row[row] ^= BIT_ROW_MASK[piece_position];
    situation.bit_col[col] ^= BIT_COL_MASK[piece_position];

    // 3. 更新xxx

    // 4. 更新xxx
}

/* 
 * 函数名：DeletePiece
 * 描述：向棋盘中添加棋子
 * 入参：
 * - int piece_position 棋子的位置
 * - int piece_id 棋子的编号
 * - Situation & situation 当前局面
 * 返回值：
 * - void
 * 最后更新时间: 21.03.24
 */
void DeletePiece(int piece_position, int piece_id, Situation & situation){
    // 1. 更新棋子棋盘数组
    situation.current_board[piece_position] = 0;
    situation.current_pieces[piece_id] = 0;
    
    // 2. 更新位行位列
    int row = GetRow(piece_position), col = GetCol(piece_position);
    situation.bit_row[row] ^= BIT_ROW_MASK[piece_position];
    situation.bit_col[col] ^= BIT_COL_MASK[piece_position];

    // 3. 更新xxx

    // 4. 更新xxx
}

/* 
 * 函数名：PieceOfFen
 * 描述：FEN字符转棋子的序号索引
 * 入参：
 * - const char fen_char FEN串中的字符，代表一个棋子
 * 返回值：
 * - int 返回棋子的序号索引(红 0-6 黑 7-13)
 * 最后更新时间: 21.03.25
 */
int PieceOfFen(const char fen_char){
    int piece_index;
    switch (fen_char){
    case 'K':
        piece_index = 0;
        break;
    case 'k':
        piece_index = 7;
        break;
    case 'A':
        piece_index = 1;
        break;
    case 'a':
        piece_index = 8;
        break;
    case 'B':
        piece_index = 2;
        break;
    case 'b':
        piece_index = 9;
        break;
    case 'N':
        piece_index = 3;
        break;
    case 'n':
        piece_index = 10;
        break;
    case 'R':
        piece_index = 4;
        break;
    case 'r':
        piece_index = 11;
        break;
    case 'C':
        piece_index = 5;
        break;
    case 'c':
        piece_index = 12;
        break;
    case 'P':
        piece_index = 6;
        break;
    case 'p':
        piece_index = 13;
        break;
    default:
        break;
    }
    return piece_index;
}

/* 
 * 函数名：FenToSituation
 * 描述：FEN串转棋局
 * 入参：
 * - Situation & situation :当前局面
 * - const char* fen : 传入的FEN串
 * 返回值：
 * - void
 * 最后更新时间: 21.03.25
 */
void FenToSituation(Situation & situation, const char* fen){
    const char *p = fen;
    int piece_id_array[14] = {16, 17, 19, 21, 23, 25, 27, 32, 33, 35, 37, 39, 41, 43};
    int col = GetCol(BOARD_FIRST_POSITION), row = GetRow(BOARD_FIRST_POSITION), piece_id_index, piece_id;
    // 1. 写入局面
    while(*p != ' '){
        if(*p == '/'){
            col = 3;
            row ++;
        }
        else if(*p > '0' && *p <= '9'){
            int no_piece = *p - '0';
            col += no_piece;
        }
        else{
            piece_id_index = PieceOfFen(*p);
            piece_id = piece_id_array[piece_id_index];
            piece_id_array[piece_id_index] ++;
            AddPiece(GetPosition(col, row), piece_id, situation);
            col ++;
        }
        p ++;
    }
    // 2. 写入当前玩家
    p ++;
    if(*p == 'r') situation.current_player = RED;
    else situation.current_player = BLACK;
}

/* 
 * 函数名：SituationToFen
 * 描述：棋局转FEN串
 * 入参：
 * - Situation & situation :当前局面
 * - char* fen : 输出的FEN串
 * 返回值：
 * - void
 * 最后更新时间: 21.03.25
 */
void SituationToFen(Situation & situation, char* fen);