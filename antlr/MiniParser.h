
   /* package declaration here */


// Generated from Mini.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"




class  MiniParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    T__32 = 33, T__33 = 34, T__34 = 35, T__35 = 36, T__36 = 37, T__37 = 38, 
    ID = 39, INTEGER = 40, WS = 41, COMMENT = 42
  };

  enum {
    RuleProgram = 0, RuleTypes = 1, RuleTypeDeclaration = 2, RuleNestedDecl = 3, 
    RuleDecl = 4, RuleType = 5, RuleDeclarations = 6, RuleDeclaration = 7, 
    RuleFunctions = 8, RuleFunction = 9, RuleParameters = 10, RuleReturnType = 11, 
    RuleStatement = 12, RuleBlock = 13, RuleStatementList = 14, RuleLvalue = 15, 
    RuleExpression = 16, RuleArguments = 17
  };

  MiniParser(antlr4::TokenStream *input);
  ~MiniParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class ProgramContext;
  class TypesContext;
  class TypeDeclarationContext;
  class NestedDeclContext;
  class DeclContext;
  class TypeContext;
  class DeclarationsContext;
  class DeclarationContext;
  class FunctionsContext;
  class FunctionContext;
  class ParametersContext;
  class ReturnTypeContext;
  class StatementContext;
  class BlockContext;
  class StatementListContext;
  class LvalueContext;
  class ExpressionContext;
  class ArgumentsContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypesContext *types();
    DeclarationsContext *declarations();
    FunctionsContext *functions();
    antlr4::tree::TerminalNode *EOF();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgramContext* program();

  class  TypesContext : public antlr4::ParserRuleContext {
  public:
    TypesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TypeDeclarationContext *> typeDeclaration();
    TypeDeclarationContext* typeDeclaration(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypesContext* types();

  class  TypeDeclarationContext : public antlr4::ParserRuleContext {
  public:
    TypeDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    NestedDeclContext *nestedDecl();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeDeclarationContext* typeDeclaration();

  class  NestedDeclContext : public antlr4::ParserRuleContext {
  public:
    NestedDeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclContext *> decl();
    DeclContext* decl(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NestedDeclContext* nestedDecl();

  class  DeclContext : public antlr4::ParserRuleContext {
  public:
    DeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    antlr4::tree::TerminalNode *ID();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclContext* decl();

  class  TypeContext : public antlr4::ParserRuleContext {
  public:
    TypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    TypeContext() = default;
    void copyFrom(TypeContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  BoolTypeContext : public TypeContext {
  public:
    BoolTypeContext(TypeContext *ctx);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  StructTypeContext : public TypeContext {
  public:
    StructTypeContext(TypeContext *ctx);

    antlr4::tree::TerminalNode *ID();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IntTypeContext : public TypeContext {
  public:
    IntTypeContext(TypeContext *ctx);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  TypeContext* type();

  class  DeclarationsContext : public antlr4::ParserRuleContext {
  public:
    DeclarationsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclarationContext *> declaration();
    DeclarationContext* declaration(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationsContext* declarations();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TypeContext *type();
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationContext* declaration();

  class  FunctionsContext : public antlr4::ParserRuleContext {
  public:
    FunctionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<FunctionContext *> function();
    FunctionContext* function(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionsContext* functions();

  class  FunctionContext : public antlr4::ParserRuleContext {
  public:
    FunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    ParametersContext *parameters();
    ReturnTypeContext *returnType();
    DeclarationsContext *declarations();
    StatementListContext *statementList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionContext* function();

  class  ParametersContext : public antlr4::ParserRuleContext {
  public:
    ParametersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclContext *> decl();
    DeclContext* decl(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParametersContext* parameters();

  class  ReturnTypeContext : public antlr4::ParserRuleContext {
  public:
    ReturnTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ReturnTypeContext() = default;
    void copyFrom(ReturnTypeContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ReturnTypeVoidContext : public ReturnTypeContext {
  public:
    ReturnTypeVoidContext(ReturnTypeContext *ctx);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnTypeRealContext : public ReturnTypeContext {
  public:
    ReturnTypeRealContext(ReturnTypeContext *ctx);

    TypeContext *type();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ReturnTypeContext* returnType();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    StatementContext() = default;
    void copyFrom(StatementContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  AssignmentContext : public StatementContext {
  public:
    AssignmentContext(StatementContext *ctx);

    LvalueContext *lvalue();
    ExpressionContext *expression();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DeleteContext : public StatementContext {
  public:
    DeleteContext(StatementContext *ctx);

    ExpressionContext *expression();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PrintContext : public StatementContext {
  public:
    PrintContext(StatementContext *ctx);

    ExpressionContext *expression();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ReturnContext : public StatementContext {
  public:
    ReturnContext(StatementContext *ctx);

    ExpressionContext *expression();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InvocationContext : public StatementContext {
  public:
    InvocationContext(StatementContext *ctx);

    antlr4::tree::TerminalNode *ID();
    ArgumentsContext *arguments();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  PrintLnContext : public StatementContext {
  public:
    PrintLnContext(StatementContext *ctx);

    ExpressionContext *expression();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ConditionalContext : public StatementContext {
  public:
    ConditionalContext(StatementContext *ctx);

    MiniParser::BlockContext *thenBlock = nullptr;
    MiniParser::BlockContext *elseBlock = nullptr;
    ExpressionContext *expression();
    std::vector<BlockContext *> block();
    BlockContext* block(size_t i);
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NestedBlockContext : public StatementContext {
  public:
    NestedBlockContext(StatementContext *ctx);

    BlockContext *block();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  WhileContext : public StatementContext {
  public:
    WhileContext(StatementContext *ctx);

    ExpressionContext *expression();
    BlockContext *block();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  StatementContext* statement();

  class  BlockContext : public antlr4::ParserRuleContext {
  public:
    BlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StatementListContext *statementList();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlockContext* block();

  class  StatementListContext : public antlr4::ParserRuleContext {
  public:
    StatementListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementListContext* statementList();

  class  LvalueContext : public antlr4::ParserRuleContext {
  public:
    LvalueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    LvalueContext() = default;
    void copyFrom(LvalueContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  LvalueIdContext : public LvalueContext {
  public:
    LvalueIdContext(LvalueContext *ctx);

    antlr4::tree::TerminalNode *ID();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  LvalueDotContext : public LvalueContext {
  public:
    LvalueDotContext(LvalueContext *ctx);

    LvalueContext *lvalue();
    antlr4::tree::TerminalNode *ID();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  LvalueContext* lvalue();
  LvalueContext* lvalue(int precedence);
  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExpressionContext() = default;
    void copyFrom(ExpressionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  IntegerExprContext : public ExpressionContext {
  public:
    IntegerExprContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *INTEGER();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  TrueExprContext : public ExpressionContext {
  public:
    TrueExprContext(ExpressionContext *ctx);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  IdentifierExprContext : public ExpressionContext {
  public:
    IdentifierExprContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *ID();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BinaryExprContext : public ExpressionContext {
  public:
    BinaryExprContext(ExpressionContext *ctx);

    MiniParser::ExpressionContext *lft = nullptr;
    antlr4::Token *op = nullptr;
    MiniParser::ExpressionContext *rht = nullptr;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NewExprContext : public ExpressionContext {
  public:
    NewExprContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *ID();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NestedExprContext : public ExpressionContext {
  public:
    NestedExprContext(ExpressionContext *ctx);

    ExpressionContext *expression();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  DotExprContext : public ExpressionContext {
  public:
    DotExprContext(ExpressionContext *ctx);

    ExpressionContext *expression();
    antlr4::tree::TerminalNode *ID();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  UnaryExprContext : public ExpressionContext {
  public:
    UnaryExprContext(ExpressionContext *ctx);

    antlr4::Token *op = nullptr;
    ExpressionContext *expression();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InvocationExprContext : public ExpressionContext {
  public:
    InvocationExprContext(ExpressionContext *ctx);

    antlr4::tree::TerminalNode *ID();
    ArgumentsContext *arguments();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  FalseExprContext : public ExpressionContext {
  public:
    FalseExprContext(ExpressionContext *ctx);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  NullExprContext : public ExpressionContext {
  public:
    NullExprContext(ExpressionContext *ctx);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExpressionContext* expression();
  ExpressionContext* expression(int precedence);
  class  ArgumentsContext : public antlr4::ParserRuleContext {
  public:
    ArgumentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpressionContext *> expression();
    ExpressionContext* expression(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgumentsContext* arguments();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool lvalueSempred(LvalueContext *_localctx, size_t predicateIndex);
  bool expressionSempred(ExpressionContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

