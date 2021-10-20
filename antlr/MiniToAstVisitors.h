#pragma once

#include "MiniBaseVisitor.h"

#include "antlr4-common.h"

#include "ast/program.h"
#include "ast/expressions.h"
#include "ast/statements.h"
#include "ast/types.h"

#include "lib/proxy.h"

using mini::proxy;

class MiniToAstTypeVisitor : public MiniBaseVisitor {
protected:
    antlrcpp::Any defaultResult() override {
        return proxy<ast::VoidType, ast::Type>(
            std::make_shared<ast::VoidType>()
        );
    }
public:
    MiniToAstTypeVisitor() {};

    antlrcpp::Any visitIntType(MiniParser::IntTypeContext* ctx) override {
        return proxy<ast::IntType, ast::Type>(
            std::make_shared<ast::IntType>()
        );
    }

    antlrcpp::Any visitBoolType(MiniParser::BoolTypeContext* ctx) override {
        return proxy<ast::BoolType, ast::Type>(
            std::make_shared<ast::BoolType>()
        );
    }

    antlrcpp::Any visitStructType(MiniParser::StructTypeContext* ctx) override {
        return proxy<ast::StructType, ast::Type>(
            std::make_shared<ast::StructType>(ctx->getStart()->getLine(), ctx->ID()->getText())
        );
    }

    antlrcpp::Any visitReturnTypeReal(MiniParser::ReturnTypeRealContext* ctx) override {
        return ctx->type()->accept(this);
    }

    antlrcpp::Any visitReturnTypeVoid(MiniParser::ReturnTypeVoidContext* ctx) override {
        return proxy<ast::VoidType, ast::Type>(
            std::make_shared<ast::VoidType>()
        );
    }
};

class MiniToAstExpressionVisitor : public MiniBaseVisitor {
protected:
    antlrcpp::Any defaultResult() override {
        return proxy<ast::NullExpression, ast::Expression>(
            std::make_shared<ast::NullExpression>(-1)
        );
    }
public:
    MiniToAstExpressionVisitor() {};

    antlrcpp::Any visitIntegerExpr(MiniParser::IntegerExprContext* ctx) override {
        return proxy<ast::IntegerExpression, ast::Expression>(
            std::make_shared<ast::IntegerExpression>(
                ctx->getStart()->getLine(),
                std::stoi(ctx->INTEGER()->getText())
            )
        );
    }

    antlrcpp::Any visitTrueExpr(MiniParser::TrueExprContext* ctx) override {
        return proxy<ast::TrueExpression, ast::Expression>(
            std::make_shared<ast::TrueExpression>(
                ctx->getStart()->getLine()
            )
        );
    }

    antlrcpp::Any visitIdentifierExpr(MiniParser::IdentifierExprContext* ctx) override {
        return proxy<ast::IdentifierExpression, ast::Expression>(
            std::make_shared<ast::IdentifierExpression>(
                ctx->getStart()->getLine(),
                ctx->ID()->getText()
            )
        );
    }

    antlrcpp::Any visitBinaryExpr(MiniParser::BinaryExprContext* ctx) override {
        return proxy<ast::BinaryExpression, ast::Expression>(
            std::make_shared<ast::BinaryExpression>(
                ctx->op->getLine(),
                ctx->op->getText(),
                ctx->lft->accept(this),
                ctx->rht->accept(this)
            )
        );
    }

    antlrcpp::Any visitNewExpr(MiniParser::NewExprContext* ctx) override {
        return proxy<ast::NewExpression, ast::Expression>(
            std::make_shared<ast::NewExpression>(
                ctx->getStart()->getLine(),
                ctx->ID()->getText()
            )
        );
    }

    antlrcpp::Any visitDotExpr(MiniParser::DotExprContext* ctx) override {
        return proxy<ast::DotExpression, ast::Expression>(
            std::make_shared<ast::DotExpression>(
                ctx->getStart()->getLine(),
                ctx->expression()->accept(this),
                ctx->ID()->getText()
            )
        );
    }
    
    antlrcpp::Any visitFalseExpr(MiniParser::FalseExprContext* ctx) override {
        return proxy<ast::FalseExpression, ast::Expression>(
            std::make_shared<ast::FalseExpression>(
                ctx->getStart()->getLine()
            )
        );
    }

