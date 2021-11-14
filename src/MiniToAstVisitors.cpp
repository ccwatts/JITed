
#include "MiniToAstVisitors.h"

#include "MiniBaseVisitor.h"

#include "antlr4-common.h"

#include "ast/program.h"
#include "ast/expressions.h"
#include "ast/statements.h"
#include "ast/types.h"

#include "lib/proxy.h"

using jited::proxy;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

antlrcpp::Any MiniToAstTypeVisitor::defaultResult() {
    return proxy<jited::ast::VoidType, jited::ast::Type>(
        std::make_shared<jited::ast::VoidType>()
    );
}

MiniToAstTypeVisitor::MiniToAstTypeVisitor() {};

antlrcpp::Any MiniToAstTypeVisitor::visitIntType(MiniParser::IntTypeContext* ctx) {
    return proxy<jited::ast::IntType, jited::ast::Type>(
        std::make_shared<jited::ast::IntType>()
    );
}

antlrcpp::Any MiniToAstTypeVisitor::visitBoolType(MiniParser::BoolTypeContext* ctx) {
    return proxy<jited::ast::BoolType, jited::ast::Type>(
        std::make_shared<jited::ast::BoolType>()
    );
}

antlrcpp::Any MiniToAstTypeVisitor::visitStructType(MiniParser::StructTypeContext* ctx) {
    return proxy<jited::ast::StructType, jited::ast::Type>(
        std::make_shared<jited::ast::StructType>(ctx->getStart()->getLine(), ctx->ID()->getText())
    );
}

antlrcpp::Any MiniToAstTypeVisitor::visitReturnTypeReal(MiniParser::ReturnTypeRealContext* ctx) {
    return ctx->type()->accept(this);
}

antlrcpp::Any MiniToAstTypeVisitor::visitReturnTypeVoid(MiniParser::ReturnTypeVoidContext* ctx) {
    return proxy<jited::ast::VoidType, jited::ast::Type>(
        std::make_shared<jited::ast::VoidType>()
    );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

antlrcpp::Any MiniToAstExpressionVisitor::defaultResult() {
    return proxy<jited::ast::NullExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::NullExpression>(-1)
    );
}

MiniToAstExpressionVisitor::MiniToAstExpressionVisitor() {};

