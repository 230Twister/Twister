#include "define.h"

#ifndef UCCI_H
#define UCCI_H

// UCCI指令类型
enum UcciCommEnum {
	UCCI_COMM_UNKNOWN, UCCI_COMM_UCCI, UCCI_COMM_ISREADY,
	UCCI_COMM_POSITION, UCCI_COMM_GO, UCCI_COMM_QUIT
};

// UCCI指令可以解释成以下这个抽象的结构
union UcciCommStruct {
	/* 1. "position"指令传递的信息，适合于"e_CommPosition"指令类型
	 *    "position"指令用来设置局面，包括初始局面连同后续着法构成的局面
	 *    例如，position startpos moves h2e2 h9g8，FEN串就是"startpos"代表的FEN串，着法数(MoveNum)就是2
	 */
	struct {
		const char* szFenStr;       // FEN串，特殊局面(如"startpos"等)也由解释器最终转换成FEN串
		int nMoveNum;               // 后续着法数
		UINT32* lpdwMovesCoord;     // 后续着法，指向程序"IdleLine()"中的一个静态数组，但可以把"CoordList"本身看成数组
	};
	/* 2. "go"指令传递的信息，适合于"UCCI_COMM_GO指令类型 */
	struct {
		int nTime;
	};
};

UcciCommEnum BootLine(void);                       	// UCCI引擎启动的第一条指令，只接收"ucci"
UcciCommEnum IdleLine(UcciCommStruct& UcciComm); 	// 引擎空闲时接收指令

bool strEqual(const char* str1, const char* str2);
bool strEqualSkip(char*& str1, const char* str2);
int Str2Digit(const char* sz, int nMin, int nMax);

#endif