
#include "MiniToAstVisitors.h"

#include "MiniBaseVisitor.h"

#include "antlr4-common.h"

#include "ast/program.h"
#include "ast/expressions.h"
#include "ast/statements.h"
#include "ast/types.h"

#include "lib/proxy.h"

using mini::proxy;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

antlrcpp::Any MiniToAstTypeVisitor::defaultResult() {
    return proxy<ast::VoidType, ast::Type>(
        std::make_shared<ast::VoidType>()
    );
}

MiniToAstTypeVisitor::MiniToAstTypeVisitor() {};

antlrcpp::Any MiniToAstTypeVisitor::visitIntType(MiniParser::IntTypeContext* ctx) {
    return proxy<ast::IntType, ast::Type>(
        std::make_shared<ast::IntType>()
    );
}

antlrcpp::Any MiniToAstTypeVisitor::visitBoolType(MiniParser::BoolTypeContext* ctx) {
    return proxy<ast::BoolType, ast::Type>(
        std::make_shared<ast::BoolType>()
    );
}

antlrcpp::Any MiniToAstTypeVisitor::visitStructType(MiniParser::StructTypeContext* ctx) {
    return proxy<ast::StructType, ast::Type>(
        std::make_shared<ast::StructType>(ctx->getStart()->getLine(), ctx->ID()->getText())
    );
}

antlrcpp::Any MiniToAstTypeVisitor::visitReturnTypeReal(MiniParser::ReturnTypeRealContext* ctx) {
    return ctx->type()->accept(this);
}