    antlrcpp::Any visitUnaryExpr(MiniParser::UnaryExprContext* ctx) override {
        return proxy<ast::UnaryExpression, ast::Expression>(
            std::make_shared<ast::UnaryExpression>(
                ctx->op->getLine(),
                ctx->op->getText(),
                ctx->expression()->accept(this)
            )
        );
    }

    std::vector<ast::ExpressionPtr> gatherArguments(MiniParser::ArgumentsContext* ctx) {
        std::vector<ast::ExpressionPtr> args;
        for (auto ectx : ctx->expression()) {
            args.push_back(ectx->accept(this));
        }
        return args;
    }

    antlrcpp::Any visitInvocationExpr(MiniParser::InvocationExprContext* ctx) override {
        return proxy<ast::InvocationExpression, ast::Expression>(
            std::make_shared<ast::InvocationExpression>(
                ctx->getStart()->getLine(),
                ctx->ID()->getText(),
                gatherArguments(ctx->arguments())
            )
        );
    }

    antlrcpp::Any visitNestedExpr(MiniParser::NestedExprContext* ctx) override {
        return ctx->expression()->accept(this);
    }
};

class MiniToAstStatementVisitor : public MiniBaseVisitor {
private:
    MiniToAstExpressionVisitor expressionVisitor;

    ast::ExpressionPtr visitLvalueNested(MiniParser::LvalueContext* ctx) {
        MiniParser::LvalueIdContext* idctx = dynamic_cast<MiniParser::LvalueIdContext*>(ctx);
        if (idctx != nullptr) {
            return proxy<ast::IdentifierExpression, ast::Expression>(
                std::make_shared<ast::IdentifierExpression>(
                    idctx->getStart()->getLine(),
                    idctx->ID()->getText()
                )
            );
        } else {
            MiniParser::LvalueDotContext* dotctx = static_cast<MiniParser::LvalueDotContext*>(ctx);
            return proxy<ast::DotExpression, ast::Expression>(
                std::make_shared<ast::DotExpression>(
                    dotctx->getStart()->getLine(),
                    visitLvalueNested(dotctx->lvalue()),
                    dotctx->ID()->getText()
                )
            );
        }
    }

    ast::LvaluePtr visitLvalue(MiniParser::LvalueContext* ctx) {
        MiniParser::LvalueIdContext* idctx = dynamic_cast<MiniParser::LvalueIdContext*>(ctx);
        if (idctx != nullptr) {
            return proxy<ast::LvalueId, ast::Lvalue>(
                std::make_shared<ast::LvalueId>(
                    idctx->getStart()->getLine(),
                    idctx->ID()->getText()
                )
            );
        } else {
            MiniParser::LvalueDotContext* dotctx = static_cast<MiniParser::LvalueDotContext*>(ctx);
            return proxy<ast::LvalueDot, ast::Lvalue>(
                std::make_shared<ast::LvalueDot>(
                    dotctx->getStart()->getLine(),
                    visitLvalueNested(dotctx->lvalue()),
                    dotctx->ID()->getText()
                )
            );
        }
    }

    std::vector<ast::ExpressionPtr> gatherArguments(MiniParser::ArgumentsContext* ctx) {
        std::vector<ast::ExpressionPtr> args;
        for (auto ectx : ctx->expression()) {
            args.push_back(ectx->accept(&expressionVisitor));
        }
        return args;
    }
protected:
    antlrcpp::Any defaultResult() override {
        return proxy<ast::BlockStatement, ast::Statement>(ast::BlockStatement::emptyBlock());
    }
public:
    MiniToAstStatementVisitor() {};

    antlrcpp::Any visitNestedBlock(MiniParser::NestedBlockContext* ctx) override {
        return ctx->block()->accept(this);
    }

    antlrcpp::Any visitAssignment(MiniParser::AssignmentContext* ctx) override {
        ast::ExpressionPtr exp;
        if (ctx->expression() != nullptr) {
            exp = ctx->expression()->accept(&expressionVisitor);
        } else {
            exp = proxy<ast::ReadExpression, ast::Expression>(
                std::make_shared<ast::ReadExpression>(
                    ctx->getStart()->getLine()
                )
            );
        }

        return proxy<ast::AssignmentStatement, ast::Statement>(
            std::make_shared<ast::AssignmentStatement>(
                ctx->getStart()->getLine(),
                visitLvalue(ctx->lvalue()),
                exp
            )
        );
    }

