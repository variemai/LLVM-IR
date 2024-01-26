#include "profilingfunctions.h"
#include <iostream>

extern "C" {
    unsigned long long flopCount = 0;
    unsigned long long iopCount = 0;

    void recordFLOP() {
        ++flopCount;
    }

    void recordIOP() {
        ++iopCount;
    }

    void printOPcounts() {
        std::cout << "=============Profiling Data===========\n" << "";
        std::cout << "FLOPS: " << flopCount << "\nIOPS: "  << iopCount << "\n";
        std::cout << "=============Profiling Data END========\n" << "";
    }
}
