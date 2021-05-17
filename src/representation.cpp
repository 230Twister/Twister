/* 
 * 文件名： represention.cpp
 * 描述	：实现局面相关操作
 * 作者 : yao
 * 最后更新时间: 21.02.24
 */
#include "representation.h"
#include "preset.h"
#include "hash_table.h"
#include "value.h"
extern int step;
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
    situation.value[RED] = 0;
    situation.value[BLACK] = 0;
    // 清空着法栈
    situation.moves_stack.clear();
    situation.moves_stack.reserve(100);
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

    // 3. 更新置换表
    ZobristKey ^= ZobristTable[piece_id][piece_position];
    ZobristKeyCheck ^= ZobristTableCheck[piece_id][piece_position];
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
        piece_index = 0;
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
 * - const int num_of_movements 后续着法数
 * - const char *movements 后续着法字符串
 * 返回值：
 * - void
 * 最后更新时间: 21.03.25
 */
void FenToSituation(Situation & situation, const char* fen, const int num_of_movements, const char *movements){
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
    if(*p == 'w') situation.current_player = RED;
    else situation.current_player = BLACK;

    // 3. 若轮到红方走子，局面哈希值异或额外标记
    if(situation.current_player == RED){
        ZobristKey ^= ZobristPlayer;
        ZobristKeyCheck ^= ZobristPlayerCheck;
    }

    // 4. 将后续着法转入局面
    // std::ofstream f;
    // f.open("debug.log", std::ios::app | std::ios::out);
    // f << "后续着法:\n";
    int j = 0;
    for ( int n = 0; n < num_of_movements; n ++ ) {
    	std::string s = "";
    	for (int k = j; k < j + 4; k ++ ) {
    		s += movements[k];
    	}
    	Movement move = StrToMovement(s);
        // f << "From:" << (int)move.from << " To:" << (int)move.to << std::endl;
        MakeAMove(situation, move);
    	j += 5;
    }
    // f.close();

    // 5. 清空栈中的后续着法
    situation.moves_stack.clear();
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
void SituationToFen(Situation & situation, char* fen){
    int row, col, piece_id, space_count;
    char *fent = fen;
    for(row = 3; row <= 12; row++){
        space_count = 0;
        for(col = 3; col <= 11; col ++){
            piece_id = situation.current_board[GetPosition(col, row)];
            if(piece_id != 0){
                if(space_count != 0){
                    *fent = space_count + '0';
                    space_count = 0;
                    fent ++;
                }
                if(piece_id & 16){
                    *fent = FEN_OF_PIECE[0][piece_id - 16];
                }
                else{
                    *fent = FEN_OF_PIECE[1][piece_id - 32];
                }
                fent ++;
            }
            else{
                space_count ++;
            }
        }
        if(space_count > 0){
            *fent = space_count + '0';
            fent ++;
        }
        *fent = '/';
        fent ++;
    }
    *(fent - 1) = ' ';
    *fent = (situation.current_player == RED ? 'w' : 'b');
    fent ++;
    *fent = '\0';
}

/* 
 * 函数：着法与字符串的互转
 * 描述：着法与字符串的互转
 * 入参：
 * - string 或 movement
 * 返回值：
 * - string 或 movement
 * 最后更新时间: 21.04.09
 */
const int P1[] = { 0, 0, 0, +6, +4, +2, 0, -2, -4, -6, -8, -10, -12 };
std::string IntToStr (const int pos){
	int row = GetRow (pos);
	int col = GetCol(pos);
	std::string s = "";
	s += (char) (col - 3 + 'a');
	s += (char) (row + P1[row] + '0');
	return s;
}
const int P2[] = {+12, +10, +8, +6, +4, +2, 0, -2, -4, -6};
int StrToInt (const std::string move ) {
	int col = (int) (move[0] - 'a' + 3);
	int t = (int) (move[1] - '0');
	int row = t + P2[t];
	return GetPosition(col, row);
}
std::string MovementToStr (const Movement move ) {
	if (move.from == move.to) {
		return "   0";
	}
	std::string s = "";
	s += IntToStr (move.from);
	s += IntToStr (move.to);
	return s;
}
Movement  StrToMovement(const std::string move) {
	std::string s = "";
	s += move[0]; 
    s += move[1];
	int from = StrToInt (s);
	s = ""; 
    s += move[2]; 
    s += move[3];
	int to = StrToInt (s);
    Movement movement;
    movement.from = from;
    movement.to = to;
    movement.capture = 0;
	return movement;
}

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
                int capture_col = ROOK_CANNON_CAN_GET_ROW[col - 3][situation.bit_row[row]].cannon_capture[j];
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
                int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][situation.bit_col[col]].cannon_capture[j];
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
    int from, to, from_row, from_col, to_row, to_col;

    // 1. 判断自己将(帅)是否在棋盘上，并记录其位置
    to = situation.current_pieces[GetPlayerFlag(situation.current_player) + 0];
    if(to == 0) return false;
    to_row = GetRow(to);
    to_col = GetCol(to);

    // 2. 检测马的将军
    for(int i = player_flag + 5; i <= player_flag + 6; i ++){
        from = situation.current_pieces[i];
        if(from != 0){
            int horse_leg = HORSE_LEGAL_MOVE[to - from + 256] + from;
            if(horse_leg != from && situation.current_board[horse_leg] == 0) return true;
        }
    }

    // 3. 检测车的将军
    for(int i = player_flag + 7; i <= player_flag + 8; i ++){
        from = situation.current_pieces[i];
        if(from != 0){
            from_row = GetRow(from);
            from_col = GetCol(from);
            if(from_col == to_col){
                if((ROOK_CANNON_CAN_GET_COL_MASK[from_row - 3][situation.bit_col[from_col]].rook_capture & BIT_COL_MASK[to]) != 0) 
                    return true;
            }
            else if(from_row == to_row){
                if((ROOK_CANNON_CAN_GET_ROW_MASK[from_col - 3][situation.bit_row[from_row]].rook_capture & BIT_ROW_MASK[to]) != 0) 
                    return true;
            }
        }
    }
    
    // 4. 检测炮的将军
    for(int i = player_flag + 9; i <= player_flag + 10; i ++){
        from = situation.current_pieces[i];
        if(from != 0){
            from_row = GetRow(from);
            from_col = GetCol(from);
            if(from_col == to_col){
                if((ROOK_CANNON_CAN_GET_COL_MASK[from_row - 3][situation.bit_col[from_col]].cannon_capture & BIT_COL_MASK[to]) != 0) 
                    return true;
            }
            else if(from_row == to_row){
                if((ROOK_CANNON_CAN_GET_ROW_MASK[from_col - 3][situation.bit_row[from_row]].cannon_capture & BIT_ROW_MASK[to]) != 0) 
                    return true;
            }
        }
    }

    // 5. 检测兵(卒)的将军(纵向)
    from = SquareBack(to, player);
    int from_id = situation.current_board[from];
    if(((from_id & player_flag) != 0) && ((from_id & 15) >= 11))
        return true;

    // 6. 检测兵(卒)的将军(横向)
    for(from = to - 1; from <= to + 1; from += 2){
        int from_id = situation.current_board[from];
        if(((from_id & player_flag) != 0) && ((from_id & 15) >= 11))
            return true;
    }

    // 7. 没有被将军
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
bool MakeAMove(Situation & situation, const Movement move){

    int piece_id_from = situation.current_board[move.from];
    int piece_id_to = situation.current_board[move.to];

    // 1. 放入走法栈
    situation.moves_stack.push_back(move);
    SignSituation(step);
    step++;

    // 2. 置换表更新
    // 2.1 当前局面哈希值
    ZobristKey ^= ZobristPlayer;
    ZobristKey ^= ZobristTable[piece_id_from][move.from];
    if(piece_id_to != 0)
        ZobristKey ^= ZobristTable[piece_id_to][move.to];
    ZobristKey ^= ZobristTable[piece_id_from][move.to];

    // 2.2 当前局面哈希校验值
    ZobristKeyCheck ^= ZobristPlayerCheck;
    ZobristKeyCheck ^= ZobristTableCheck[piece_id_from][move.from];
    if(piece_id_to != 0)
        ZobristKeyCheck ^= ZobristTableCheck[piece_id_to][move.to];
    ZobristKeyCheck ^= ZobristTableCheck[situation.current_board[move.from]][move.to];

    // 3. 更新子力价值
    situation.value[situation.current_player] -= PiecesValue[situation.current_player][PIECE_NUM_TO_TYPE[piece_id_from]][move.from];
    situation.value[situation.current_player] += PiecesValue[situation.current_player][PIECE_NUM_TO_TYPE[piece_id_from]][move.to];

    // 4. 移动棋子
    // 4.1 如果是吃子着法
    if(piece_id_to != 0){
        // 4.1.1 更新子力价值
        situation.value[ColorOfPiece(piece_id_to)] -= PiecesValue[ColorOfPiece(piece_id_to)][PIECE_NUM_TO_TYPE[piece_id_to]][move.to];
        // 4.1.2 只更新起始格的位行位列
        int row = GetRow(move.from), col = GetCol(move.from);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.from];
        situation.bit_col[col] ^= BIT_COL_MASK[move.from];
        // 4.1.3 更新棋子棋盘数组
        situation.current_board[move.from] = 0;
        situation.current_board[move.to] = piece_id_from;
        situation.current_pieces[piece_id_from] = move.to;
        situation.current_pieces[piece_id_to] = 0;
    }else{
        // 4.1.1 更新起始格的位行位列
        int row = GetRow(move.from), col = GetCol(move.from);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.from];
        situation.bit_col[col] ^= BIT_COL_MASK[move.from];
        // 4.1.2 更新目标格的位行位列
        row = GetRow(move.to), col = GetCol(move.to);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.to];
        situation.bit_col[col] ^= BIT_COL_MASK[move.to];
        // 4.1.3 更新棋子棋盘数组
        situation.current_board[move.from] = 0;
        situation.current_board[move.to] = piece_id_from;
        situation.current_pieces[piece_id_from] = move.to;
    
    }

    // 5. 交换走棋方
    if(BeChecked(situation)){
        ChangePlayer(situation.current_player);
        return true;
    }
    ChangePlayer(situation.current_player);
    return KingFacing(situation);
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
    Movement move = situation.moves_stack.back();
    situation.moves_stack.pop_back();
    step--;

    int piece_id_from = situation.current_board[move.to];
    int piece_id_to = move.capture;

    // 2. 置换表更新
    // 2.1 当前局面哈希值
    ZobristKey ^= ZobristPlayer;
    ZobristKey ^= ZobristTable[piece_id_from][move.to];
    if(piece_id_to != 0)
        ZobristKey ^= ZobristTable[piece_id_to][move.to];
    ZobristKey ^= ZobristTable[piece_id_from][move.from];

    // 2.2 当前局面哈希校验值
    ZobristKeyCheck ^= ZobristPlayerCheck;
    ZobristKeyCheck ^= ZobristTableCheck[piece_id_from][move.to];
    if(piece_id_to != 0)
        ZobristKeyCheck ^= ZobristTableCheck[piece_id_to][move.to];
    ZobristKeyCheck ^= ZobristTableCheck[situation.current_board[move.to]][move.from];

    UnsignSituation(step);

    // 3. 更新子力价值
    situation.value[ColorOfPiece(piece_id_from)] -= PiecesValue[ColorOfPiece(piece_id_from)][PIECE_NUM_TO_TYPE[piece_id_from]][move.to];
    situation.value[ColorOfPiece(piece_id_from)] += PiecesValue[ColorOfPiece(piece_id_from)][PIECE_NUM_TO_TYPE[piece_id_from]][move.from];

    // 4. 返回棋子
    // 4.1 如果是吃子着法
    if(piece_id_to != 0){
        // 4.1.1 更新子力价值
        situation.value[ColorOfPiece(piece_id_to)] += PiecesValue[ColorOfPiece(piece_id_to)][PIECE_NUM_TO_TYPE[piece_id_to]][move.to];
        // 4.1.2 只更新起始格的位行位列
        int row = GetRow(move.from), col = GetCol(move.from);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.from];
        situation.bit_col[col] ^= BIT_COL_MASK[move.from];
        // 4.1.3 更新棋子棋盘数组
        situation.current_board[move.from] = piece_id_from;
        situation.current_board[move.to] = piece_id_to;
        situation.current_pieces[piece_id_from] = move.from;
        situation.current_pieces[piece_id_to] = move.to;
    }else{
        // 4.1.1 更新起始格的位行位列
        int row = GetRow(move.from), col = GetCol(move.from);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.from];
        situation.bit_col[col] ^= BIT_COL_MASK[move.from];
        // 4.1.2 更新目标格的位行位列
        row = GetRow(move.to), col = GetCol(move.to);
        situation.bit_row[row] ^= BIT_ROW_MASK[move.to];
        situation.bit_col[col] ^= BIT_COL_MASK[move.to];
        // 4.1.3 更新棋子棋盘数组
        situation.current_board[move.from] = piece_id_from;
        situation.current_board[move.to] = 0;
        situation.current_pieces[piece_id_from] = move.from;
    }

    // 5. 交换走棋方
    ChangePlayer(situation.current_player);
}

