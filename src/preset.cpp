/* 
 * 文件名： preset.cpp
 * 描述	： 产生用于加快着法生成的预置数组
 * 作者 : yao
 * 最后更新时间: 21.02.23
 */
#include "preset.h"

int HORSE_CAN_GET[256][10];             // 马在各个位置所能到的地方
int HORSE_LEG[256][10];                 // 马在各个位置马腿所在的地方
int KING_CAN_GET[256][2][10];           // 将(帅)在各个位置所能到的地方
int ADVISOR_CAN_GET[256][2][10];   	    // 士(仕)在各个位置所能到的地方
int PAWN_CAN_GET[256][2][10];           // 兵(卒)在各个位置所能到的地方
int BISHOP_CAN_GET[256][2][10];     	// 象在各个位置所能到的地方
int BISHOP_EYE[256][2][10];        	    // 象在各个位置象眼所在的地方

RookCannonCanGet ROOK_CANNON_CAN_GET_ROW[9][1 << 9];         // 车炮在某行方向上的着法
RookCannonCanGet ROOK_CANNON_CAN_GET_COL[10][1 << 10];       // 车炮在某列方向上的着法
RookCannonMask ROOK_CANNON_CAN_GET_ROW_MASK[9][1 << 9];      // 车炮在某行方向上的着法屏蔽位用于合理性判断
RookCannonMask ROOK_CANNON_CAN_GET_COL_MASK[10][1 << 10];    // 车炮在某列方向上的着法屏蔽位用于合理性判断
UINT16 BIT_ROW_MASK[256];                                    // 行屏蔽位
UINT16 BIT_COL_MASK[256];                                    // 列屏蔽位

// 合法跨度数组 1表示将跨度合法 2表示仕跨度合理 3表示象跨度合理
const UINT8 LEGAL_MOVE[512] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 2, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
// 马的合法跨度数组 0表示不能走 非0表示该步的马脚增量
const int HORSE_LEGAL_MOVE[512] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,-16,  0,-16,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0, 16,  0, 16,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0
};

/* 
 * 函数名：
 * 描述：生成预置数组
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.03.23
 */
