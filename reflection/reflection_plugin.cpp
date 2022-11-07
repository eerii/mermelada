#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;

namespace 
{
    struct SomeConsumer : public ASTConsumer {
        void HandleTranslationUnit(ASTContext &) override {
            llvm::errs() << "hiii\n";
        }
    };

    struct SomePlugin : public PluginASTAction {
        std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &instance, llvm::StringRef file) override {
            return std::make_unique<SomeConsumer>();
        }

        bool ParseArgs(const CompilerInstance &instance, const std::vector<std::string> &args) override {
            return true;
        }

        PluginASTAction::ActionType getActionType() override {
            return AddAfterMainAction;
        }
    };
}

static FrontendPluginRegistry::Add<SomePlugin> Register("something", "");