/* 
 * 函数名：MakeNullMove
 * 描述：执行空着
 * 入参：
 * - Situation & situation :当前局面
 * 返回值：
 * - void 
 * 最后更新时间: 21.04.28
 */
void MakeNullMove(Situation & situation){
    // 1. 放入走法栈
    situation.moves_stack.push_back(Movement{0, 0, 0, 0});
    step++;

    // 2. 更新局面哈希
    ZobristKey ^= ZobristPlayer;
    ZobristKeyCheck ^= ZobristPlayerCheck;

    // 3. 交换走棋方
    ChangePlayer(situation.current_player);
}

/* 
 * 函数名：UnMakeNullMove
 * 描述：撤销空着
 * 入参：
 * - Situation & situation :当前局面
 * 返回值：
 * - void
 * 最后更新时间: 21.04.28
 */
void UnMakeNullMove(Situation & situation){
    // 1. 从走法栈中弹出，进行回滚
    situation.moves_stack.pop_back();
    step--;

    // 2. 更新局面哈希
    ZobristKey ^= ZobristPlayer;
    ZobristKeyCheck ^= ZobristPlayerCheck;

    // 3. 交换走棋方
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
    int red_king_pos = situation.current_pieces[GetPlayerFlag(RED) + 0], black_king_pos = situation.current_pieces[GetPlayerFlag(BLACK) + 0];
    if(red_king_pos == 0 || black_king_pos == 0){
        return false;
    }
    // 2. 先判断两个将是否在同一列
    if(GetCol(red_king_pos) != GetCol(black_king_pos)){
        return false;
    }
    // 3. 判断是否对将
    return (ROOK_CANNON_CAN_GET_COL_MASK[GetRow(red_king_pos) - 3][situation.bit_col[GetCol(red_king_pos)]].rook_capture & BIT_COL_MASK[black_king_pos]) != 0;
}