antlrcpp::Any MiniToAstExpressionVisitor::visitIntegerExpr(MiniParser::IntegerExprContext* ctx) {
    return proxy<jited::ast::IntegerExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::IntegerExpression>(
            ctx->getStart()->getLine(),
            std::stoi(ctx->INTEGER()->getText())
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitTrueExpr(MiniParser::TrueExprContext* ctx) {
    return proxy<jited::ast::TrueExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::TrueExpression>(
            ctx->getStart()->getLine()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitIdentifierExpr(MiniParser::IdentifierExprContext* ctx) {
    return proxy<jited::ast::IdentifierExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::IdentifierExpression>(
            ctx->getStart()->getLine(),
            ctx->ID()->getText()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitBinaryExpr(MiniParser::BinaryExprContext* ctx) {
    return proxy<jited::ast::BinaryExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::BinaryExpression>(
            ctx->op->getLine(),
            ctx->op->getText(),
            ctx->lft->accept(this),
            ctx->rht->accept(this)
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitNewExpr(MiniParser::NewExprContext* ctx) {
    return proxy<jited::ast::NewExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::NewExpression>(
            ctx->getStart()->getLine(),
            ctx->ID()->getText()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitDotExpr(MiniParser::DotExprContext* ctx) {
    return proxy<jited::ast::DotExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::DotExpression>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(this),
            ctx->ID()->getText()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitFalseExpr(MiniParser::FalseExprContext* ctx) {
    return proxy<jited::ast::FalseExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::FalseExpression>(
            ctx->getStart()->getLine()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitUnaryExpr(MiniParser::UnaryExprContext* ctx) {
    return proxy<jited::ast::UnaryExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::UnaryExpression>(
            ctx->op->getLine(),
            ctx->op->getText(),
            ctx->expression()->accept(this)
        )
    );
}

std::vector<jited::ast::ExpressionPtr> MiniToAstExpressionVisitor::gatherArguments(MiniParser::ArgumentsContext* ctx) {
    std::vector<jited::ast::ExpressionPtr> args;
    for (auto ectx : ctx->expression()) {
        args.push_back(ectx->accept(this));
    }
    return args;
}

antlrcpp::Any MiniToAstExpressionVisitor::visitInvocationExpr(MiniParser::InvocationExprContext* ctx) {
    return proxy<jited::ast::InvocationExpression, jited::ast::Expression>(
        std::make_shared<jited::ast::InvocationExpression>(
            ctx->getStart()->getLine(),
            ctx->ID()->getText(),
            gatherArguments(ctx->arguments())
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitNestedExpr(MiniParser::NestedExprContext* ctx) {
    return ctx->expression()->accept(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

jited::ast::ExpressionPtr MiniToAstStatementVisitor::visitLvalueNested(MiniParser::LvalueContext* ctx) {
    MiniParser::LvalueIdContext* idctx = dynamic_cast<MiniParser::LvalueIdContext*>(ctx);
    if (idctx != nullptr) {
        return proxy<jited::ast::IdentifierExpression, jited::ast::Expression>(
            std::make_shared<jited::ast::IdentifierExpression>(
                idctx->getStart()->getLine(),
                idctx->ID()->getText()
            )
        );
    } else {
        MiniParser::LvalueDotContext* dotctx = static_cast<MiniParser::LvalueDotContext*>(ctx);
        return proxy<jited::ast::DotExpression, jited::ast::Expression>(
            std::make_shared<jited::ast::DotExpression>(
                dotctx->getStart()->getLine(),
                visitLvalueNested(dotctx->lvalue()),
                dotctx->ID()->getText()
            )
        );
    }
}

jited::ast::LvaluePtr MiniToAstStatementVisitor::visitLvalue(MiniParser::LvalueContext* ctx) {
    MiniParser::LvalueIdContext* idctx = dynamic_cast<MiniParser::LvalueIdContext*>(ctx);
    if (idctx != nullptr) {
        return proxy<jited::ast::LvalueId, jited::ast::Lvalue>(
            std::make_shared<jited::ast::LvalueId>(
                idctx->getStart()->getLine(),
                idctx->ID()->getText()
            )
        );
    } else {
        MiniParser::LvalueDotContext* dotctx = static_cast<MiniParser::LvalueDotContext*>(ctx);
        return proxy<jited::ast::LvalueDot, jited::ast::Lvalue>(
            std::make_shared<jited::ast::LvalueDot>(
                dotctx->getStart()->getLine(),
                visitLvalueNested(dotctx->lvalue()),
                dotctx->ID()->getText()
            )
        );
    }
}

std::vector<jited::ast::ExpressionPtr> MiniToAstStatementVisitor::gatherArguments(MiniParser::ArgumentsContext* ctx) {
    std::vector<jited::ast::ExpressionPtr> args;
    for (auto ectx : ctx->expression()) {
        args.push_back(ectx->accept(&expressionVisitor));
    }
    return args;
}

antlrcpp::Any MiniToAstStatementVisitor::defaultResult() {
    return proxy<jited::ast::BlockStatement, jited::ast::Statement>(jited::ast::BlockStatement::emptyBlock());
}

MiniToAstStatementVisitor::MiniToAstStatementVisitor() {};

antlrcpp::Any MiniToAstStatementVisitor::visitNestedBlock(MiniParser::NestedBlockContext* ctx) {
    return ctx->block()->accept(this);
}

antlrcpp::Any MiniToAstStatementVisitor::visitAssignment(MiniParser::AssignmentContext* ctx) {
    jited::ast::ExpressionPtr exp;
    if (ctx->expression() != nullptr) {
        exp = ctx->expression()->accept(&expressionVisitor);
    } else {
        exp = proxy<jited::ast::ReadExpression, jited::ast::Expression>(
            std::make_shared<jited::ast::ReadExpression>(
                ctx->getStart()->getLine()
            )
        );
    }

    return proxy<jited::ast::AssignmentStatement, jited::ast::Statement>(
        std::make_shared<jited::ast::AssignmentStatement>(
            ctx->getStart()->getLine(),
            visitLvalue(ctx->lvalue()),
            exp
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitPrint(MiniParser::PrintContext* ctx) {
    return proxy<jited::ast::PrintStatement, jited::ast::Statement>(
        std::make_shared<jited::ast::PrintStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor)
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitPrintLn(MiniParser::PrintLnContext* ctx) {
    return proxy<jited::ast::PrintLnStatement, jited::ast::Statement>(
        std::make_shared<jited::ast::PrintLnStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor)
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitConditional(MiniParser::ConditionalContext* ctx) {
    return proxy<jited::ast::ConditionalStatement, jited::ast::Statement>(
        std::make_shared<jited::ast::ConditionalStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor),
            ctx->thenBlock->accept(this),
            ((ctx->elseBlock != nullptr) ? ctx->elseBlock->accept(this).as<jited::ast::StatementPtr>() : jited::ast::BlockStatement::emptyBlock())
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitWhile(MiniParser::WhileContext* ctx) {
    return proxy<jited::ast::WhileStatement, jited::ast::Statement>(
        std::make_shared<jited::ast::WhileStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor),
            ctx->block()->accept(this)
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitDelete(MiniParser::DeleteContext* ctx) {
    return proxy<jited::ast::DeleteStatement, jited::ast::Statement>(
        std::make_shared<jited::ast::DeleteStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor)
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitReturn(MiniParser::ReturnContext* ctx) {
    if (ctx->expression() != nullptr) {
        return proxy<jited::ast::ReturnStatement, jited::ast::Statement>(
            std::make_shared<jited::ast::ReturnStatement>(
                ctx->getStart()->getLine(),
                ctx->expression()->accept(&expressionVisitor)
            )
        );
    } else {
        return proxy<jited::ast::ReturnEmptyStatement, jited::ast::Statement>(
            std::make_shared<jited::ast::ReturnEmptyStatement>(
                ctx->getStart()->getLine()
            )
        );
    }
}

antlrcpp::Any MiniToAstStatementVisitor::visitInvocation(MiniParser::InvocationContext* ctx) {
    return proxy<jited::ast::InvocationStatement, jited::ast::Statement>(
        std::make_shared<jited::ast::InvocationStatement>(
            ctx->getStart()->getLine(),
            std::make_shared<jited::ast::InvocationExpression>(
                ctx->getStart()->getLine(),
                ctx->ID()->getText(),
                gatherArguments(ctx->arguments())
            )
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitStatementList(MiniParser::StatementListContext* ctx) {
    std::vector<jited::ast::StatementPtr> statements;
    for (auto sctx : ctx->statement()) {
        statements.push_back(sctx->accept(this));
    }
    return proxy<jited::ast::BlockStatement, jited::ast::Statement>(
        std::make_shared<jited::ast::BlockStatement>(
            ctx->getStart()->getLine(),
            statements
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitBlock(MiniParser::BlockContext* ctx) {
    return ctx->statementList()->accept(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MiniToAstDeclarationsVisitor::addDeclarationsTo(MiniParser::DeclarationContext* ctx, std::vector<jited::ast::DeclarationPtr>& decls) {
    jited::ast::TypePtr type = ctx->type()->accept(&typeVisitor);

    for (antlr4::tree::TerminalNode* node : ctx->ID()) {;
        decls.push_back(
            std::make_shared<jited::ast::Declaration>(
                node->getSymbol()->getLine(),
                type,
                node->getText()
            )
        );
    }
}

antlrcpp::Any MiniToAstDeclarationsVisitor::defaultResult() {
    return std::vector<jited::ast::Declaration>();
}

antlrcpp::Any MiniToAstDeclarationsVisitor::visitDeclarations(MiniParser::DeclarationsContext* ctx) {
    std::vector<jited::ast::DeclarationPtr> decls;
    for (auto dctx : ctx->declaration()) {
        addDeclarationsTo(dctx, decls);
    }
    return decls;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<jited::ast::DeclarationPtr> MiniToAstFunctionVisitor::gatherParameters(MiniParser::ParametersContext* ctx) {
    std::vector<jited::ast::DeclarationPtr> params;
    for (auto dctx : ctx->decl()) {
        params.push_back(
            std::make_shared<jited::ast::Declaration>(
                dctx->getStart()->getLine(),
                dctx->type()->accept(&typeVisitor),
                dctx->ID()->getText()
            )
        );
    }
    return params;
}

antlrcpp::Any MiniToAstFunctionVisitor::defaultResult() {
    return std::make_shared<jited::ast::Function>(
        -1,
        "invalid",
        std::vector<jited::ast::DeclarationPtr>(),
        std::make_shared<jited::ast::VoidType>(),
        std::vector<jited::ast::DeclarationPtr>(),
        jited::ast::BlockStatement::emptyBlock()
    );
}

MiniToAstFunctionVisitor::MiniToAstFunctionVisitor() {};

antlrcpp::Any MiniToAstFunctionVisitor::visitFunction(MiniParser::FunctionContext* ctx) {
    return std::make_shared<jited::ast::Function>(
        ctx->getStart()->getLine(),
        ctx->ID()->getText(),
        gatherParameters(ctx->parameters()),
        ctx->returnType()->accept(&typeVisitor),
        ctx->declarations()->accept(&declarationsVisitor),
        ctx->statementList()->accept(&statementVisitor)
    );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<jited::ast::DeclarationPtr> MiniToAstTypeDeclarationVisitor::gatherFieldDeclarations(MiniParser::NestedDeclContext* ctx) {
    std::vector<jited::ast::DeclarationPtr> fields;

    for (auto dctx : ctx->decl()) {
        fields.push_back(
            std::make_shared<jited::ast::Declaration>(
                dctx->getStart()->getLine(),
                dctx->type()->accept(&typeVisitor),
                dctx->ID()->getText()
            )
        );
    }
    return fields;
}

antlrcpp::Any MiniToAstTypeDeclarationVisitor::defaultResult() {
    return jited::ast::TypeDeclaration(-1, "invalid", {});
}

MiniToAstTypeDeclarationVisitor::MiniToAstTypeDeclarationVisitor() {};

antlrcpp::Any MiniToAstTypeDeclarationVisitor::visitTypeDeclaration(MiniParser::TypeDeclarationContext* ctx) {
    return std::make_shared<jited::ast::TypeDeclaration>(
        ctx->getStart()->getLine(),
        ctx->ID()->getText(),
        gatherFieldDeclarations(ctx->nestedDecl())
    );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<jited::ast::TypeDeclarationPtr> MiniToAstProgramVisitor::gatherTypes(MiniParser::TypesContext* ctx) {
    std::vector<jited::ast::TypeDeclarationPtr> types;

    for (auto tctx : ctx->typeDeclaration()) {
        types.push_back(tctx->accept(&typeDeclarationVisitor));
    }

    return types;
}

std::vector<jited::ast::DeclarationPtr> MiniToAstProgramVisitor::gatherDeclarations(MiniParser::DeclarationsContext* ctx) {
    return ctx->accept(&declarationsVisitor);
}

std::vector<jited::ast::FunctionPtr> MiniToAstProgramVisitor::gatherFunctions(MiniParser::FunctionsContext* ctx) {
    std::vector<jited::ast::FunctionPtr> funcs;
    for (auto fctx : ctx->function()) {
        funcs.push_back(fctx->accept(&functionVisitor));
    }
    return funcs;
}

antlrcpp::Any MiniToAstProgramVisitor::defaultResult() {
    return std::make_shared<jited::ast::Program>(
        std::vector<jited::ast::TypeDeclarationPtr>(),
        std::vector<jited::ast::DeclarationPtr>(),
        std::vector<jited::ast::FunctionPtr>()
    );
}

MiniToAstProgramVisitor::MiniToAstProgramVisitor() {};

antlrcpp::Any MiniToAstProgramVisitor::visitProgram(MiniParser::ProgramContext* ctx) {
    auto types = gatherTypes(ctx->types());
    auto decls = gatherDeclarations(ctx->declarations());
    auto funcs = gatherFunctions(ctx->functions());
    return std::make_shared<jited::ast::Program>(types, decls, funcs);
};