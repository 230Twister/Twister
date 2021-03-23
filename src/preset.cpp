/* 
 * 文件名： preset.cpp
 * 描述	： 产生用于加快着法生成的预置数组
 * 作者 : yao
 * 最后更新时间: 21.02.23
 */
#include "../include/moves_getter.h"

int HORSE_CAN_GET[256][10];             // 马在各个位置所能到的地方
int HORSE_LEG[256][10];                 // 马在各个位置马腿所在的地方
int KING_CAN_GET[256][2][10];           // 将(帅)在各个位置所能到的地方
int ADVISOR_CAN_GET[256][2][10];   	    // 士(仕)在各个位置所能到的地方
int PAWN_CAN_GET[256][2][10];           // 兵(卒)在各个位置所能到的地方
int BISHOP_CAN_GET[256][2][10];     	// 象在各个位置所能到的地方
int BISHOP_EYE[256][2][10];        	    // 象在各个位置象眼所在的地方

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
}