#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

namespace {

struct OpsPass : public PassInfoMixin<OpsPass> {

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        unsigned bits = 0;
        Type *type, *valueType;
        Value *bitsValue, *storedValue;
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
            FunctionCallee recordMemBytesOPFunc =
                F.getParent()->getOrInsertFunction("recordMemBytesOP", logFuncType);


            bool isMain = (F.getName() == "main");
            for (auto &B : F) {
                for ( auto &I : B) {
                    IRBuilder<> builder(&I);
                    switch (I.getOpcode()) {
                        case Instruction::Ret:{
                            if ( isMain ){
                                builder.CreateCall(printFunc, {});
                                return PreservedAnalyses::none();
                            }
                            break;
                        }
                        case Instruction::Add:
                            builder.CreateCall(recordIOPFunc, {});
                            break;
                        case Instruction::Sub:
                            builder.CreateCall(recordIOPFunc, {});
                            break;
                        case Instruction::Mul:
                            builder.CreateCall(recordIOPFunc, {});
                            break;
                        case Instruction::UDiv:
                            builder.CreateCall(recordIOPFunc, {});
                            break;
                        case Instruction::SDiv:
                            builder.CreateCall(recordIOPFunc, {});
                            break;
                        case Instruction::FAdd:
                            builder.CreateCall(recordFLOPFunc, {});
                            break;
                        case Instruction::FSub:
                            builder.CreateCall(recordFLOPFunc, {});
                            break;
                        case Instruction::FMul:
                            builder.CreateCall(recordFLOPFunc, {});
                            break;
                        case Instruction::FDiv:
                            builder.CreateCall(recordFLOPFunc, {});
                            break;

                        case Instruction::Store:
                        {
                            auto *storeInst = dyn_cast<StoreInst>(&I);
                            storedValue = storeInst->getValueOperand(); // Get the value stored
                            valueType = storedValue->getType();
                            bits = valueType->getPrimitiveSizeInBits();
                            bitsValue = ConstantInt::get(Type::getInt32Ty(Ctx), bits);
                            builder.CreateCall(recordMemBytesOPFunc, {bitsValue});
                            // type = storeInst->getType();
                            // bits = type->getPrimitiveSizeInBits();
                            // bitsValue = ConstantInt::get(Type::getInt32Ty(Ctx), bits);
                            // builder.CreateCall(recordMemBytesOPFunc,{bitsValue});
                            break;
                        }
                        case Instruction::Load:
                        {
                            auto *loadInst = dyn_cast<LoadInst>(&I);
                            type = loadInst->getType();
                            bits = type->getPrimitiveSizeInBits();
                            bitsValue = ConstantInt::get(Type::getInt32Ty(Ctx), bits);
                            builder.CreateCall(recordMemBytesOPFunc,{bitsValue});
                            break;
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
