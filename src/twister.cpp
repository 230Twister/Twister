#include <iostream>
#include "ucci.h"

int main() {
    UcciCommStruct UcciComm;
	if (BootLine() != UCCI_COMM_UCCI) {
		return 0;
	}
    std::cout << "ucciok\n";
    std::cout << "Hello, world!\n";
}