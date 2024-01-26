#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct OpsPass : public PassInfoMixin<OpsPass> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        errs() << "I saw a module " << M.getName() << "!\n";
        for (auto &F : M.functions()) {
            errs() << "I saw a function called: " << F.getName() << "!\n";
            for (auto &B : F) {
                errs() << "I saw a basic block of size: " << B.size() << "\n";
                for ( auto &I : B) {
                    errs() << "I saw an instruction with opcode: " << I.getOpcode() << "\n";
                }
            }
        }
        return PreservedAnalyses::all();
    };
};

}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        .APIVersion = LLVM_PLUGIN_API_VERSION,
        .PluginName = "Ops Pass",
        .PluginVersion = "v0.1",
        .RegisterPassBuilderCallbacks = [](PassBuilder &PB) {
            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level) {
                    MPM.addPass(OpsPass());
                });
        }
    };
}