void InitPresetArray(){
    // 1. 初始化预置数组
    for(int i = 0; i < 256; i ++){
        // 1.1. 初始化其他预置数组
        for(int j = 0; j < 10; j ++){
            HORSE_CAN_GET[i][j] = 0;
            HORSE_CAN_GET[i][j] = 0;
            BISHOP_CAN_GET[i][0][j] = 0;
            BISHOP_CAN_GET[i][1][j] = 0;
            KING_CAN_GET[i][0][j] = 0;
            KING_CAN_GET[i][1][j] = 0;
            BISHOP_EYE[i][0][j] = 0;
            BISHOP_EYE[i][1][j] = 0;
            ADVISOR_CAN_GET[i][0][j] = 0;
            ADVISOR_CAN_GET[i][1][j] = 0;
            PAWN_CAN_GET[i][0][j] = 0;
            PAWN_CAN_GET[i][1][j] = 0;
        }

        // 1.2. 初始化位行位列屏蔽位
        if(ON_BOARD[i]){
            BIT_ROW_MASK[i] = 1 << RowBitOpration(GetCol(i) - 3);
            BIT_COL_MASK[i] = 1 << ColBitOpration(GetRow(i) - 3);
        }
        else{
            BIT_ROW_MASK[i] = 0;
            BIT_ROW_MASK[i] = 0;
        }
    }
    int position = BOARD_FIRST_POSITION;    // 初始位置为棋盘上的第一个位置

    // 2.循环棋盘上的每一个位置
    //   (备注： POSITION_MASK[7] = {2, 4, 16, 1, 1, 1, 8} 各子力的特征值 将士相车马炮卒)
    while(position != 0){
        int num_of_can_get = 0;             // 预生成着法的个数
        int next_position;                  // 着后的下一个位置

        // 2.1. 生成帅（红）的在该位置的可行走法
        if(LEGAL_POSITION[RED][position] & POSITION_MASK[0]){
            for(int i = 0; i < 4; i ++){
                next_position = position + KING_DIRECTION[i];
                if(LEGAL_POSITION[RED][next_position] & POSITION_MASK[0]){
                    KING_CAN_GET[position][RED][num_of_can_get] = next_position;
                    num_of_can_get ++;
                }
            }
        }
        num_of_can_get = 0;

        // 2.2. 生成将（黑）在该位置的可行走法
        if(LEGAL_POSITION[BLACK][position] & POSITION_MASK[0]){
            for(int i = 0; i < 4; i ++){
                next_position = position + KING_DIRECTION[i];
                if(LEGAL_POSITION[BLACK][next_position] & POSITION_MASK[0]){
                    KING_CAN_GET[position][BLACK][num_of_can_get] = next_position;
                    num_of_can_get ++;
                }
            }
        }
        num_of_can_get = 0;

        
        // 2.3. 生成仕(红)在该位置的可行走法
        if(LEGAL_POSITION[RED][position] & POSITION_MASK[1]){
            for(int i = 0; i < 4; i ++){
                next_position = position + ADVISOR_DIRECTION[i];
                if(LEGAL_POSITION[RED][next_position] & POSITION_MASK[1]){
                    ADVISOR_CAN_GET[position][RED][num_of_can_get] = next_position;
                    num_of_can_get ++;
                }
            }
        }
        num_of_can_get = 0;

        // 2.3. 生成士(黑)在该位置的可行走法
        if(LEGAL_POSITION[BLACK][position] & POSITION_MASK[1]){
            for(int i = 0; i < 4; i ++){
                next_position = position + ADVISOR_DIRECTION[i];
                if(LEGAL_POSITION[BLACK][next_position] & POSITION_MASK[1]){
                    ADVISOR_CAN_GET[position][BLACK][num_of_can_get] = next_position;
                    num_of_can_get ++;
                }
            }
        }
        num_of_can_get = 0;

        // 2.4. 生成象(红)在该位置的可行走法
        if(LEGAL_POSITION[RED][position] & POSITION_MASK[2]){
            for(int i = 0; i < 4; i ++){
                next_position = position + BISHOP_DIRECTION[i];
                if(LEGAL_POSITION[RED][next_position] & POSITION_MASK[2]){
                    BISHOP_CAN_GET[position][RED][num_of_can_get] = next_position;
                    BISHOP_EYE[position][RED][num_of_can_get] = position + BISHOP_EYE_DIRECTION[i];
                    num_of_can_get ++;
                }
            }
        }
        num_of_can_get = 0;

        // 2.5. 生成象(黑)在该位置的可行走法
        if(LEGAL_POSITION[BLACK][position] & POSITION_MASK[2]){
            for(int i = 0; i < 4; i ++){
                next_position = position + BISHOP_DIRECTION[i];
                if(LEGAL_POSITION[BLACK][next_position] & POSITION_MASK[2]){
                    BISHOP_CAN_GET[position][BLACK][num_of_can_get] = next_position;
                    BISHOP_EYE[position][BLACK][num_of_can_get] = position + BISHOP_EYE_DIRECTION[i];
                    num_of_can_get ++;
                }
            }
        }
        num_of_can_get = 0;

        // 2.6. 生成马在该位置的可行走法
        if(LEGAL_POSITION[RED][position] & POSITION_MASK[4]){
            for(int i = 0; i < 8; i ++){
                next_position = position + HORSE_DIRECTION[i];
                if(LEGAL_POSITION[RED][next_position] & POSITION_MASK[4]){
                    HORSE_CAN_GET[position][num_of_can_get] = next_position;
                    HORSE_LEG[position][num_of_can_get] = position + HORSE_LEG_DIRECTION[i];
                    num_of_can_get ++;
                }
            }
        }
        num_of_can_get = 0;

        // 2.7. 生成兵(红)在该位置的可行走法
        if(LEGAL_POSITION[RED][position] & POSITION_MASK[6]){
            for(int i = 0; i < 3; i ++){
                next_position = position + PAWN_DIRECTION[RED][i];
                if(LEGAL_POSITION[RED][next_position] & POSITION_MASK[6]){
                    PAWN_CAN_GET[position][RED][num_of_can_get] = next_position; 
                    num_of_can_get ++;
                }
            }
        }
        num_of_can_get = 0;

        // 2.8. 生成兵(黑)在该位置的可行走法
        if(LEGAL_POSITION[BLACK][position] & POSITION_MASK[6]){
            for(int i = 0; i < 3; i ++){
                next_position = position + PAWN_DIRECTION[BLACK][i];
                if(LEGAL_POSITION[BLACK][next_position] & POSITION_MASK[6]){
                    PAWN_CAN_GET[position][BLACK][num_of_can_get] = next_position; 
                    num_of_can_get ++;
                }
            }
        }
        num_of_can_get = 0;

        position = NextBoardPosition(position);
    }

    // 3.生成位行位列预置数组
    
    // 3.1. 循环一行中的每一列
    for(int i = 0; i < 9; i ++){
        // 循环一行中的每种状态(位置是否有棋子)
        for(int j = 0; j < (1 << 9); j ++){
            // 3.1.1 将着法初始化(用当前位置初始化)
            ROOK_CANNON_CAN_GET_ROW[i][j].connon_capture[0] = ROOK_CANNON_CAN_GET_ROW[i][j].connon_capture[1] = i + 3;
            ROOK_CANNON_CAN_GET_ROW[i][j].no_capture[0] = ROOK_CANNON_CAN_GET_ROW[i][j].no_capture[1] = i + 3;
            ROOK_CANNON_CAN_GET_ROW[i][j].rook_capture[0] = ROOK_CANNON_CAN_GET_ROW[i][j].rook_capture[1] = i + 3;
            ROOK_CANNON_CAN_GET_ROW_MASK[i][j].cannon_capture = ROOK_CANNON_CAN_GET_ROW_MASK[i][j].no_capture = ROOK_CANNON_CAN_GET_ROW_MASK[i][j].rook_capture = 0;
            int col;
            // 3.1.2 生成车炮右边的着法
            for(col = i + 1; col < 9; col ++){
                // 此状态下，该列没有棋子
                if((j & (1 << RowBitOpration(col))) == 0){
                    ROOK_CANNON_CAN_GET_ROW[i][j].no_capture[0] = col + 3;
                    ROOK_CANNON_CAN_GET_ROW_MASK[i][j].no_capture |= (1 << RowBitOpration(col));
                }
                // 此状态下，该列有棋子,可生成吃子着法
                else{
                    ROOK_CANNON_CAN_GET_ROW[i][j].rook_capture[0] = col + 3;
                    ROOK_CANNON_CAN_GET_ROW_MASK[i][j].rook_capture |= (1 << RowBitOpration(col));
                    break;
                }
            }
            for(col++; col < 9; col ++){
                // 已经隔了一子，判断炮是否有吃子着法
                if((j & (1 << RowBitOpration(col))) != 0){
                    ROOK_CANNON_CAN_GET_ROW[i][j].connon_capture[0] = col + 3;
                    ROOK_CANNON_CAN_GET_ROW_MASK[i][j].cannon_capture |= (1 << RowBitOpration(col));
                    break;
                }
            }
            // 3.1.3 生成车炮左边的着法
            for(col = i - 1; col >= 0; col--){
                // 此状态下，该列没有棋子
                if((j & (1 << RowBitOpration(col))) == 0){
                    ROOK_CANNON_CAN_GET_ROW[i][j].no_capture[1] = col + 3;
                    ROOK_CANNON_CAN_GET_ROW_MASK[i][j].no_capture |= (1 << RowBitOpration(col));
                }
                // 此状态下，该列有棋子,可生成吃子着法
                else{
                    ROOK_CANNON_CAN_GET_ROW[i][j].rook_capture[1] = col + 3;
                    ROOK_CANNON_CAN_GET_ROW_MASK[i][j].rook_capture |= (1 << RowBitOpration(col));
                    break;
                }
            }
            for(col --; col >=0; col --){
                // 已经隔了一子，判断炮是否有吃子着法
                if((j & (1 << RowBitOpration(col))) != 0){
                    ROOK_CANNON_CAN_GET_ROW[i][j].connon_capture[1] = col + 3;
                    ROOK_CANNON_CAN_GET_ROW_MASK[i][j].cannon_capture |= (1 << RowBitOpration(col));
                    break;
                }
            }
        }
    }
    // 3.2. 循环一列中的每一行
    for(int i = 0; i < 10; i ++){
        // 循环一列的每一个状态
        for(int j = 0; j < (1 << 10); j ++){
            // 3.2.1 着法初始化
            ROOK_CANNON_CAN_GET_COL[i][j].connon_capture[0] = ROOK_CANNON_CAN_GET_COL[i][j].connon_capture[1] = i + 3;
            ROOK_CANNON_CAN_GET_COL[i][j].no_capture[0] = ROOK_CANNON_CAN_GET_COL[i][j].no_capture[1] = i + 3;
            ROOK_CANNON_CAN_GET_COL[i][j].rook_capture[0] = ROOK_CANNON_CAN_GET_COL[i][j].rook_capture[1] = i + 3;
            ROOK_CANNON_CAN_GET_COL_MASK[i][j].cannon_capture = ROOK_CANNON_CAN_GET_COL_MASK[i][j].no_capture = ROOK_CANNON_CAN_GET_COL_MASK[i][j].rook_capture = 0;
            // 3.2.2 生成车炮向下的着法
            int row;
            for(row = i + 1; row < 10; row ++){
                // 当前状态下该行无棋子
                if((j & (1 << ColBitOpration(row))) == 0){
                    ROOK_CANNON_CAN_GET_COL[i][j].no_capture[0] = row + 3;
                    ROOK_CANNON_CAN_GET_COL_MASK[i][j].no_capture |= (1 << ColBitOpration(row));
                }
                // 当前状态下该行有棋子
                else{
                    ROOK_CANNON_CAN_GET_COL[i][j].rook_capture[0] = row + 3;
                    ROOK_CANNON_CAN_GET_COL_MASK[i][j].rook_capture |= (1 << ColBitOpration(row));
                    break;
                }
            }
            for(row ++; row < 10; row ++){
                // 隔了一子，生成炮吃法
                if((j & (1 << ColBitOpration(row))) != 0){
                    ROOK_CANNON_CAN_GET_COL[i][j].connon_capture[0] = row + 3;
                    ROOK_CANNON_CAN_GET_COL_MASK[i][j].cannon_capture |= (1 << ColBitOpration(row));
                    break;
                }
            }
            // 3.2.3 生成车炮向上的着法
            for(row = i - 1; row >= 0; row --){
                // 当前状态下该行无棋子
                if((j & (1 << ColBitOpration(row))) == 0){
                    ROOK_CANNON_CAN_GET_COL[i][j].no_capture[1] = row + 3;
                    ROOK_CANNON_CAN_GET_COL_MASK[i][j].no_capture |= (1 << ColBitOpration(row));
                }
                // 当前状态下该行有棋子
                else{
                    ROOK_CANNON_CAN_GET_COL[i][j].rook_capture[1] = row + 3;
                    ROOK_CANNON_CAN_GET_COL_MASK[i][j].rook_capture |= (1 << ColBitOpration(row));
                    break;
                }
            }
            for(row --; row >= 0; row --){
                // 隔了一子，生成炮吃法
                if((j & (1 << ColBitOpration(row))) != 0){
                    ROOK_CANNON_CAN_GET_COL[i][j].connon_capture[1] = row + 3;
                    ROOK_CANNON_CAN_GET_COL_MASK[i][j].cannon_capture |= (1 << ColBitOpration(row));
                    break;
                }
            }
        }
    }  
}