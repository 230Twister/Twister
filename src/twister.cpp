#include <iostream>
#include "ucci.h"
#include "hash_table.h"
#include "search.h"
#include "preset.h"
int main ( int argc, char *argv[] ) {
	if ( BootLine() == e_CommUcci ) {
		printf( "id name twister\n" );
		printf( "id version 0.1\n" );
		printf( "id author twister230\n" );
		printf( "ucciok\n" );
		fflush( stdout );
        Situation situation;
        InitHashTable();
        InitPresetArray();
		while ( true ) {
			CommEnum Order = IdleLine ( Command, 0 );
			if ( Order == e_CommAnnotation ) {
				printf( "%s\n", Command.Annotation.String );
				fflush( stdout );
			}
			else if ( Order == e_CommQuit ) { // quit
				printf( "bye\n" );
				fflush( stdout );
				break;
			}
			else if ( Order == e_CommIsReady ) { // isready
				printf( "readyok\n" );
				fflush( stdout );
			}
			else if ( Order == e_CommSetOption ) {
				if( Command.Option.Type == e_NewGame ) { // newgame
                    ResetZobristKey();
				}
			}
			else if ( Order == e_CommPosition ) { // position [ startpos | fen ] moves ...
				ResetZobristKey();
                InitSituation(situation);
                // 输出日志文件
                std::ofstream f;
                f.open("debug.log", std::ios::app | std::ios::out);
                f << "界面传给引擎的ucci指令: " << Command.Position.FenStr << "||" << Command.Position.MoveNum << " " << Command.Position.MoveStr << std::endl;
                FenToSituation(situation, Command.Position.FenStr, Command.Position.MoveNum, Command.Position.MoveStr);
                char fen[100];
                SituationToFen(situation, fen);
                f << "引擎处理完的当前局面的FEN: " << fen << std::endl;
                f << "=======================================\n";
                f.close();

			}
			else if ( Order == e_CommGo || Order == e_CommGoPonder ) { // go ...
				ComputerThink(situation);
			}
		}
	}
	return 0;
}