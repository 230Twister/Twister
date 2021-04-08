/* 
 * 文件名： hash_table.cpp
 * 描述	：实现置换表
 * 作者 : oscar
 * 最后更新时间: 21.03.24
 */
#include <random>
#include "hash_table.h"
#include <cstring>

const UINT8 hashEXACT = 0;                  // PV结点类型
const UINT8 hashALPHA = 1;                  // ALPHA结点类型
const UINT8 hashBETA = 2;                   // BETA结点类型
const UINT32 HASHTABLE_SIZE = 1 << 24;      // 置换表大小 16MB
const UINT32 HASHTABLE_MASK = (1 << 24) - 1;

const int NONE_VALUE = 1 << 20;             // 空价值
const Movement NONE_MOVE = {0, 0, 0};       // 空着

HashNode* HashTable;                        // 置换表
UINT64 ZobristKey;                          // 当前局面键值
UINT64 ZobristKeyCheck;                     // 当前局面校验值
UINT64 ZobristPlayer;                       // 走棋方的键值
UINT64 ZobristPlayerCheck;                  // 走棋方校验值
UINT64 ZobristTable[48][256];               // 棋子在棋盘各个位置的键值
UINT64 ZobristTableCheck[48][256];          // 棋子在棋盘各个位置的校验值

extern int step;
extern const int MAX_VALUE;
extern const int WIN_VALUE;
/* 
 * 函数名：InitHashTable
 * 描述：生成置换表以及相关键值
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.03.24
 */
void InitHashTable(){
    HashTable = new HashNode[HASHTABLE_SIZE];
    memset(HashTable, 0, HASHTABLE_SIZE * sizeof(HashNode));
    std::independent_bits_engine<std::default_random_engine, 64, UINT64> engine;

    ZobristPlayer = engine();       // 生成走棋方键值
    ZobristPlayerCheck = engine();  // 生成走棋方校验值
    ZobristKey = engine();
    ZobristKeyCheck = engine();
    for(int i = 16; i < 48; i++){
        for(int j = 0; j < 256; j++){
            ZobristTable[i][j] = engine();
        }
    }
    for(int i = 16; i < 48; i++){
        for(int j = 0; j < 256; j++){
            ZobristTableCheck[i][j] = engine();
        }
    }
}

/* 
 * 函数名：ResetHashTable
 * 描述：重置 置换表
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.03.25
 */
void ResetHashTable(){
    memset(HashTable, 0, HASHTABLE_SIZE * sizeof(HashNode));
}

/* 
 * 函数名：DeleteHashTable
 * 描述：删除置换表
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.03.25
 */
void DeleteHashTable(){
    delete[] HashTable;
}

/* 
 * 函数名：ResetZobristKey
 * 描述：重置局面Key
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.04.05
 */
void ResetZobristKey(){
    std::independent_bits_engine<std::default_random_engine, 64, UINT64> engine;
    ZobristKey = engine();
    ZobristKeyCheck = engine();
}

/* 
 * 函数名：ReadHashTable
 * 描述：读取置换表
 * 入参: 
 * - int depth      当前局面深度
 * - int alpha      当前局面alpha值
 * - int beta       当前局面beta值
 * - Movement& move 当前局面最佳走法
 * 返回值：
 * - int 获取到的值
 * 最后更新时间: 21.03.25
 */
int ReadHashTable(int depth, int alpha, int beta, Movement& move){
    int index = ZobristKey & HASHTABLE_MASK;
    HashNode hash_node = HashTable[index];

    // 判断校验值是否正确
    if(ZobristKeyCheck == hash_node.check){
        // 获取到的置换表结点的搜索深度应该较大才能认为是有效的
        if(hash_node.depth > depth){
            // 调整至相对值
            if(hash_node.value > WIN_VALUE){
                hash_node.value -= step;
            }
            if(hash_node.value < -WIN_VALUE){
                hash_node.value += step;
            }

            // PV结点 直接返回置换表中的值
            if(hash_node.type == hashEXACT){
                return hash_node.value;
            }
            // alpha结点
            else if(hash_node.type == hashALPHA && hash_node.value <= alpha){
                return alpha;
            }
            // beta结点
            else if(hash_node.type == hashBETA && hash_node.value >= beta){
                return beta;
            }
            // 读取失败就获得该局面的最佳走法
            move = hash_node.good_move;
        }
    }
    return NONE_VALUE;
}

/* 
 * 函数名：SaveHashTable
 * 描述：读取置换表
 * 入参: 
 * - int depth          当前局面深度
 * - int value          当前局面的估值分数
 * - UINT8 node_type    当前结点类型
 * - Movement move      当前最佳着法
 * 返回值：
 * - void
 * 最后更新时间: 21.03.25
 */
void SaveHashTable(int depth, int value, UINT8 node_type, Movement move){
    int index = ZobristKey & HASHTABLE_MASK;
    HashNode hash_node = HashTable[index];
    HashNode new_hash_node = HashNode{ZobristKeyCheck, depth, value, node_type, move};
    
    // 存入置换表前，将估值调整到绝对值
    if(new_hash_node.value > WIN_VALUE){
        new_hash_node.value += step;
    }
    if(new_hash_node.value < -WIN_VALUE){
        new_hash_node.value -= step;
    }
    // 出现相同局面冲突
    if(hash_node.check == ZobristKeyCheck){
        // 深度优先覆盖
        if(hash_node.depth < depth){
            HashTable[index] = new_hash_node;
        }
    }
    // 不同局面，直接覆盖
    else{
        HashTable[index] = new_hash_node;
    }
}