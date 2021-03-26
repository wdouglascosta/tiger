#ifndef CODEGENCONTEXT_H
#define CODEGENCONTEXT_H
#include <AST/ast.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include "utils/symboltable.h"

#include <set>

namespace AST {
class Type;
class VarDec;
class FunctionDec;
}  // namespace AST

class CodeGenContext {
 public:
  bool hasError{false};
  llvm::LLVMContext context;
  llvm::IRBuilder<> builder{context};
  std::unique_ptr<llvm::Module> module{
      llvm::make_unique<llvm::Module>("main", context)};
  SymbolTable<AST::VarDec> valueDecs;
  SymbolTable<llvm::Type> types;
  SymbolTable<AST::Type> typeDecs;
  SymbolTable<llvm::Function> functions;
  SymbolTable<AST::FunctionDec> functionDecs;
  // TODO
  // SymbolTable<std::string> externalFunctions;
  std::deque<llvm::StructType *> staticLink;
  llvm::AllocaInst *currentFrame;
  size_t currentLevel = 0;

  llvm::Type *intType{llvm::Type::getInt64Ty(context)};
  llvm::Type *voidType{llvm::Type::getVoidTy(context)};
  llvm::Type *stringType{
      llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(context))};
  llvm::PointerType *nilType{
      llvm::PointerType::getUnqual(llvm::StructType::get(context))};
  // llvm::Type *stringType{llvm::Type::getInt64Ty(context)};
  llvm::Function *allocaArrayFunction{createIntrinsicFunction(
      "allocaArray",
      {llvm::Type::getInt64Ty(context), llvm::Type::getInt64Ty(context)},
      llvm::Type::getInt8PtrTy(context))};
  llvm::Function *allocaRecordFunction = {
      createIntrinsicFunction("allocaRecord", {llvm::Type::getInt64Ty(context)},
                              llvm::Type::getInt8PtrTy(context))};
  llvm::Function *strCmpFunction = {
      createIntrinsicFunction("strcmp_", {stringType, stringType}, intType)};
  std::stack<
      std::tuple<llvm::BasicBlock * /*next*/, llvm::BasicBlock * /*after*/>>
      loopStack;
  llvm::Value *zero{llvm::ConstantInt::get(intType, llvm::APInt(64, 0))};
  llvm::Value *one{llvm::ConstantInt::get(intType, llvm::APInt(64, 1))};

  llvm::Value *logErrorV(std::string const &msg);

  llvm::AllocaInst *createEntryBlockAlloca(llvm::Function *function,
                                           llvm::Type *type,
                                           const std::string &name,
                                           llvm::Value *size = nullptr);

  llvm::Type *getElementType(llvm::Type *type);

  bool isNil(llvm::Type *exp);
  bool isRecord(llvm::Type *exp);
  bool isMatch(llvm::Type *a, llvm::Type *b);
  llvm::Value *checkStore(llvm::Value *val, llvm::Value *ptr);
  llvm::Value *convertNil(llvm::Value *val, llvm::Value *other);
  llvm::Function *createIntrinsicFunction(std::string const &name,
                                          std::vector<llvm::Type *> const &args,
                                          llvm::Type *retType);
  llvm::Value *strcmp(llvm::Value *a, llvm::Value *b);
  void intrinsic();
  llvm::Type *logErrorT(std::string const &msg);

  llvm::Type *typeOf(std::string const &name,
                     std::set<std::string> &parentName);
  llvm::Type *typeOf(const std::string &name);
  CodeGenContext();
};

#endif  // CODEGENCONTEXT_H