    antlrcpp::Any visitPrint(MiniParser::PrintContext* ctx) override {
        return proxy<ast::PrintStatement, ast::Statement>(
            std::make_shared<ast::PrintStatement>(
                ctx->getStart()->getLine(),
                ctx->expression()->accept(&expressionVisitor)
            )
        );
    }

    antlrcpp::Any visitPrintLn(MiniParser::PrintLnContext* ctx) override {
        return proxy<ast::PrintLnStatement, ast::Statement>(
            std::make_shared<ast::PrintLnStatement>(
                ctx->getStart()->getLine(),
                ctx->expression()->accept(&expressionVisitor)
            )
        );
    }

    antlrcpp::Any visitConditional(MiniParser::ConditionalContext* ctx) override {
        return proxy<ast::ConditionalStatement, ast::Statement>(
            std::make_shared<ast::ConditionalStatement>(
                ctx->getStart()->getLine(),
                ctx->expression()->accept(&expressionVisitor),
                ctx->thenBlock->accept(this),
                ((ctx->elseBlock != nullptr) ? ctx->elseBlock->accept(this).as<ast::StatementPtr>() : ast::BlockStatement::emptyBlock())
            )
        );
    }

    antlrcpp::Any visitWhile(MiniParser::WhileContext* ctx) override {
        return proxy<ast::WhileStatement, ast::Statement>(
            std::make_shared<ast::WhileStatement>(
                ctx->getStart()->getLine(),
                ctx->expression()->accept(&expressionVisitor),
                ctx->block()->accept(this)
            )
        );
    }

    antlrcpp::Any visitDelete(MiniParser::DeleteContext* ctx) override {
        return proxy<ast::DeleteStatement, ast::Statement>(
            std::make_shared<ast::DeleteStatement>(
                ctx->getStart()->getLine(),
                ctx->expression()->accept(&expressionVisitor)
            )
        );
    }

    antlrcpp::Any visitReturn(MiniParser::ReturnContext* ctx) override {
        if (ctx->expression() != nullptr) {
            return proxy<ast::ReturnStatement, ast::Statement>(
                std::make_shared<ast::ReturnStatement>(
                    ctx->getStart()->getLine(),
                    ctx->expression()->accept(&expressionVisitor)
                )
            );
        } else {
            return proxy<ast::ReturnEmptyStatement, ast::Statement>(
                std::make_shared<ast::ReturnEmptyStatement>(
                    ctx->getStart()->getLine()
                )
            );
        }
    }

    antlrcpp::Any visitInvocation(MiniParser::InvocationContext* ctx) override {
        return proxy<ast::InvocationStatement, ast::Statement>(
            std::make_shared<ast::InvocationStatement>(
                ctx->getStart()->getLine(),
                std::make_shared<ast::InvocationExpression>(
                    ctx->getStart()->getLine(),
                    ctx->ID()->getText(),
                    gatherArguments(ctx->arguments())
                )
            )
        );
    }

    antlrcpp::Any visitStatementList(MiniParser::StatementListContext* ctx) override {
        std::vector<ast::StatementPtr> statements;
        for (auto sctx : ctx->statement()) {
            statements.push_back(sctx->accept(this));
        }
        return proxy<ast::BlockStatement, ast::Statement>(
            std::make_shared<ast::BlockStatement>(
                ctx->getStart()->getLine(),
                statements
            )
        );
    }

    antlrcpp::Any visitBlock(MiniParser::BlockContext* ctx) {
        return ctx->statementList()->accept(this);
    }
};

class MiniToAstDeclarationsVisitor : public MiniBaseVisitor {
private:
    MiniToAstTypeVisitor typeVisitor;

    void addDeclarationsTo(MiniParser::DeclarationContext* ctx, std::vector<ast::DeclarationPtr>& decls) {
        ast::TypePtr type = ctx->type()->accept(&typeVisitor);

        for (antlr4::tree::TerminalNode* node : ctx->ID()) {;
            decls.push_back(
                std::make_shared<ast::Declaration>(
                    node->getSymbol()->getLine(),
                    type,
                    node->getText()
                )
            );
        }
    }
protected:
    antlrcpp::Any defaultResult() override {
        return std::vector<ast::Declaration>();
    }

public:
    antlrcpp::Any visitDeclarations(MiniParser::DeclarationsContext* ctx) override {
        std::vector<ast::DeclarationPtr> decls;
        for (auto dctx : ctx->declaration()) {
            addDeclarationsTo(dctx, decls);
        }
        return decls;
    }
};

