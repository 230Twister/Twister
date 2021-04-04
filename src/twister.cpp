#include <iostream>
#include "ucci.h"
#include "hash_table.h"

int main() {
    UcciCommStruct UcciComm;
	if (BootLine() != UCCI_COMM_UCCI) {
		return 0;
	}
    std::cout << "ucciok\n";
    InitHashTable();

    std::cout << "Hello, world!\n";
}