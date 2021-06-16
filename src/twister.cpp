#include <iostream>
#include <time.h>
#include "ucci.h"
#include "hash_table.h"
#include "search.h"
#include "preset.h"
#include "value.h"
#include "book.h"

int main(int argc, char *argv[]) {
	if (BootLine() == CommUcci) {
		printf("ucciok\n");
		fflush(stdout);
        Situation situation;
        InitHashTable();
        LoadBookHashTable();
        InitPresetArray();
		while (true) {
			CommEnum Order = IdleLine (Command, 0);
			if (Order == CommAnnotation) {
				printf("%s\n", Command.Annotation.String );
				fflush(stdout);
			}
			else if (Order == CommQuit) {
				printf("bye\n");
				fflush(stdout);
				DeleteHashTable();
                DeleteBook();
				break;
			}
			else if (Order == CommIsReady) { 
				printf("readyok\n");
				fflush(stdout );
			}
			else if (Order == CommSetOption) {
				if(Command.Option.Type == NewGame) { 
                    ResetZobristKey();
				}
			}
			else if (Order == CommPosition) { 
				ResetZobristKey();
                InitSituation(situation);
                FenToSituation(situation, Command.Position.FenStr, Command.Position.MoveNum, Command.Position.MoveStr);
				// 预评估
				PreEvaluate(situation);
			}
			else if (Order == CommGo || Order == CommGoPonder) {
				ComputerThink(situation);
			}
		}
	}
	return 0;
}