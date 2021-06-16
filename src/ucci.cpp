/* 
 * 文件名： ucci.cpp
 * 描述	： 引擎与界面的通信
 * 最后更新时间: 21.04.24
 */

#include "define.h"
#include "ucci.h"
#include <windows.h>
inline void Idle(void) {
    Sleep(1);
}

CommDetail Command;

// 检查输入是否符合指令格式
int CheckInput(int &BytesLeft) {
    static int s_Init = false;
    static BOOL s_ConsoleMode;
    static HANDLE s_InputHandle;
    DWORD data;
    if (BytesLeft > 0) {
        return true;
    }
    if (!s_Init) {
        s_Init = true;
        s_InputHandle = GetStdHandle(STD_INPUT_HANDLE);
        s_ConsoleMode = GetConsoleMode(s_InputHandle, &data);
        if (s_ConsoleMode) {
            SetConsoleMode(s_InputHandle, data & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(s_InputHandle);
        }
    }
    if (s_ConsoleMode) {
        GetNumberOfConsoleInputEvents(s_InputHandle, &data);
        return data > 1;
    } else {
        if (!PeekNamedPipe(s_InputHandle, NULL, 0, NULL, &data, NULL)) {
            return true;
        }
        BytesLeft = data;
        return data != 0;
    }
}

// 读取一行界面发来的指令
char *ReadInput(void){
    const int c_MaxInputBuff = 2048;
    static char s_LineStr[c_MaxInputBuff];
    static int s_BytesLeft = 0;
    char *RetVal;
    if ( CheckInput(s_BytesLeft) ) {
        RetVal = fgets ( s_LineStr, c_MaxInputBuff, stdin );
        if ( RetVal != NULL ) {
            if  (s_BytesLeft > 0 ) {
                s_BytesLeft -= (int) strlen ( RetVal );
            }
            RetVal = strchr ( s_LineStr, '\n' );
            *RetVal = '\0';
            RetVal = s_LineStr;
        }
        return RetVal;
    } else {
        return NULL;
    }
}

// 读取某串字符中的数字，同时移动字符串的指针
int ReadDigit(const char *&LineStr, int MaxValue) {
    int RetValue;
    RetValue = 0;
    while (1) {
        if (*LineStr >= '0' && *LineStr <= '9') {
            RetValue *= 10;
            RetValue += *LineStr - '0';
            LineStr ++;
            if (RetValue > MaxValue) {
                RetValue = MaxValue;
            }
        } else {
            break;
        }
    }
    return RetValue;
}

// 三个UCCI指令解释器
CommEnum BootLine(void) {
    const char *LineStr;
    LineStr = ReadInput();
    while (LineStr == NULL) {
        Idle();
        LineStr = ReadInput();
    }
    if (strcmp(LineStr, "ucci") == 0) {
        return CommUcci;
    } else {
        return CommNone;
    }
}

CommEnum IdleLine(CommDetail &Command, int  Debug) {
    const char *LineStr;
    CommEnum RetValue;
    LineStr = ReadInput();
    while (LineStr == NULL) {
        Idle();
        LineStr = ReadInput();
    }
    if (Debug) {
        printf("info string %s\n", LineStr);
        fflush(stdout);
    }
    // "IdleLine()"是最复杂的UCCI指令解释器，大多数的UCCI指令都由它来解释，包括：

    // "isready"指令
    if (strcmp(LineStr, "isready") == 0) {
        return CommIsReady;

        // "position {<special_position> | fen <fen_string>} [moves <move_list>]"指令
    } else if (strncmp(LineStr, "position ", 9) == 0) {
        LineStr += 9;
        // 首先判断是否是特殊局面(这里规定了5种)，是特殊局面就直接转换成对应的FEN串
        if (strncmp(LineStr, "startpos", 8) == 0) {
            Command.Position.FenStr = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1";
        } else {
            return CommNone;
        }
        // 寻找是否指定了后续着法，即是否有"moves"关键字
        while (*LineStr != '\0') {
            if (strncmp(LineStr, " moves ", 7) == 0) {
                LineStr += 7;
                Command.Position.MoveNum = int((strlen(LineStr) + 1) / 5); // 提示："moves"后面的每个着法都是4个字符和1个空格
                Command.Position.MoveStr = LineStr;
                return CommPosition;
            }
            LineStr ++;
        }
        Command.Position.MoveNum = 0;
        Command.Position.MoveStr = NULL;
        return CommPosition;
    } 
    // "go [ponder] {infinite | depth <depth> | time <time> [movestogo <moves_to_go> | increment <inc_time>]}"指令
    else if (strncmp(LineStr, "go ", 3) == 0) {
        LineStr += 3;
        // 首先判断到底是"go"还是"go ponder"，因为两者解释成不同的指令
        if (strncmp(LineStr, "ponder ", 7) == 0) {
            LineStr += 7;
            RetValue = CommGoPonder;
        } else {
            RetValue = CommGo;
        }
        // 然后判断是固定深度还是设定时限
        if (strncmp(LineStr, "time ", 5) == 0) {
            LineStr += 5;
            Command.Search.DepthTime.Time = ReadDigit(LineStr, 36000);
            // 如果是设定时限，就要判断是时段制还是加时制
            if (strncmp(LineStr, " movestogo ", 11) == 0) {
                LineStr += 11;
                Command.Search.Mode = TimeMove;
                Command.Search.TimeMode.MovesToGo = ReadDigit(LineStr, 100);
                if (Command.Search.TimeMode.MovesToGo < 1) {
                    Command.Search.TimeMode.MovesToGo = 1;
                }
            } else if (strncmp(LineStr, " increment ", 11) == 0) {
                LineStr += 11;
                Command.Search.Mode = TimeInc;
                Command.Search.TimeMode.Increment = ReadDigit(LineStr, 600);
            } else {
                Command.Search.Mode = TimeMove;
                Command.Search.TimeMode.MovesToGo = 1;
            }
        } else if (strncmp(LineStr, "depth ", 6) == 0) {
            LineStr += 6;
            Command.Search.Mode = TimeDepth;
            Command.Search.DepthTime.Depth = ReadDigit(LineStr, c_MaxDepth - 1);
        } else {
            Command.Search.Mode = TimeDepth;
            Command.Search.DepthTime.Depth = c_MaxDepth - 1;
        }
        return RetValue;
        // "stop"指令
    } else if (strcmp(LineStr, "stop") == 0) {
        return CommStop;

        // "quit"指令
    } else if (strcmp(LineStr, "quit") == 0) {
        return CommQuit;
    } 
    // 无法识别的指令
    else {
        return CommNone;
    }
}

CommEnum BusyLine(int  Debug) {
    const char *LineStr;
    LineStr = ReadInput();
    if (LineStr == NULL) {
        return CommNone;
    } else {
        if (Debug) {
            printf("info string %s\n", LineStr);
            fflush(stdout);
        }
        if (strcmp(LineStr, "isready") == 0) {
            return CommIsReady;
        } else if (strcmp(LineStr, "stop") == 0) {
            return CommStop;
        } else {
            return CommNone;
        }
    }
}