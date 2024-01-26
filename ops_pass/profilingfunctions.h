#ifndef LLVM_PROFILINGFUNCTIONS_H
#define LLVM_PROFILINGFUNCTIONS_H

extern "C"{
    extern unsigned long long flopCount;
    extern unsigned long long iopCount;

    void recordFLOP();
    void recordIOP();
    void printOPcounts();
}
#endif //LLVM_PROFILINGFUNCTIONS_H