/* 
 * 函数名：MovementsLegal
 * 描述：判断走法是否合法
 * 入参：
 * - const Movement move : 需要判断的走法
 * - const Situation & situation :当前局面
 * 返回值：
 * - bool 该走法在当前局面是否合法
 * 最后更新时间: 21.04.05
 */
bool MovementsLegal(const Movement move, const Situation & situation){
    int from = move.from, to = move.to, player_flag = GetPlayerFlag(situation.current_player), player = situation.current_player;
    int piece_id_from = situation.current_board[from];
    int from_col = 0, from_row = 0, to_col = 0, to_row = 0;
    // 1. 判断出发点是否为自己的棋子(包含判断是否是没有棋子)
    if((piece_id_from & player_flag) == 0){
        return false;
    }
    // 2. 对目标点进行判断
    if(situation.current_board[to] != move.capture){
        return false;
    }
    // 3.对出发点的棋子进行分类讨论
    switch (piece_id_from - player_flag){
    // 3.1 将(帅)
    case 0:
        return (LEGAL_POSITION[player][to] & POSITION_MASK[0]) && (LEGAL_MOVE[to - from + 256] == 1);
    // 3.2 仕
    case 1:
    case 2:
        return (LEGAL_POSITION[player][to] & POSITION_MASK[1]) && (LEGAL_MOVE[to - from + 256] == 2);
    // 3.3 象
    case 3:
    case 4:
        return (LEGAL_POSITION[player][to] & POSITION_MASK[2]) && (LEGAL_MOVE[to - from + 256] == 3) && (situation.current_board[(to + from) >> 1] == 0);
    // 3.4 马
    case 5:
    case 6: {
        int horse_leg = HORSE_LEGAL_MOVE[to - from + 256] + from;
        return (horse_leg != from) && (situation.current_board[horse_leg] == 0);
    }
    // 3.5 车
    case 7:
    case 8:
        from_col = GetCol(from), from_row = GetRow(from), to_col = GetCol(to), to_row = GetRow(to);
        // 3.5.1 如果在同一列
        if(from_col == to_col){
            if(move.capture){
                return (ROOK_CANNON_CAN_GET_COL_MASK[from_row - 3][situation.bit_col[from_col]].rook_capture & BIT_COL_MASK[to]) != 0;
            }
            else{
                return (ROOK_CANNON_CAN_GET_COL_MASK[from_row - 3][situation.bit_col[from_col]].no_capture & BIT_COL_MASK[to]) != 0;
            }
        }
        // 3.5.2 如果在同一行
        else if(from_row == to_row){
            if(move.capture){
                return (ROOK_CANNON_CAN_GET_ROW_MASK[from_col - 3][situation.bit_row[from_row]].rook_capture & BIT_ROW_MASK[to]) != 0;
            }
            else{
                return (ROOK_CANNON_CAN_GET_ROW_MASK[from_col - 3][situation.bit_row[from_row]].no_capture & BIT_ROW_MASK[to]) != 0;
            }
        }
        // 3.5.2 不在同一行列
        else{
            return false;
        }
    // 3.6 炮
    case 9:
    case 10:
        from_col = GetCol(from), from_row = GetRow(from), to_col = GetCol(to), to_row = GetRow(to);
        // 3.6.1 如果在同一列
        if(from_col == to_col){
            if(move.capture){
                return (ROOK_CANNON_CAN_GET_COL_MASK[from_row - 3][situation.bit_col[from_col]].cannon_capture & BIT_COL_MASK[to]) != 0;
            }
            else{
                return (ROOK_CANNON_CAN_GET_COL_MASK[from_row - 3][situation.bit_col[from_col]].no_capture & BIT_COL_MASK[to]) != 0;
            }
        }
        // 3.6.2 如果在同一行
        else if(from_row == to_row){
            if(move.capture){
                return (ROOK_CANNON_CAN_GET_ROW_MASK[from_col - 3][situation.bit_row[from_row]].cannon_capture & BIT_ROW_MASK[to]) != 0;
            }
            else{
                return (ROOK_CANNON_CAN_GET_ROW_MASK[from_col - 3][situation.bit_row[from_row]].no_capture & BIT_ROW_MASK[to]) != 0;
            }
        }
        // 3.6.2 不在同一行列
        else{
            return false;
        }
    // 3.7 兵
    default:
        if(!(LEGAL_POSITION[player][to] & POSITION_MASK[6])){
            return false;
        }
        for(int i = 0; PAWN_CAN_GET[from][player][i] != 0; i ++){
            if(PAWN_CAN_GET[from][player][i] == to){
                return true;
            }
        }
        return false;
    }
}

