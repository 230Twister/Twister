/* 
 * 文件名： represention.cpp
 * 描述	：实现局面相关操作
 * 作者 : yao
 * 最后更新时间: 21.02.24
 */
#include "representation.h"
#include "preset.h"
#include "hash_table.h"
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

/* 
 * 函数名：CheckOpponent
 * 描述：检测是否将军
 * 入参：
 * - Situation & situation :当前局面
 * 返回值：
 * - bool 判断执棋方是否将军
 * 最后更新时间: 21.03.29
 */
bool CheckOpponent(const Situation & situation){
    // 1. 获得玩家特征值 
    int player_flag = GetPlayerFlag(situation.current_player), player = situation.current_player;
    int opponent_king_id = GetPlayerFlag(OpponentPlayer(situation.current_player)) + 0;
    int piece_from, piece_to, can_move_counter, piece_to_id;

    // 2. 检测马的将军
    for(int i = player_flag + 5; i <= player_flag + 6; i ++){
        if(situation.current_pieces[i] != 0){
            int horse_leg;                  // 马腿位置
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
            while(piece_to != 0){
                horse_leg = HORSE_LEG[piece_from][can_move_counter];
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id == opponent_king_id && situation.current_board[horse_leg] == 0){
                    return true;
                }
                can_move_counter ++;
                piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
            }
        }
    }

    // 3. 车的吃子着法
    for(int i = player_flag + 7; i <= player_flag + 8; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            int col = GetCol(piece_from), row = GetRow(piece_from);

            // 右 左 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_col = ROOK_CANNON_CAN_GET_ROW[col - 3][situation.bit_row[row]].rook_capture[j];
                if(capture_col != col){
                    piece_to = GetPosition(capture_col, row);
                    piece_to_id = situation.current_board[piece_to];
                    if(piece_to_id == opponent_king_id){
                       return true;
                    }
                }
            }
            // 下 上 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][situation.bit_col[col]].rook_capture[j];
                if(capture_row != row){
                    piece_to = GetPosition(col, capture_row);
                    piece_to_id = situation.current_board[piece_to];
                    if(piece_to_id == opponent_king_id){
                       return true;
                    }
                }
            }
        
        }
    }
    
    // 4. 炮的吃子着法
    for(int i = player_flag + 9; i <= player_flag + 10; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            int col = GetCol(piece_from), row = GetRow(piece_from);

            // 右 左 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_col = ROOK_CANNON_CAN_GET_ROW[col - 3][situation.bit_row[row]].connon_capture[j];
                if(capture_col != col){
                    piece_to = GetPosition(capture_col, row);
                    piece_to_id = situation.current_board[piece_to];
                    if(piece_to_id == opponent_king_id){
                       return true;
                    }
                }
            }
            // 下 上 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][situation.bit_col[col]].connon_capture[j];
                if(capture_row != row){
                    piece_to = GetPosition(col, capture_row);
                    piece_to_id = situation.current_board[piece_to];
                    if(piece_to_id == opponent_king_id){
                       return true;
                    }
                }
            }
        
        }
    }

    // 5. 兵(卒)的吃子着法
    for(int i = player_flag + 11; i <= player_flag + 15; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = PAWN_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id == opponent_king_id){
                    return true;
                }
                can_move_counter ++;
                piece_to = PAWN_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }

    // 6. 没有将军
    return false;
}

/* 
 * 函数名：BeChecked
 * 描述：检测是否被对方将军
 * 入参：
 * - Situation & situation :当前局面
 * 返回值：
 * - bool 判断执棋方是否被对方将军
 * 最后更新时间: 21.03.29
 */
