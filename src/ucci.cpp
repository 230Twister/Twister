#include "ucci.h"
#include <cstring>
#include <cctype>
#include <cmath>

const int LINE_INPUT_MAX_CHAR = 8192;
const int MAX_MOVE_NUM = 1024;
char szFen[LINE_INPUT_MAX_CHAR];
UINT32 dwCoordList[MAX_MOVE_NUM];

bool ParsePos(UcciCommStruct& UcciComm, char* lp) {
    int i;
    //输入一个字符串lp，判断是否指定了FEN串
    //StrEqvSkip比较lp和fen，比较时自动忽略大小写，
    //若s1和s2相同则返回1，且lp指针后移strlen("fen")的长度，，否则返回0
    if (strEqualSkip(lp, "fen ")) {
        strcpy(szFen, lp);
        UcciComm.szFenStr = szFen;//szFenStr为FEN串
     // 然后判断是否是startpos
     // 比较lp和startpos，比较时自动忽略大小写，
     // 若s1和s2相同则返回1，且lp指针后移strlen("fen")的长度，，否则返回0
     // 简单来说StrEqvSkip识别一个字符串然后指针移到这个字符串后面
    }
    else if (strEqual(lp, "startpos")) {
        UcciComm.szFenStr = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";
    }
    else {
        // 如果两者都不是，就立即返回
        return false;
    }
    // 然后寻找是否指定了后续着法，即是否有"moves"关键字
    UcciComm.nMoveNum = 0; //nMoveNum为后续着法数目
    if (strEqualSkip(lp, " moves ")) {
        //略过"moves"
        *(lp - strlen(" moves ")) = '\0';
        UcciComm.nMoveNum = std::min((int)(strlen(lp) + 1) / 5, MAX_MOVE_NUM);   // "moves"后面的每个着法都是1个空格和4个字符
        for (i = 0; i < UcciComm.nMoveNum; i++) {
            dwCoordList[i] = *(UINT32*)lp;  // 4个字符可转换为一个"UINT32"，存储和处理起来方便
            lp += sizeof(UINT32) + 1;       // lp后移四个字符和一个空格
        }
        UcciComm.lpdwMovesCoord = dwCoordList;
    }
    return true;
}

UcciCommEnum BootLine(void) {
    //该函数用来接受第一条指令，如果是ucci，则返回UCCI_COMM_UCCI
    char szLineStr[LINE_INPUT_MAX_CHAR];
    while (!std::cin.getline(szLineStr, LINE_INPUT_MAX_CHAR));
    if (strEqual(szLineStr, "ucci")) {
        return UCCI_COMM_UCCI;
    }
    else {
        return UCCI_COMM_UNKNOWN;
    }
    //UCCI_COMM_UCCI和UCCI_COMM_UNKNOWN都是ucci指令类型
}

UcciCommEnum IdleLine(UcciCommStruct& UcciComm, bool bDebug) {
    //如果bDebug指令为1，输出当前读到的指令
    char szLineStr[LINE_INPUT_MAX_CHAR];
    char* lp;
    bool bGoTime;

    while (!std::cin.getline(szLineStr, LINE_INPUT_MAX_CHAR));
    lp = szLineStr;
    if (bDebug) {
        printf("info idleline [%s]\n", lp);
        fflush(stdout);//清空输入缓冲区
    }

    // 1. "isready"指令
    if (strEqual(lp, "isready")) {
        return UCCI_COMM_ISREADY;
    }
    // 2. "position {<special_position> | fen <fen_string>} [moves <move_list>]"指令
    //如果读到position指令，调用ParsePos指令，着法列表保存在UcciComm里
    else if (strEqualSkip(lp, "position ")) {
        return ParsePos(UcciComm, lp) ? UCCI_COMM_POSITION : UCCI_COMM_UNKNOWN;

    }
    // 3. "go [ponder | draw] <mode>"指令
    else if (strEqualSkip(lp, "go time ")) {
        bGoTime = true;
        UcciComm.nTime = Str2Digit(lp, 0, 2000000000);
        return UCCI_COMM_GO;
    }
    // 4. "quit"指令
    else if (strEqual(lp, "quit")) {
        return UCCI_COMM_QUIT;
    }
    // 5. 无法识别的指令
    else {
        return UCCI_COMM_UNKNOWN;
    }
}

bool strEqual(const char* str1, const char* str2){
    int len = strlen(str2);
    for(int i = 0; i < len; i++){
        if(tolower(*(str1 + i)) != tolower(*(str2 + i)))
            return false;
    }
    return true;
}

bool strEqualSkip(char*& str1, const char* str2){
    if(strEqual(str1, str2)){
        str1 += strlen(str2);
        return true;
    }
    return false;
}

int Str2Digit(const char* sz, int nMin, int nMax) {
	int nRet;
	if (sscanf(sz, "%d", &nRet) > 0) {
		return std::min(std::max(nRet, nMin), nMax);
	}
	else {
		return nMin;
	}
}