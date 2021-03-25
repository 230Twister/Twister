/* 
 * 文件名： moves_getter.cpp
 * 描述	：实现所有着法的生成器
 * 作者 : yao
 * 最后更新时间: 21.02.22 
 */
#include "representation.h"
#include "define.h"
#include "preset.h"

/* 
 * 函数名：GetAllMovements
 * 描述：生成所有着法
 * 入参：
 * - const Situation & situation：当前局面
 * - int & num_of_all_movements：当前着法数
 * - Movement* all_movements ：	当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.03.23
 */
void GetAllMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements){
    GetAllCaptureMovements(situation, num_of_all_movements, all_movements);
    GetAllNotCaptureMovements(situation, num_of_all_movements, all_movements);
}

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
void GetAllCaptureMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements){
    // 1. 获得玩家特征值 
    int player_flag = GetPlayerFlag(situation.current_player), player = situation.current_player;
    int piece_from, piece_to, can_move_counter, piece_to_id;
    // 2. 将(帅)的吃子着法
    for(int i = player_flag + 0; i <= player_flag + 0; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = KING_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id != 0 && ColorOfPiece(piece_to_id) != player){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = KING_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }

    // 3. 士(仕)的吃子着法
    for(int i = player_flag + 1; i <= player_flag + 2; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = ADVISOR_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id != 0 && ColorOfPiece(piece_to_id) != player){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = ADVISOR_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }
    // 4. 象(相)的吃子着法
    for(int i = player_flag + 3; i <= player_flag + 4; i ++){
        if(situation.current_pieces[i] != 0){
            int bishop_eye;                  // 象眼位置
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = BISHOP_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                bishop_eye = BISHOP_EYE[piece_from][player][can_move_counter];
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id != 0 && ColorOfPiece(piece_to_id) != player && situation.current_board[bishop_eye] == 0){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = BISHOP_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }

    // 5. 马的吃子着法
    for(int i = player_flag + 5; i <= player_flag + 6; i ++){
        if(situation.current_pieces[i] != 0){
            int horse_leg;                  // 马腿位置
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
            while(piece_to != 0){
                horse_leg = HORSE_LEG[piece_from][can_move_counter];
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id != 0 && ColorOfPiece(piece_to_id) != player && situation.current_board[horse_leg] == 0){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
            }
        }
    }

    // 6. 车的吃子着法
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
                    if(ColorOfPiece(piece_to_id) != player){
                        all_movements[num_of_all_movements].capture = piece_to_id;
                        all_movements[num_of_all_movements].from = piece_from;
                        all_movements[num_of_all_movements].to = piece_to;
                        num_of_all_movements ++;
                    }
                }
            }
            // 下 上 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][situation.bit_col[col]].rook_capture[j];
                if(capture_row != row){
                    piece_to = GetPosition(col, capture_row);
                    piece_to_id = situation.current_board[piece_to];
                    if(ColorOfPiece(piece_to_id) != player){
                        all_movements[num_of_all_movements].capture = piece_to_id;
                        all_movements[num_of_all_movements].from = piece_from;
                        all_movements[num_of_all_movements].to = piece_to;
                        num_of_all_movements ++;
                    }
                }
            }
        
        }
    }
    
    // 7. 炮的吃子着法
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
                    if(ColorOfPiece(piece_to_id) != player){
                        all_movements[num_of_all_movements].capture = piece_to_id;
                        all_movements[num_of_all_movements].from = piece_from;
                        all_movements[num_of_all_movements].to = piece_to;
                        num_of_all_movements ++;
                    }
                }
            }
            // 下 上 方向的吃子
            for(int j = 0; j < 2; j ++){
                int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][situation.bit_col[col]].connon_capture[j];
                if(capture_row != row){
                    piece_to = GetPosition(col, capture_row);
                    piece_to_id = situation.current_board[piece_to];
                    if(ColorOfPiece(piece_to_id) != player){
                        all_movements[num_of_all_movements].capture = piece_to_id;
                        all_movements[num_of_all_movements].from = piece_from;
                        all_movements[num_of_all_movements].to = piece_to;
                        num_of_all_movements ++;
                    }
                }
            }
        
        }
    }

    // 8. 兵(卒)的吃子着法
    for(int i = player_flag + 11; i <= player_flag + 15; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = PAWN_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id != 0 && ColorOfPiece(piece_to_id) != player){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = PAWN_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }
}