bool BeChecked(const Situation & situation){
    // 1. 获得玩家特征值 (注意走棋方假想为对手)
    int player_flag = GetPlayerFlag(OpponentPlayer(situation.current_player)), player = OpponentPlayer(situation.current_player);
    int opponent_king_id = GetPlayerFlag(situation.current_player) + 0;
    int piece_from, piece_to, can_move_counter, piece_to_id;

    // 2. 检测马的将军
    for(int i = player_flag + 5; i <= player_flag + 6; i ++){
        if(situation.current_pieces[i] != 0){
            int horse_leg;                  // 马腿位置
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
            while(piece_to != 0){
                horse_leg = HORSE_LEG[piece_from][can_move_counter];
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id == opponent_king_id && situation.current_board[horse_leg] == 0){
                    return true;
                }
                can_move_counter ++;
                piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
            }
        }
    }

    // 3. 车的吃子着法
    for(int i = player_flag + 7; i <= player_flag + 8; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            int col = GetCol(piece_from), row = GetRow(piece_from);

            // 右 左 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_col = ROOK_CANNON_CAN_GET_ROW[col - 3][situation.bit_row[row]].rook_capture[j];
                if(capture_col != col){
                    piece_to = GetPosition(capture_col, row);
                    piece_to_id = situation.current_board[piece_to];
                    if(piece_to_id == opponent_king_id){
                       return true;
                    }
                }
            }
            // 下 上 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][situation.bit_col[col]].rook_capture[j];
                if(capture_row != row){
                    piece_to = GetPosition(col, capture_row);
                    piece_to_id = situation.current_board[piece_to];
                    if(piece_to_id == opponent_king_id){
                       return true;
                    }
                }
            }
        
        }
    }
    
    // 4. 炮的吃子着法
    for(int i = player_flag + 9; i <= player_flag + 10; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            int col = GetCol(piece_from), row = GetRow(piece_from);

            // 右 左 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_col = ROOK_CANNON_CAN_GET_ROW[col - 3][situation.bit_row[row]].connon_capture[j];
                if(capture_col != col){
                    piece_to = GetPosition(capture_col, row);
                    piece_to_id = situation.current_board[piece_to];
                    if(piece_to_id == opponent_king_id){
                       return true;
                    }
                }
            }
            // 下 上 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][situation.bit_col[col]].connon_capture[j];
                if(capture_row != row){
                    piece_to = GetPosition(col, capture_row);
                    piece_to_id = situation.current_board[piece_to];
                    if(piece_to_id == opponent_king_id){
                       return true;
                    }
                }
            }
        
        }
    }

    // 5. 兵(卒)的吃子着法
    for(int i = player_flag + 11; i <= player_flag + 15; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = PAWN_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id == opponent_king_id){
                    return true;
                }
                can_move_counter ++;
                piece_to = PAWN_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }

    // 6. 没有被将军
    return false;
}

/* 
 * 函数名：MakeAMove
 * 描述：下一步棋
 * 入参：
 * - Situation & situation :当前局面
 * - const Movement move ：着法
 * 返回值：
 * - bool 
 * 最后更新时间: 21.04.05
 */
bool MakeAMove (Situation & situation, const Movement move){
    // 1. 放入走法栈
    situation.moves_stack.push(move);

    // 2. 置换表更新
    // 2.1 当前局面哈希值
    ZobristKey ^= ZobristPlayer;
    ZobristKey ^= ZobristTable[situation.current_board[move.from]][move.from];
    if(move.capture != 0)
        ZobristKey ^= ZobristTable[move.capture][move.to];
    ZobristKey ^= ZobristTable[situation.current_board[move.from]][move.to];

    // 2.2 当前局面哈希校验值
    ZobristKeyCheck ^= ZobristPlayerCheck;
    ZobristKeyCheck ^= ZobristTableCheck[situation.current_board[move.from]][move.from];
    if(move.capture != 0)
        ZobristKey ^= ZobristTableCheck[move.capture][move.to];
    ZobristKey ^= ZobristTableCheck[situation.current_board[move.from]][move.to];

    // 3. 移动棋子
    int piece_id_from = situation.current_board[move.from];
    int piece_id_to = situation.current_board[move.to];
    // 3.1 如果是吃子着法
    if(piece_id_to != 0){
        // 3.1.1 只更新起始格的位行位列
        int row = GetRow(move.from), col = GetCol(move.from);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.from];
        situation.bit_col[col] ^= BIT_COL_MASK[move.from];
        // 3.1.2 更新棋子棋盘数组
        situation.current_board[move.from] = 0;
        situation.current_board[move.to] = piece_id_from;
        situation.current_pieces[piece_id_from] = move.to;
        situation.current_pieces[piece_id_to] = 0;

    }else{
        // 3.1.1 更新起始格的位行位列
        int row = GetRow(move.from), col = GetCol(move.from);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.from];
        situation.bit_col[col] ^= BIT_COL_MASK[move.from];
        // 3.1.2 更新目标格的位行位列
        row = GetRow(move.to), col = GetCol(move.to);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.to];
        situation.bit_col[col] ^= BIT_COL_MASK[move.to];
        // 3.1.3 更新棋子棋盘数组
        situation.current_board[move.from] = 0;
        situation.current_board[move.to] = piece_id_from;
        situation.current_pieces[piece_id_from] = move.to;
    
    }
    // 4. 如果...返回False

    // 5. 交换走棋方
    ChangePlayer(situation.current_player);
}

