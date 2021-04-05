#include <iostream>
#include "ucci.h"
#include "hash_table.h"
#include "search.h"
#include "preset.h"

void PrintLn(const char* str);

int main() {
    UcciCommStruct UcciComm;
    Situation situation;
	if (BootLine() != UCCI_COMM_UCCI) {
		return 0;
	}
    InitHashTable();
    InitPresetArray();
	PrintLn("ucciok");

    bool quit = false;
	while (!quit) {
		switch (IdleLine(UcciComm)) {
			case UCCI_COMM_ISREADY:
				PrintLn("readyok");
				break;
			case UCCI_COMM_POSITION:
                ResetZobristKey();
				// 构造局面
                InitSituation(situation);
                FenToSituation(situation, UcciComm.szFenStr);
                PrintLn(UcciComm.szFenStr);
				break;
			case UCCI_COMM_GO:
				ComputerThink(situation);
				break;
			case UCCI_COMM_QUIT:
				quit = true;
				break;
			default:
				break;
		}
	}
	PrintLn("bye");
}

void PrintLn(const char* str){
    printf(str);
    printf("\n");
    fflush(stdout);
}