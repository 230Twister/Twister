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
const UINT32 BOOK_HASHTABLE_MASK = (1 << 20) - 1;
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

#include "buffer.h"
//获取一个四字节的字符串
#define LOAD_4CHAR(p) (*((unsigned int*) (&buffer_in[p])))

//从源复制一 四字节字符串 到目标位置
#define COPY(d, s) (*((unsigned int*) (&buffer_out[d])) = LOAD_4CHAR(s))

//复制字符串，每次复制四个字符
inline void copyToDestination(char* buffer_out, int& des, unsigned char* buffer_in, int& source, int& length) {
	COPY(des, source);

	for (int i = 4; i < length; i += 4) {
		COPY(des + i, source + i);
	}
}

//lz4解压
void decompress(char* buffer_out) {
	int blockLength = 0;
    int p = 0;                                  //指向当前输出缓冲区的位置
	for(int i = 0; i < 2; i++) {
        int und_p = 4 * (i + 1) + blockLength;  //指向当前待解压区的位置
        blockLength = *((unsigned int*) (&data[4 * i + blockLength]));
		int und_end = und_p + blockLength;	    //待解压区的结束位置

		while (1) {
			int token = data[und_p++];

			//如果token中字面值长度是有的
			if (token >= 16) {
				int literalLength = token >> 4;    //字面值长度

				if (literalLength == 15) {
					//字面值长度大于等于15就接着去扩展区获取
					while (1) {
						int c = data[und_p++];
						literalLength += c;

						if (c != 255)
							break;
					}
				}

				//复制字面的字符
				copyToDestination(buffer_out, p, data, und_p, literalLength);

				//挪动当前位置指向和待压缩区位置指向
				p += literalLength;
				und_p += literalLength;

				//如果复制完字面字符已经到底了就退出
				if (und_p >= und_end)
					break;
			}

			//获取匹配字符串在的位置
			int ago_p = p - (*((unsigned short*)(&data[und_p])));
			und_p += 2;

			//获取token里的匹配长度
			int matchLength = (token & 15) + 4;

			if (matchLength == 15 + 4) {
				//如果有扩展长度就获取扩展的匹配长度
				while (1) {
					int c = data[und_p++];
					matchLength += c;

					if (c != 255)
						break;
				}
			}

			//偏移大于等于4可以大胆整块复制
			if (p - ago_p >= 4) {
				copyToDestination(buffer_out, p, (unsigned char*) buffer_out, ago_p, matchLength);
				p += matchLength;
			}
			else {
				//否则一个个字符复制
				while (matchLength-- != 0)
					buffer_out[p++] = buffer_out[ago_p++];
			}
		}
	}
}

bool getLine(char* buff, int& buff_p, string& line){
    line = "";
    if(buff[buff_p] == 0) return false;
    while(buff[buff_p] != '\n'){
        line += buff[buff_p++];
    }
    buff_p++;
    return true;
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

    char* buff = new char[1<<24];
    memset(buff, 0, (1<<24) * sizeof(char));
    int buff_p = 0;
    decompress(buff);

    string line;
    while(getLine(buff, buff_p, line)){
        InitBookZobrist();
        char* temp = new char[line.size() + 1];
		strcpy(temp, line.c_str());
        string move_s = strtok(temp, ":");

		line = strtok(NULL, " ");
		string side = strtok(NULL, " ");
		line = line + " " + side;       // line存放FEN串
        delete temp;
        FENToHash(line.c_str(), move_s);                 // 将对应局面存入哈希表
    }
    delete[] buff;
}


/* 
 * 函数名：FENToHash
 * 描述：根据fen串存入哈希表
 * 入参: 
 * - const char* fen      fen串
 * - string move_s        对应着法和分数
 * 返回值：
 * - void
 * 最后更新时间: 21.05.03
 */
void FENToHash(const char* fen, string move_s){
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
    if(*p != 'b'){
        BookZobristKey ^= ZobristPlayer;
        BookZobristKeyCheck ^= ZobristPlayerCheck;
    }

    UINT32 place;
    place = BookZobristKey & BOOK_HASHTABLE_MASK;
    while(BookHashTable[place].check && BookHashTable[place].check != BookZobristKeyCheck)  // 存着不同的局面，遇到冲突，线性探测再散列
        place = (place + 1) & BOOK_HASHTABLE_MASK;

    if(!BookHashTable[place].check){                            // 位置为空，存入哈希表
        BookHashTable[place].check = BookZobristKeyCheck;
        BookHashTable[place].move = StrToMovement(move_s.substr(0, 4));
    }
}


/* 
 * 函数名：ReadBookTable
 * 描述：读取哈希表
 * 入参: 
 * - Situation& situation 当前局面
 * 返回值：
 * - Movement 对应着法
 * 最后更新时间: 21.05.03
 */
Movement ReadBookTable(Situation& situation){
    Movement m;
    m.from = 0;
    m.to = 0;
    m.capture = 0;
    m.value = 0;
    UINT32 place = ZobristKey & BOOK_HASHTABLE_MASK;
    while(BookHashTable[place].check && BookHashTable[place].check != ZobristKeyCheck)  // 冲突
        place = (place + 1) & BOOK_HASHTABLE_MASK;
    if(BookHashTable[place].check){
        m = BookHashTable[place].move;
        m.capture = situation.current_board[m.to];
    }
    return m;
}

void DeleteBook(){
    delete[] BookHashTable;
}