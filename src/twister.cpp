#include <iostream>
#include <time.h>
#include "ucci.h"
#include "hash_table.h"
#include "search.h"
#include "preset.h"
#include "value.h"
#include "book.h"

int main(int argc, char *argv[]) {
	if (BootLine() == e_CommUcci) {
		printf("ucciok\n");
		fflush(stdout);
        Situation situation;
        InitHashTable();
        LoadBookHashTable();
        InitPresetArray();
		while (true) {
			CommEnum Order = IdleLine (Command, 0);
			if (Order == e_CommAnnotation) {
				printf("%s\n", Command.Annotation.String );
				fflush(stdout);
			}
			else if (Order == e_CommQuit) { // quit
				printf("bye\n");
				fflush(stdout);
				DeleteHashTable();
                DeleteBook();
				break;
			}
			else if (Order == e_CommIsReady) { // isready
				printf("readyok\n");
				fflush(stdout );
			}
			else if (Order == e_CommSetOption) {
				if(Command.Option.Type == e_NewGame) { // newgame
                    ResetZobristKey();
				}
			}
			else if (Order == e_CommPosition) { // position [ startpos | fen ] moves ...
				ResetZobristKey();
                InitSituation(situation);
                FenToSituation(situation, Command.Position.FenStr, Command.Position.MoveNum, Command.Position.MoveStr);
				// 预评估
				PreEvaluate(situation);
			}
			else if (Order == e_CommGo || Order == e_CommGoPonder) { // go ...
				ComputerThink(situation);
			}
		}
	}
	return 0;
}