/* 
 * 文件名： moves_getter.cpp
 * 描述	：实现所有着法的生成器
 * 作者 : yao
 * 最后更新时间: 21.02.22 
 */
#include "../include/representation.h"

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
void GetAllMovements(const Situation & situation, int & num_of_all_movements, Movement* all_movements){
    int player_flag = situation.current_player << 4 + 16;
    for(int i = player_flag; i < player_flag + 16; i ++){
        
    }
}