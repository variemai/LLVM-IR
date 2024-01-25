#include "llvm/IR/PassManager.h"
#include "function_pass.h"

namespace llvm {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM){
        errs() << F.getName() << "\n";
        return PreservedAnalyses::all();
    }

};