class MiniToAstFunctionVisitor : public MiniBaseVisitor {
private:
    MiniToAstTypeVisitor typeVisitor;
    MiniToAstDeclarationsVisitor declarationsVisitor;
    MiniToAstStatementVisitor statementVisitor;

    std::vector<ast::DeclarationPtr> gatherParameters(MiniParser::ParametersContext* ctx) {
        std::vector<ast::DeclarationPtr> params;
        for (auto dctx : ctx->decl()) {
            params.push_back(
                std::make_shared<ast::Declaration>(
                    dctx->getStart()->getLine(),
                    dctx->type()->accept(&typeVisitor),
                    dctx->ID()->getText()
                )
            );
        }
        return params;
    }
protected:
    antlrcpp::Any defaultResult() override {
        return std::make_shared<ast::Function>(
            -1,
            "invalid",
            std::vector<ast::DeclarationPtr>(),
            std::make_shared<ast::VoidType>(),
            std::vector<ast::DeclarationPtr>(),
            ast::BlockStatement::emptyBlock()
        );
    }
public:
    MiniToAstFunctionVisitor() {};

    antlrcpp::Any visitFunction(MiniParser::FunctionContext* ctx) {
        return std::make_shared<ast::Function>(
            ctx->getStart()->getLine(),
            ctx->ID()->getText(),
            gatherParameters(ctx->parameters()),
            ctx->returnType()->accept(&typeVisitor),
            ctx->declarations()->accept(&declarationsVisitor),
            ctx->statementList()->accept(&statementVisitor)
        );
    }
};

class MiniToAstTypeDeclarationVisitor : public MiniBaseVisitor {
private:
    MiniToAstTypeVisitor typeVisitor;

    std::vector<ast::DeclarationPtr> gatherFieldDeclarations(MiniParser::NestedDeclContext* ctx) {
        std::vector<ast::DeclarationPtr> fields;

        for (auto dctx : ctx->decl()) {
            fields.push_back(
                std::make_shared<ast::Declaration>(
                    dctx->getStart()->getLine(),
                    dctx->type()->accept(&typeVisitor),
                    dctx->ID()->getText()
                )
            );
        }
        return fields;
    }
protected:
    antlrcpp::Any defaultResult() override {
        return ast::TypeDeclaration(-1, "invalid", {});
    }
public:
    MiniToAstTypeDeclarationVisitor() {};

    antlrcpp::Any visitTypeDeclaration(MiniParser::TypeDeclarationContext* ctx) override {
        return std::make_shared<ast::TypeDeclaration>(
            ctx->getStart()->getLine(),
            ctx->ID()->getText(),
            gatherFieldDeclarations(ctx->nestedDecl())
        );
    };
};


class MiniToAstProgramVisitor : public MiniBaseVisitor {
private:
    MiniToAstTypeDeclarationVisitor typeDeclarationVisitor;
    MiniToAstDeclarationsVisitor declarationsVisitor;
    MiniToAstFunctionVisitor functionVisitor;
private:
    std::vector<ast::TypeDeclarationPtr> gatherTypes(MiniParser::TypesContext* ctx) {
        std::vector<ast::TypeDeclarationPtr> types;

        for (auto tctx : ctx->typeDeclaration()) {
            types.push_back(tctx->accept(&typeDeclarationVisitor));
        }

        return types;
    }

    std::vector<ast::DeclarationPtr> gatherDeclarations(MiniParser::DeclarationsContext* ctx) {
        return ctx->accept(&declarationsVisitor);
    }

    std::vector<ast::FunctionPtr> gatherFunctions(MiniParser::FunctionsContext* ctx) {
        std::vector<ast::FunctionPtr> funcs;
        for (auto fctx : ctx->function()) {
            funcs.push_back(fctx->accept(&functionVisitor));
        }
        return funcs;
    }
protected:
    antlrcpp::Any defaultResult() {
        return std::make_shared<ast::Program>(
            std::vector<ast::TypeDeclarationPtr>(),
            std::vector<ast::DeclarationPtr>(),
            std::vector<ast::FunctionPtr>()
        );
    }
public:
    MiniToAstProgramVisitor() {};
    antlrcpp::Any visitProgram(MiniParser::ProgramContext* ctx) override {
        auto types = gatherTypes(ctx->types());
        auto decls = gatherDeclarations(ctx->declarations());
        auto funcs = gatherFunctions(ctx->functions());
        return std::make_shared<ast::Program>(types, decls, funcs);
    };
};