/* 
 * 函数名：IfProtected
 * 描述：判断该位置是否被保护
 * 入参：
 * - int player 玩家
 * - const int to 判断的位置
 * - const Situation & situation 当前局面 
 * - int exp 排除的位置
 * 返回值：
 * - bool 该走法在当前局面是否合法
 * 最后更新时间: 21.05.14
 */
bool IfProtected(int player, const int to, const Situation & situation, int exp){
    // 对手的player_flag
    int player_flag = GetPlayerFlag(player);

    // 对手的棋子所在的位置，即判断保护的from
    int from;
    if(situation.current_board[to] != 0 && ColorOfPiece(situation.current_board[to]) != player) 
        return false;
    
    // 是否过河
    if(InHomeHalf(to, player)){
        // 被将(帅)保护
        for(int i = player_flag + 0; i <= player_flag + 0; i ++){
            from = situation.current_pieces[i];
            if (from && (LEGAL_POSITION[player][to] & POSITION_MASK[0]) && (LEGAL_MOVE[to - from + 256] == 1) && from != exp)
                return true;
        }

        // 被士(仕)保护
        for(int i = player_flag + 1; i <= player_flag + 2; i ++){
            from = situation.current_pieces[i];
            if (from && (LEGAL_POSITION[player][to] & POSITION_MASK[1]) && (LEGAL_MOVE[to - from + 256] == 2) && from != exp)
                return true;
        }

        // 被象(相)保护
        for(int i = player_flag + 3; i <= player_flag + 4; i ++){
            from = situation.current_pieces[i];
            if(from && (LEGAL_POSITION[player][to] & POSITION_MASK[2]) && (LEGAL_MOVE[to - from + 256] == 3) && (situation.current_board[(to + from) >> 1] == 0) && from != exp)
                return true;
        }
    }
    else{
        // 被兵保护(横向)
        for(from = to - 1; from <= to + 1; from += 2){
            int from_id = situation.current_board[from];
            if(((from_id & player_flag) != 0) && ((from_id & 15) >= 11) && from != exp)
                return true;
        }
    }

    // 被马保护
    for(int i = player_flag + 5; i <= player_flag + 6; i ++){
        from = situation.current_pieces[i];
        int horse_leg = HORSE_LEGAL_MOVE[to - from + 256] + from;
        if(from && horse_leg != from && situation.current_board[horse_leg] == 0 && from != exp)
            return true;
    }

    int from_col = GetCol(from), from_row = GetRow(from), to_col = GetCol(to), to_row = GetRow(to);

    // 被车保护
    for(int i = player_flag + 7; i <= player_flag + 8; i ++){
        from = situation.current_pieces[i];
        from_col = GetCol(from);
        from_row = GetRow(from);
        if(from && from != to){
            if(from_row == to_row){
                if((ROOK_CANNON_CAN_GET_ROW_MASK[from_col - 3][situation.bit_row[from_row]].rook_capture & BIT_ROW_MASK[to]) != 0 && from != exp)
                    return true;
            }
            else if(from_col == to_col){
                if((ROOK_CANNON_CAN_GET_COL_MASK[from_row - 3][situation.bit_col[from_col]].rook_capture & BIT_COL_MASK[to]) != 0 && from != exp)
                    return true;
            }
        }
    }

    // 被炮保护
    for(int i = player_flag + 9; i <= player_flag + 10; i ++){
        from = situation.current_pieces[i];
        from_col = GetCol(from);
        from_row = GetRow(from);
        if(from && from != to){
            if(from_row == to_row){
                if((ROOK_CANNON_CAN_GET_ROW_MASK[from_col - 3][situation.bit_row[from_row]].cannon_capture & BIT_ROW_MASK[to]) != 0 && from != exp)
                    return true;
            }
            else if(from_col == to_col){
                if((ROOK_CANNON_CAN_GET_COL_MASK[from_row - 3][situation.bit_col[from_col]].cannon_capture & BIT_COL_MASK[to]) != 0 && from != exp)
                    return true;
            }
        }
    }

    // 被兵保护(纵向)
    from = SquareBack(to, player);
    int from_id = situation.current_board[from];
    if(((from_id & player_flag) != 0) && ((from_id & 15) >= 11) && from != exp)
        return true;

    return false;
}