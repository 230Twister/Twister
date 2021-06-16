/* 
 * 文件名： ucci.h
 * 描述	： 引擎与界面的通信
 * 最后更新时间: 21.04.24
 */

#ifndef UCCI_H
#define UCCI_H

const int c_MaxDepth = 32;

// 和UCCI指令中关键字有关的选项
enum OptionEnum{
    OptionNone,
    OptionBatch,
    OptionDebug,
    OptionBookFiles,
    OptionEgtbPaths,
    OptionHashSize,
    OptionThreads,
    OptionDrawMoves,
    OptionRepetition,
    OptionPruning,
    OptionKnowledge,
    OptionSelectivity,
    OptionStyle,
    OptionLoadBook,
    NewGame
};
enum CheckEnum{
    CheckFalse,
    CheckTrue
};
enum RepetitionEnum{
    RepetitionAlwaysDraw,
    RepetitionCheckBan,
    RepetitionAsianRule,
    RepetitionChineseRule
};
enum ScaleEnum{
    ScaleNone,
    ScaleSmall,
    ScaleMedium,
    ScaleLarge
};
enum StyleEnum{
    StyleSolid,
    StyleNormal,
    StyleRisky
};
enum TimeEnum{
    TimeDepth,
    TimeMove,
    TimeInc
};
enum CommEnum{
    CommNone,
    CommPonderHit,
    CommStop,
    CommUcci,
    CommIsReady,
    CommSetOption,
    CommPosition,
    CommBanMoves,
    CommGo,
    CommGoPonder,
    CommQuit,
    CommAnnotation
}; // UCCI指令类型

// UCCI指令可以解释成以下这个抽象的结构
union CommDetail{

    struct{
        OptionEnum Type; // 选项类型
        union{ // 选项值
            int Spin;
            CheckEnum Check;
            RepetitionEnum Repetition;
            ScaleEnum Scale;
            StyleEnum Style;
            const char *String;
        } Value;
    } Option;

    struct{
        const char *FenStr;  // FEN串，特殊局面(如"startpos"等)也由解释器最终转换成FEN串
        int MoveNum;         // 后续着法数
        const char *MoveStr; // 后续着法字符串
    } Position;

    struct{
        int MoveNum;
        const char *MoveStr;
    } BanMoves;

    struct{
        TimeEnum Mode;
        union{
            int Depth; // 如果是固定深度，则表示深度(层)
            int Time;  // 如果是限定时间，则表示时间(秒)
        } DepthTime;
        union{
            int MovesToGo; // 如果是加时制，则限定时间内要走多少步棋
            int Increment; // 如果是时段制，则表示走完该步后限定时间加多少秒
        } TimeMode;
    } Search;

    struct{
        const char *String;
    } Annotation;
};

extern CommDetail Command;

// 下面两个函数可被UCCI指令解释器和浅红象棋协议解释器调用
char *ReadInput(void);                             // 读取一行
int ReadDigit(const char *&LineStr, int MaxValue); // 读取某串字符中的数字

// 下面三个函数用来解释UCCI指令
CommEnum BootLine(void);                           // UCCI引擎启动的第一条指令，只接收"ucci"
CommEnum IdleLine(CommDetail &Command, int Debug); // 引擎空闲时接收指令
CommEnum BusyLine(int Debug);

int QhInputLine(int Busy);

#endif