/* 
 * 函数名：UnMakeAMove
 * 描述：撤销一步棋
 * 入参：
 * - Situation & situation :当前局面
 * 返回值：
 * - void
 * 最后更新时间: 21.04.05
 */
void UnMakeAMove (Situation & situation){
    // 1. 从走法栈中弹出，进行回滚
    Movement move = situation.moves_stack.top();
    situation.moves_stack.pop();

    // 2. 置换表更新
    // 2.1 当前局面哈希值
    ZobristKey ^= ZobristPlayer;
    ZobristKey ^= ZobristTable[situation.current_board[move.to]][move.to];
    if(move.capture != 0)
        ZobristKey ^= ZobristTable[move.capture][move.to];
    ZobristKey ^= ZobristTable[situation.current_board[move.to]][move.from];

    // 2.2 当前局面哈希校验值
    ZobristKeyCheck ^= ZobristPlayerCheck;
    ZobristKeyCheck ^= ZobristTableCheck[situation.current_board[move.to]][move.to];
    if(move.capture != 0)
        ZobristKey ^= ZobristTableCheck[move.capture][move.to];
    ZobristKey ^= ZobristTableCheck[situation.current_board[move.to]][move.from];

    // 3. 返回棋子
    int piece_id_from = situation.current_board[move.to];
    int piece_id_to = move.capture;
    // 3.1 如果是吃子着法
    if(piece_id_to != 0){
        // 3.1.1 只更新起始格的位行位列
        int row = GetRow(move.from), col = GetCol(move.from);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.from];
        situation.bit_col[col] ^= BIT_COL_MASK[move.from];
        // 3.1.2 更新棋子棋盘数组
        situation.current_board[move.from] = piece_id_from;
        situation.current_board[move.to] = piece_id_to;
        situation.current_pieces[piece_id_from] = move.from;
        situation.current_pieces[piece_id_to] = move.to;
    }else{
        // 3.1.1 更新起始格的位行位列
        int row = GetRow(move.from), col = GetCol(move.from);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.from];
        situation.bit_col[col] ^= BIT_COL_MASK[move.from];
        // 3.1.2 更新目标格的位行位列
        row = GetRow(move.to), col = GetCol(move.to);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.to];
        situation.bit_col[col] ^= BIT_COL_MASK[move.to];
        // 3.1.3 更新棋子棋盘数组
        situation.current_board[move.from] = piece_id_from;
        situation.current_board[move.to] = 0;
        situation.current_pieces[piece_id_from] = move.from;
    }

    // 5. 交换走棋方
    ChangePlayer(situation.current_player);
}

/* 
 * 函数名：KingFacing
 * 描述：检测是否将对将局面
 * 入参：
 * - Situation & situation :当前局面
 * 返回值：
 * - bool 是否将对将
 * 最后更新时间: 21.04.05
 */
bool KingFacing(const Situation & situation){
    // 1. 如果有一方没有将 则无效
    int red_king_pos = situation.current_board[GetPlayerFlag(RED) + 0], black_king_pos = situation.current_board[GetPlayerFlag(BLACK) + 0];
    if(situation.current_pieces[GetPlayerFlag(RED) + 0] == 0 || situation.current_pieces[GetPlayerFlag(BLACK) + 0] == 0){
        return false;
    }

    // 2. 先判断两个将是否在同一列
    // 2.1 不在同一列
    if(GetCol(red_king_pos) != GetCol(black_king_pos)){
        return false;
    }
    // 2.2 在同一列，判断中间是否有棋子阻挡(使用车的着法生成，如果将对将，那么中间的车可以吃到的子就是这两个将)
    int resume_rook_row = 7, resume_rook_col = GetCol(red_king_pos);        // 假想的车的位置
    int resume_capture_down = ROOK_CANNON_CAN_GET_COL[resume_rook_row - 3][situation.bit_col[resume_rook_col]].rook_capture[0];
    int resume_capture_up = ROOK_CANNON_CAN_GET_COL[resume_rook_row - 3][situation.bit_col[resume_rook_col]].rook_capture[1];
    int down_pos = GetPosition(resume_rook_col, resume_capture_down), up_pos = GetPosition(resume_rook_col, resume_capture_up); // 假想的车能吃到的地方
    
    // 3. 返回假想的车吃到的是否为上下两个将
    return (down_pos == red_king_pos && up_pos == black_king_pos) || (down_pos == black_king_pos && up_pos == red_king_pos); 
}