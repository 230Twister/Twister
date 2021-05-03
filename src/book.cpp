/* 
 * 文件名： book.cpp
 * 描述	：开局库
 * 作者 : saltgrey
 * 最后更新时间: 21.05.03 
 */
#include <representation.h>
#include "book.h"
#include "hash_table.h"
#include <string>
#include <fstream>
#include <stdlib.h>

const UINT32 BOOK_HASHTABLE_SIZE = 1 << 20;         // 哈希表大小 1MB
BookHashNode* BookHashTable;                        // 哈希表
UINT64 BookZobristKey;                              // 当前局面键值
UINT64 BookZobristKeyCheck;                         // 当前局面校验值

/* 
 * 函数名：InitBookZobrist
 * 描述：初始化相关键值
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.05.03
 */
void InitBookZobrist(){
    BookZobristKey = ZobristKey;
    BookZobristKeyCheck = ZobristKeyCheck;
}


/* 
 * 函数名：LoadBookHashTable
 * 描述：逐行存入哈希表
 * 入参: 
 * - void
 * 返回值：
 * - void
 * 最后更新时间: 21.05.03
 */
void LoadBookHashTable(){
    BookHashTable = new BookHashNode[BOOK_HASHTABLE_SIZE];
    memset(BookHashTable, 0, BOOK_HASHTABLE_SIZE * sizeof(BookHashNode));

    ifstream f("book.txt");
    if(!f.is_open())
        cout << "未能打开book.txt" << endl;
    string line;
    while(getline(f, line)){
        InitBookZobrist();
        char* temp = new char[strlen(line.c_str()) + 1];
		strcpy(temp, line.c_str());
		string move = strtok(temp, " ");
		string s = strtok(NULL, " ");
		line = strtok(NULL, " ");
		string side = strtok(NULL, " ");
		line = line + " " + side;       // line存放FEN串
        delete temp;
        char* fen = new char[strlen(line.c_str()) + 1];
        char* score = new char[strlen(s.c_str()) + 1];
        FENToHash(fen, move, score);                 // 将对应局面存入哈希表
        delete fen;
    }
}


/* 
 * 函数名：FENToHash
 * 描述：根据fen串存入哈希表
 * 入参: 
 * - const char* fen      fen串
 * - string move          对应着法
 * - const char* score    对应分数
 * 返回值：
 * - void
 * 最后更新时间: 21.05.03
 */
void FENToHash(const char* fen, string move, const char* score){
    const char *p = fen;
    int piece_id_array[14] = {16, 17, 19, 21, 23, 25, 27, 32, 33, 35, 37, 39, 41, 43};
    int col = GetCol(BOARD_FIRST_POSITION), row = GetRow(BOARD_FIRST_POSITION), piece_id_index, piece_id;

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
            BookZobristKey ^= ZobristTable[piece_id][GetPosition(col, row)];
            BookZobristKeyCheck ^= ZobristTableCheck[piece_id][GetPosition(col, row)];

            piece_id_array[piece_id_index] ++;
            col ++;
        }
        p ++;
    }
    p ++;   // *p = side
    if(*p == 'w'){
        BookZobristKey ^= ZobristPlayer;
        BookZobristKeyCheck ^= ZobristPlayerCheck;
    }

    UINT32 place;
    place = BookZobristKey % BOOK_HASHTABLE_SIZE;
    while(BookHashTable[place].check && BookHashTable[place].check != BookZobristKeyCheck)  // 存着不同的局面，遇到冲突，线性探测再散列
        place ++;

    if(!BookHashTable[place].check){                            // 位置为空，存入哈希表
        BookHashTable[place].check = BookZobristKeyCheck;
        BookBoardNode *node = new BookBoardNode;
        node->next = NULL;
        BookHashTable[place].next = node;
        node->move = StrToMovement(move);
        int s = atoi(score);
        node->move.value = s;
    }
    else{                                                       // 同一局面，新着法排在后面
        BookBoardNode* p = BookHashTable[place].next;
        while(p->next){
            p = p->next;
        }
        BookBoardNode *node = new BookBoardNode;
        node->next = NULL;
        p->next = node;
        node->move = StrToMovement(move);
        int s = atoi(score);
        node->move.value = s;
    }
}


/* 
 * 函数名：ReadHashTable
 * 描述：读取哈希表
 * 入参: 
 * - Situation situation 当前局面
 * 返回值：
 * - Movement 对应着法
 * 最后更新时间: 21.05.03
 */
Movement ReadHashTable(Situation situation){
    Movement m;
    m.from = 0;
    m.to = 0;
    m.capture = 0;
    m.value = 0;
    UINT32 place = ZobristKey % BOOK_HASHTABLE_SIZE;
    while(BookHashTable[place].check && BookHashTable[place].check != ZobristKeyCheck)  // 冲突
        place ++;
    if(BookHashTable[place].check){
        m = BookHashTable[place].next->move;
        if(situation.current_board[m.to])
            m.capture = 1;
        else
            m.capture = 0;
    }
    return m;
}