antlrcpp::Any MiniToAstTypeVisitor::visitReturnTypeVoid(MiniParser::ReturnTypeVoidContext* ctx) {
    return proxy<ast::VoidType, ast::Type>(
        std::make_shared<ast::VoidType>()
    );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

antlrcpp::Any MiniToAstExpressionVisitor::defaultResult() {
    return proxy<ast::NullExpression, ast::Expression>(
        std::make_shared<ast::NullExpression>(-1)
    );
}

MiniToAstExpressionVisitor::MiniToAstExpressionVisitor() {};

antlrcpp::Any MiniToAstExpressionVisitor::visitIntegerExpr(MiniParser::IntegerExprContext* ctx) {
    return proxy<ast::IntegerExpression, ast::Expression>(
        std::make_shared<ast::IntegerExpression>(
            ctx->getStart()->getLine(),
            std::stoi(ctx->INTEGER()->getText())
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitTrueExpr(MiniParser::TrueExprContext* ctx) {
    return proxy<ast::TrueExpression, ast::Expression>(
        std::make_shared<ast::TrueExpression>(
            ctx->getStart()->getLine()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitIdentifierExpr(MiniParser::IdentifierExprContext* ctx) {
    return proxy<ast::IdentifierExpression, ast::Expression>(
        std::make_shared<ast::IdentifierExpression>(
            ctx->getStart()->getLine(),
            ctx->ID()->getText()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitBinaryExpr(MiniParser::BinaryExprContext* ctx) {
    return proxy<ast::BinaryExpression, ast::Expression>(
        std::make_shared<ast::BinaryExpression>(
            ctx->op->getLine(),
            ctx->op->getText(),
            ctx->lft->accept(this),
            ctx->rht->accept(this)
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitNewExpr(MiniParser::NewExprContext* ctx) {
    return proxy<ast::NewExpression, ast::Expression>(
        std::make_shared<ast::NewExpression>(
            ctx->getStart()->getLine(),
            ctx->ID()->getText()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitDotExpr(MiniParser::DotExprContext* ctx) {
    return proxy<ast::DotExpression, ast::Expression>(
        std::make_shared<ast::DotExpression>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(this),
            ctx->ID()->getText()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitFalseExpr(MiniParser::FalseExprContext* ctx) {
    return proxy<ast::FalseExpression, ast::Expression>(
        std::make_shared<ast::FalseExpression>(
            ctx->getStart()->getLine()
        )
    );
}

antlrcpp::Any MiniToAstExpressionVisitor::visitUnaryExpr(MiniParser::UnaryExprContext* ctx) {
    return proxy<ast::UnaryExpression, ast::Expression>(
        std::make_shared<ast::UnaryExpression>(
            ctx->op->getLine(),
            ctx->op->getText(),
            ctx->expression()->accept(this)
        )
    );
}

std::vector<ast::ExpressionPtr> MiniToAstExpressionVisitor::gatherArguments(MiniParser::ArgumentsContext* ctx) {
    std::vector<ast::ExpressionPtr> args;
    for (auto ectx : ctx->expression()) {
        args.push_back(ectx->accept(this));
    }
    return args;
}

antlrcpp::Any MiniToAstExpressionVisitor::visitInvocationExpr(MiniParser::InvocationExprContext* ctx) {
    return proxy<ast::InvocationExpression, ast::Expression>(
        std::make_shared<ast::InvocationExpression>(
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

ast::ExpressionPtr MiniToAstStatementVisitor::visitLvalueNested(MiniParser::LvalueContext* ctx) {
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

ast::LvaluePtr MiniToAstStatementVisitor::visitLvalue(MiniParser::LvalueContext* ctx) {
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

std::vector<ast::ExpressionPtr> MiniToAstStatementVisitor::gatherArguments(MiniParser::ArgumentsContext* ctx) {
    std::vector<ast::ExpressionPtr> args;
    for (auto ectx : ctx->expression()) {
        args.push_back(ectx->accept(&expressionVisitor));
    }
    return args;
}

antlrcpp::Any MiniToAstStatementVisitor::defaultResult() {
    return proxy<ast::BlockStatement, ast::Statement>(ast::BlockStatement::emptyBlock());
}

MiniToAstStatementVisitor::MiniToAstStatementVisitor() {};

antlrcpp::Any MiniToAstStatementVisitor::visitNestedBlock(MiniParser::NestedBlockContext* ctx) {
    return ctx->block()->accept(this);
}

antlrcpp::Any MiniToAstStatementVisitor::visitAssignment(MiniParser::AssignmentContext* ctx) {
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

antlrcpp::Any MiniToAstStatementVisitor::visitPrint(MiniParser::PrintContext* ctx) {
    return proxy<ast::PrintStatement, ast::Statement>(
        std::make_shared<ast::PrintStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor)
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitPrintLn(MiniParser::PrintLnContext* ctx) {
    return proxy<ast::PrintLnStatement, ast::Statement>(
        std::make_shared<ast::PrintLnStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor)
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitConditional(MiniParser::ConditionalContext* ctx) {
    return proxy<ast::ConditionalStatement, ast::Statement>(
        std::make_shared<ast::ConditionalStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor),
            ctx->thenBlock->accept(this),
            ((ctx->elseBlock != nullptr) ? ctx->elseBlock->accept(this).as<ast::StatementPtr>() : ast::BlockStatement::emptyBlock())
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitWhile(MiniParser::WhileContext* ctx) {
    return proxy<ast::WhileStatement, ast::Statement>(
        std::make_shared<ast::WhileStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor),
            ctx->block()->accept(this)
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitDelete(MiniParser::DeleteContext* ctx) {
    return proxy<ast::DeleteStatement, ast::Statement>(
        std::make_shared<ast::DeleteStatement>(
            ctx->getStart()->getLine(),
            ctx->expression()->accept(&expressionVisitor)
        )
    );
}

antlrcpp::Any MiniToAstStatementVisitor::visitReturn(MiniParser::ReturnContext* ctx) {
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

antlrcpp::Any MiniToAstStatementVisitor::visitInvocation(MiniParser::InvocationContext* ctx) {
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

antlrcpp::Any MiniToAstStatementVisitor::visitStatementList(MiniParser::StatementListContext* ctx) {
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

antlrcpp::Any MiniToAstStatementVisitor::visitBlock(MiniParser::BlockContext* ctx) {
    return ctx->statementList()->accept(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MiniToAstDeclarationsVisitor::addDeclarationsTo(MiniParser::DeclarationContext* ctx, std::vector<ast::DeclarationPtr>& decls) {
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

antlrcpp::Any MiniToAstDeclarationsVisitor::defaultResult() {
    return std::vector<ast::Declaration>();
}

antlrcpp::Any MiniToAstDeclarationsVisitor::visitDeclarations(MiniParser::DeclarationsContext* ctx) {
    std::vector<ast::DeclarationPtr> decls;
    for (auto dctx : ctx->declaration()) {
        addDeclarationsTo(dctx, decls);
    }
    return decls;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<ast::DeclarationPtr> MiniToAstFunctionVisitor::gatherParameters(MiniParser::ParametersContext* ctx) {
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

antlrcpp::Any MiniToAstFunctionVisitor::defaultResult() {
    return std::make_shared<ast::Function>(
        -1,
        "invalid",
        std::vector<ast::DeclarationPtr>(),
        std::make_shared<ast::VoidType>(),
        std::vector<ast::DeclarationPtr>(),
        ast::BlockStatement::emptyBlock()
    );
}

MiniToAstFunctionVisitor::MiniToAstFunctionVisitor() {};

antlrcpp::Any MiniToAstFunctionVisitor::visitFunction(MiniParser::FunctionContext* ctx) {
    return std::make_shared<ast::Function>(
        ctx->getStart()->getLine(),
        ctx->ID()->getText(),
        gatherParameters(ctx->parameters()),
        ctx->returnType()->accept(&typeVisitor),
        ctx->declarations()->accept(&declarationsVisitor),
        ctx->statementList()->accept(&statementVisitor)
    );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<ast::DeclarationPtr> MiniToAstTypeDeclarationVisitor::gatherFieldDeclarations(MiniParser::NestedDeclContext* ctx) {
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

antlrcpp::Any MiniToAstTypeDeclarationVisitor::defaultResult() {
    return ast::TypeDeclaration(-1, "invalid", {});
}

MiniToAstTypeDeclarationVisitor::MiniToAstTypeDeclarationVisitor() {};

antlrcpp::Any MiniToAstTypeDeclarationVisitor::visitTypeDeclaration(MiniParser::TypeDeclarationContext* ctx) {
    return std::make_shared<ast::TypeDeclaration>(
        ctx->getStart()->getLine(),
        ctx->ID()->getText(),
        gatherFieldDeclarations(ctx->nestedDecl())
    );
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<ast::TypeDeclarationPtr> MiniToAstProgramVisitor::gatherTypes(MiniParser::TypesContext* ctx) {
    std::vector<ast::TypeDeclarationPtr> types;

    for (auto tctx : ctx->typeDeclaration()) {
        types.push_back(tctx->accept(&typeDeclarationVisitor));
    }

    return types;
}

std::vector<ast::DeclarationPtr> MiniToAstProgramVisitor::gatherDeclarations(MiniParser::DeclarationsContext* ctx) {
    return ctx->accept(&declarationsVisitor);
}

std::vector<ast::FunctionPtr> MiniToAstProgramVisitor::gatherFunctions(MiniParser::FunctionsContext* ctx) {
    std::vector<ast::FunctionPtr> funcs;
    for (auto fctx : ctx->function()) {
        funcs.push_back(fctx->accept(&functionVisitor));
    }
    return funcs;
}

antlrcpp::Any MiniToAstProgramVisitor::defaultResult() {
    return std::make_shared<ast::Program>(
        std::vector<ast::TypeDeclarationPtr>(),
        std::vector<ast::DeclarationPtr>(),
        std::vector<ast::FunctionPtr>()
    );
}

MiniToAstProgramVisitor::MiniToAstProgramVisitor() {};

antlrcpp::Any MiniToAstProgramVisitor::visitProgram(MiniParser::ProgramContext* ctx) {
    auto types = gatherTypes(ctx->types());
    auto decls = gatherDeclarations(ctx->declarations());
    auto funcs = gatherFunctions(ctx->functions());
    return std::make_shared<ast::Program>(types, decls, funcs);
};