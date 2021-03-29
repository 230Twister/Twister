/* 
 * 文件名： value.cpp
 * 描述	：评估局面价值
 * 作者 : yuanthree
 * 最后更新时间: 21.03.26
 */
#include "value.h"

void Eval(Situation &s)
{
    INT16 i, j, k, r;
    UINT8 p;      //棋子位置
    UINT8 n;      //用于记录下一步的可能位置
    UINT8 m;      //用于记录马腿象眼位置
    int SideTag;  //用于标志行走方
    int OverFlag; //炮的翻山标志
    INT16 bValue, wValue = 0;
    INT16 fValue[2] = {0, 0};

    //计算位置价值
    for (i = 16; i < 32; i++)
    {
        if (s.current_pieces[i] > 0)
            wValue += PositionValues[0][PieceNumToType[i]][s.current_pieces[i]];
    }
    for (i = 32; i < 48; i++)
    {
        if (s.current_pieces[i] > 0)
            bValue += PositionValues[1][PieceNumToType[i]][s.current_pieces[i]];
    }

    for (r = 0; r <= 1; r++)
    {
        SideTag = 16 + r << 4;

        // //将的灵活性
        // for (i = 0; i <= 0; i++)
        // {
        //     p = s.current_pieces[SideTag];
        //     if (!p)
        //         continue;
        //     for (k = 0; k < 4; k++) //4个方向
        //     {
        //         n = p + KING_DIRECTION[k]; //n为新的可能走到的位置
        //         if (LEGAL_POSITION[s.current_player][n] & POSITION_MASK[0])
        //         {
        //             if (!(s.current_board[n] & SideTag)) //该位置没有本方棋子
        //                 fValue[r] += 2;
        //         }
        //     }
        // }

        //将的灵活性
        for (i = 0; i <= 0; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int can_move_counter = 0;
                int piece_to = KING_CAN_GET[piece_from][r][can_move_counter];
                while (piece_to != 0)
                {
                    fValue[r] += 2;
                    can_move_counter++;
                    piece_to = KING_CAN_GET[piece_from][r][can_move_counter];
                }
            }
        }

        // //士的灵活性
        // for (i = 1; i <= 2; i++)
        // {
        //     p = s.current_pieces[SideTag + i];
        //     if (!p)
        //         continue;
        //     for (k = 0; k < 4; k++) //4个方向
        //     {
        //         n = p + ADVISOR_DIRECTION[k]; //n为新的可能走到的位置
        //         if (LEGAL_POSITION[s.current_player][n] & POSITION_MASK[1])
        //         {
        //             if (!(s.current_board[n] & SideTag)) //该位置没有本方棋子
        //                 fValue[r] += 2;
        //         }
        //     }
        // }

        //士的灵活性
        for (i = 1; i <= 2; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int can_move_counter = 0;
                int piece_to = ADVISOR_CAN_GET[piece_from][r][can_move_counter];
                while (piece_to != 0)
                {
                    fValue[r] += 2;
                    can_move_counter++;
                    piece_to = ADVISOR_CAN_GET[piece_from][r][can_move_counter];
                }
            }
        }

        // //象的灵活性
        // for (i = 3; i <= 4; i++)
        // {
        //     p = s.current_pieces[SideTag + i];
        //     if (!p)
        //         continue;
        //     for (k = 0; k < 4; k++) //4个方向
        //     {
        //         n = p + BISHOP_DIRECTION[k]; //n为新的可能走到的位置
        //         if (LEGAL_POSITION[s.current_player][n] & POSITION_MASK[2])
        //         {
        //             m = p + BISHOP_EYE_DIRECTION[k];
        //             if (!s.current_board[m] && !(s.current_board[n] & SideTag)) //象眼无棋子占据且该位置没有本方棋子
        //                 fValue[r] += 2;
        //         }
        //     }
        // }

        //象的灵活性
        for (i = 3; i <= 4; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int can_move_counter = 0;
                int piece_to = BISHOP_CAN_GET[piece_from][r][can_move_counter];
                while (piece_to != 0)
                {
                    int bishop_eye = BISHOP_EYE[piece_from][r][can_move_counter];
                    if (s.current_board[bishop_eye] == 0)
                        fValue[r] += 2;
                    can_move_counter++;
                    piece_to = BISHOP_CAN_GET[piece_from][r][can_move_counter];
                }
            }
        }

        // //马的灵活性
        // for (i = 5; i <= 6; i++)
        // {
        //     p = s.current_pieces[SideTag + i];
        //     if (!p)
        //         continue;
        //     for (k = 0; k < 8; k++) //4个方向
        //     {
        //         n = p + HORSE_DIRECTION[k]; //n为新的可能走到的位置
        //         if (LEGAL_POSITION[s.current_player][n] & POSITION_MASK[3])
        //         {
        //             m = p + HORSE_LEG_DIRECTION[k];
        //             if (!s.current_board[m] && !(s.current_board[n] & SideTag)) //马腿无棋子占据且该位置没有本方棋子
        //                 fValue[r] += 5;
        //         }
        //     }
        // }

        //马的灵活性
        for (i = 5; i <= 6; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[SideTag + i];
                int can_move_counter = 0;
                int piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
                while (piece_to != 0)
                {
                    int horse_leg = HORSE_LEG[piece_from][can_move_counter];
                    if (s.current_board[horse_leg] == 0)
                        fValue[r] += 5;
                    can_move_counter++;
                    piece_to = HORSE_CAN_GET[piece_from][can_move_counter];
                }
            }
        }

        // //車的灵活性
        // for (i = 7; i <= 8; i++)
        // {
        //     p = s.current_pieces[SideTag + i];
        //     if (!p)
        //         continue;
        //     for (k = 0; k < 4; k++) //4个方向
        //     {
        //         for (j = 1; j < 10; j++) //横向最多有8个位置可走，纵向9个
        //         {
        //             n = p + j * KING_DIRECTION[k];
        //             if (!(LEGAL_POSITION[s.current_player][n] & POSITION_MASK[4]))
        //                 break;               //不合理位置
        //             if (!s.current_board[n]) //目标位置没有子
        //                 fValue[r] += 4;
        //             else if (s.current_board[n] & SideTag) //目标位置有本方子
        //                 break;
        //             else //目标位置有对方棋子
        //             {
        //                 fValue[r] += 4;
        //                 break;
        //             }
        //         }
        //     }
        // }

        //車的吃子灵活性
        for (i = 7; i <= 8; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[i];
                int col = GetCol(piece_from), row = GetRow(piece_from);
                // 右 左 方向的吃子
                for (int j = 0; j < 2; j++)
                {
                    int capture_col = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].rook_capture[j];
                    if (capture_col != col)
                        fValue[r] += 4 * abs(capture_col - col);
                }
                // 下 上 方向的吃子
                for (int j = 0; j < 2; j++)
                {
                    int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].rook_capture[j];
                    if (capture_row != row)
                        fValue[r] += 4 * abs(capture_row - row);
                }
            }
        }

        //炮的吃子灵活性
        for (i = 9; i <= 10; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[i];
                int col = GetCol(piece_from), row = GetRow(piece_from);
                // 右 左 方向的吃子
                for (int j = 0; j < 2; j++)
                {
                    int capture_col = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].connon_capture[j];
                    if (capture_col != col)
                        fValue[r] += 3 * abs(capture_col - col);
                }
                // 下 上 方向的吃子
                for (int j = 0; j < 2; j++)
                {
                    int capture_row = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].connon_capture[j];
                    if (capture_row != row)
                        fValue[r] += 3 * abs(capture_row - row);
                }
            }
        }

        //車和炮的不吃子灵活性
        for (i = 7; i <= 10; i++)
        {
            if (s.current_pieces[SideTag + i] != 0)
            {
                int piece_from = s.current_pieces[i];
                int col = GetCol(piece_from), row = GetRow(piece_from);
                // 右方向
                int get_col = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].no_capture[0];
                while (get_col != col)
                {
                    if (i <= 8)
                        fValue[r] += 4;
                    else
                        fValue[r] += 3;
                    get_col--;
                }

                // 左方向
                get_col = ROOK_CANNON_CAN_GET_ROW[col - 3][s.bit_row[row]].no_capture[1];
                while (get_col != col)
                {
                    if (i <= 8)
                        fValue[r] += 4;
                    else
                        fValue[r] += 3;
                    get_col++;
                }

                // 下方向
                int get_row = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[0];
                while (get_row != row)
                {
                    if (i <= 8)
                        fValue[r] += 4;
                    else
                        fValue[r] += 3;
                    get_row--;
                }

                // 上方向
                get_row = ROOK_CANNON_CAN_GET_COL[row - 3][s.bit_col[col]].no_capture[1];
                while (get_row != row)
                {
                    if (i <= 8)
                        fValue[r] += 4;
                    else
                        fValue[r] += 3;
                    get_row++;
                }
            }
        }

        //     //兵的灵活性
        //     for (i = 11; i <= 15; i++)
        //     {
        //         p = s.current_pieces[SideTag + i];
        //         if (!p)
        //             continue;
        //         for (k = 0; k < 3; k++) //3个方向
        //         {
        //             n = p + PAWN_DIRECTION[s.current_player][k]; //n为新的可能走到的位置
        //             if (LEGAL_POSITION[s.current_player][n] & POSITION_MASK[6])
        //             {
        //                 if (!(s.current_board[n] & SideTag)) //马腿无棋子占据且该位置没有本方棋子
        //                     fValue[r] += 2;
        //             }
        //         }
        //     }
        // }

        //兵的灵活性
        for (i = 11; i <= 15; i++)
        {
            int piece_from = s.current_pieces[SideTag + i];
            int can_move_counter = 0;
            int piece_to = PAWN_CAN_GET[piece_from][r][can_move_counter];
            while (piece_to != 0)
            {
                fValue[r] += 2;
                can_move_counter++;
                piece_to = PAWN_CAN_GET[piece_from][r][can_move_counter];
            }
        }
    }

    s.value_red = fValue[0] + wValue;
    s.value_black = fValue[1] + bValue;
}