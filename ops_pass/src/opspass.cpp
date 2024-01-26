#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

namespace {

struct OpsPass : public PassInfoMixin<OpsPass> {
    // Counters for different operations
    unsigned IOPS = 0;
    unsigned FLOPS = 0;

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        for (auto &F : M.functions()) {

            // Get the function to call from our runtime library.
            LLVMContext &Ctx = F.getContext();
            std::vector<Type*> paramTypes = {Type::getInt32Ty(Ctx)};
            Type *retType = Type::getVoidTy(Ctx);
            FunctionType *logFuncType = FunctionType::get(retType, paramTypes, false);
            FunctionCallee printFunc =
                F.getParent()->getOrInsertFunction("printOPcounts", logFuncType);
            FunctionCallee recordFLOPFunc =
                F.getParent()->getOrInsertFunction("recordFLOP", logFuncType);
            FunctionCallee recordIOPFunc =
                F.getParent()->getOrInsertFunction("recordIOP", logFuncType);


            bool isMain = (F.getName() == "main");
            for (auto &B : F) {
                for ( auto &I : B) {
                    IRBuilder<> builder(&I);
                    switch (I.getOpcode()) {
                        case Instruction::Add:
                            builder.CreateCall(recordIOPFunc, {});
                            ++IOPS;
                            break;
                        case Instruction::Sub:
                            builder.CreateCall(recordIOPFunc, {});
                            ++IOPS;
                            break;
                        case Instruction::Mul:
                            builder.CreateCall(recordIOPFunc, {});
                            ++IOPS;
                            break;
                        case Instruction::UDiv:
                            builder.CreateCall(recordIOPFunc, {});
                            ++IOPS;
                            break;
                        case Instruction::SDiv:
                            builder.CreateCall(recordIOPFunc, {});
                            ++IOPS;
                            break;
                        case Instruction::FAdd:
                            builder.CreateCall(recordFLOPFunc, {});
                            ++FLOPS;
                            break;
                        case Instruction::FSub:
                            builder.CreateCall(recordFLOPFunc, {});
                            ++FLOPS;
                            break;
                        case Instruction::FMul:
                            builder.CreateCall(recordFLOPFunc, {});
                            ++FLOPS;
                            break;
                        case Instruction::FDiv:
                            builder.CreateCall(recordFLOPFunc, {});
                            ++FLOPS;
                            break;
                        case Instruction::Ret:
                            if ( isMain ){
                                builder.CreateCall(printFunc, {});
                                return PreservedAnalyses::none();
                            }
                    }
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

              // HANDLE_BINARY_INST(13, Add  , BinaryOperator)
                    // HANDLE_BINARY_INST(14, FAdd , BinaryOperator)
                    // HANDLE_BINARY_INST(15, Sub  , BinaryOperator)
                    // HANDLE_BINARY_INST(16, FSub , BinaryOperator)
                    // HANDLE_BINARY_INST(17, Mul  , BinaryOperator)
                    // HANDLE_BINARY_INST(18, FMul , BinaryOperator)
                    // HANDLE_BINARY_INST(19, UDiv , BinaryOperator)
                    // HANDLE_BINARY_INST(20, SDiv , BinaryOperator)
                    // HANDLE_BINARY_INST(21, FDiv , BinaryOperator)
                    // HANDLE_BINARY_INST(22, URem , BinaryOperator)
                    // HANDLE_BINARY_INST(23, SRem , BinaryOperator)
                    // HANDLE_BINARY_INST(24, FRem , BinaryOperator)


                   // if (I.getOpcode() == Instruction::Add){
                    //     IRBuilder<> builder(&I);
                    //     builder.CreateCall(recordIOPFunc, {});
                    //      ++IOPS;
                    // }
                    // if (I.getOpcode() == Instruction::FAdd){
                    //     IRBuilder<> builder(&I);
                    //     builder.CreateCall(recordFLOPFunc, {});
                    //      ++FLOPS;
                    // }
                    // if (I.getOpcode() == Instruction::Sub){
                    //     IRBuilder<> builder(&I);
                    //     builder.CreateCall(recordIOPFunc, {});
                    //      ++IOPS;
                    // }
                    // if (I.getOpcode() == Instruction::FSub){
                    //     IRBuilder<> builder(&I);
                    //     builder.CreateCall(recordFLOPFunc, {});
                    //      ++FLOPS;
                    // }
                    // if (I.getOpcode() == Instruction::Mul){
                    //     IRBuilder<> builder(&I);
                    //     builder.CreateCall(recordIOPFunc, {});
                    //      ++IOPS;
                    // }
                    // if (I.getOpcode() == Instruction::FMul){
                    //     IRBuilder<> builder(&I);
                    //     builder.CreateCall(recordFLOPFunc, {});
                    //      ++FLOPS;
                    // }
                    // if (I.getOpcode() == Instruction::UDiv){
                    //     IRBuilder<> builder(&I);
                    //     builder.CreateCall(recordIOPFunc, {});
                    //      ++IOPS;
                    // }
                    // if (I.getOpcode() == Instruction::SDiv){
                    //     IRBuilder<> builder(&I);
                    //     builder.CreateCall(recordIOPFunc, {});
                    //      ++IOPS;
                    // }
                    // if (I.getOpcode() == Instruction::FDiv){
                    //     IRBuilder<> builder(&I);
                    //     builder.CreateCall(recordFLOPFunc, {});
                    //      ++FLOPS;
                    // }