/* 
 * 函数名：GetAllMovements
 * 描述：生成所有不吃子着法
 * 入参：
 * - const Situation & situation：当前局面
 * - int & num_of_all_movements：当前着法数
 * - Movement* all_movements ：	当前着法数组，存储当前所有着法
 * 返回值：
 * - void
 * 最后更新时间: 21.03.24
 */
void GetAllNotCaptureMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements){
    // 1. 获得玩家特征值 
    int player_flag = GetPlayerFlag(situation.current_player), player = situation.current_player;
    int piece_from, piece_to, can_move_counter, piece_to_id;

    // 2. 将(帅)的不吃子着法
    for(int i = player_flag + 0; i <= player_flag + 0; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = KING_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id == 0){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = KING_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }

    // 3. 士(仕)的不吃子着法
    for(int i = player_flag + 1; i <= player_flag + 2; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = ADVISOR_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id == 0){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = ADVISOR_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }
    // 4. 象(相)的不吃子着法
    for(int i = player_flag + 3; i <= player_flag + 4; i ++){
        if(situation.current_pieces[i] != 0){
            int bishop_eye;                  // 象眼位置
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = BISHOP_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                bishop_eye = BISHOP_EYE[piece_from][player][can_move_counter];
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id == 0 && situation.current_board[bishop_eye] == 0){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = BISHOP_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }

    // 5. 马的不吃子着法
    for(int i = player_flag + 5; i <= player_flag + 6; i ++){
        if(situation.current_pieces[i] != 0){
            int horse_leg;                  // 马腿位置
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
            while(piece_to != 0){
                horse_leg = HORSE_LEG[piece_from][can_move_counter];
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id == 0 && situation.current_board[horse_leg] == 0){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
            }
        }
    }

    // 6. 车和炮的不吃子着法
    for(int i = player_flag + 7; i <= player_flag + 10; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            int col = GetCol(piece_from), row = GetRow(piece_from);

            // 右方向
            int get_col = ROOK_CANNON_CAN_GET_ROW[col - 3][situation.bit_row[row]].no_capture[0];
            while(get_col != col){
                piece_to = GetPosition(get_col, row);
                piece_to_id = 0;
                all_movements[num_of_all_movements].capture = piece_to_id;
                all_movements[num_of_all_movements].from = piece_from;
                all_movements[num_of_all_movements].to = piece_to;
                num_of_all_movements ++;
                get_col --;
            }

            // 左方向
            get_col = ROOK_CANNON_CAN_GET_ROW[col - 3][situation.bit_row[row]].no_capture[1];
            while(get_col != col){
                piece_to = GetPosition(get_col, row);
                piece_to_id = 0;
                all_movements[num_of_all_movements].capture = piece_to_id;
                all_movements[num_of_all_movements].from = piece_from;
                all_movements[num_of_all_movements].to = piece_to;
                num_of_all_movements ++;
                get_col ++;
            }

            // 下方向
            int get_row = ROOK_CANNON_CAN_GET_COL[row - 3][situation.bit_col[col]].no_capture[0];
            while(get_row != row){
                piece_to = GetPosition(col, get_row);
                piece_to_id = 0;
                all_movements[num_of_all_movements].capture = piece_to_id;
                all_movements[num_of_all_movements].from = piece_from;
                all_movements[num_of_all_movements].to = piece_to;
                num_of_all_movements ++;
                get_row --;
            }

            // 上方向
            get_row = ROOK_CANNON_CAN_GET_COL[row - 3][situation.bit_col[col]].no_capture[1];
            while(get_row != row){
                piece_to = GetPosition(col, get_row);
                piece_to_id = 0;
                all_movements[num_of_all_movements].capture = piece_to_id;
                all_movements[num_of_all_movements].from = piece_from;
                all_movements[num_of_all_movements].to = piece_to;
                num_of_all_movements ++;
                get_row ++;
            }
            
        }
    }
    
    // 7. 兵(卒)的不吃子着法
    for(int i = player_flag + 11; i <= player_flag + 15; i ++){
        if(situation.current_pieces[i] != 0){
            piece_from = situation.current_pieces[i];
            can_move_counter = 0;
            piece_to = PAWN_CAN_GET[piece_from][player][can_move_counter];
            while(piece_to != 0){
                piece_to_id = situation.current_board[piece_to];
                if(piece_to_id == 0){
                    all_movements[num_of_all_movements].capture = piece_to_id;
                    all_movements[num_of_all_movements].from = piece_from;
                    all_movements[num_of_all_movements].to = piece_to;
                    num_of_all_movements ++;
                }
                can_move_counter ++;
                piece_to = PAWN_CAN_GET[piece_from][player][can_move_counter];
            }
        }
    }
}   