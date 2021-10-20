
   /* package declaration here */


// Generated from Mini.g4 by ANTLR 4.7.2


#include "MiniVisitor.h"

#include "MiniParser.h"


using namespace antlrcpp;
using namespace antlr4;

MiniParser::MiniParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

MiniParser::~MiniParser() {
  delete _interpreter;
}

std::string MiniParser::getGrammarFileName() const {
  return "Mini.g4";
}

const std::vector<std::string>& MiniParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& MiniParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

MiniParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniParser::TypesContext* MiniParser::ProgramContext::types() {
  return getRuleContext<MiniParser::TypesContext>(0);
}

MiniParser::DeclarationsContext* MiniParser::ProgramContext::declarations() {
  return getRuleContext<MiniParser::DeclarationsContext>(0);
}

MiniParser::FunctionsContext* MiniParser::ProgramContext::functions() {
  return getRuleContext<MiniParser::FunctionsContext>(0);
}

tree::TerminalNode* MiniParser::ProgramContext::EOF() {
  return getToken(MiniParser::EOF, 0);
}


size_t MiniParser::ProgramContext::getRuleIndex() const {
  return MiniParser::RuleProgram;
}

antlrcpp::Any MiniParser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::ProgramContext* MiniParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, MiniParser::RuleProgram);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(36);
    types();
    setState(37);
    declarations();
    setState(38);
    functions();
    setState(39);
    match(MiniParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypesContext ------------------------------------------------------------------

MiniParser::TypesContext::TypesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniParser::TypeDeclarationContext *> MiniParser::TypesContext::typeDeclaration() {
  return getRuleContexts<MiniParser::TypeDeclarationContext>();
}

MiniParser::TypeDeclarationContext* MiniParser::TypesContext::typeDeclaration(size_t i) {
  return getRuleContext<MiniParser::TypeDeclarationContext>(i);
}


size_t MiniParser::TypesContext::getRuleIndex() const {
  return MiniParser::RuleTypes;
}

antlrcpp::Any MiniParser::TypesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitTypes(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::TypesContext* MiniParser::types() {
  TypesContext *_localctx = _tracker.createInstance<TypesContext>(_ctx, getState());
  enterRule(_localctx, 2, MiniParser::RuleTypes);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    setState(48);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(44);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
      while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
        if (alt == 1) {
          setState(41);
          typeDeclaration(); 
        }
        setState(46);
        _errHandler->sync(this);
        alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);

      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeDeclarationContext ------------------------------------------------------------------

MiniParser::TypeDeclarationContext::TypeDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniParser::TypeDeclarationContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::NestedDeclContext* MiniParser::TypeDeclarationContext::nestedDecl() {
  return getRuleContext<MiniParser::NestedDeclContext>(0);
}


size_t MiniParser::TypeDeclarationContext::getRuleIndex() const {
  return MiniParser::RuleTypeDeclaration;
}

antlrcpp::Any MiniParser::TypeDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitTypeDeclaration(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::TypeDeclarationContext* MiniParser::typeDeclaration() {
  TypeDeclarationContext *_localctx = _tracker.createInstance<TypeDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 4, MiniParser::RuleTypeDeclaration);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(50);
    match(MiniParser::T__0);
    setState(51);
    match(MiniParser::ID);
    setState(52);
    match(MiniParser::T__1);
    setState(53);
    nestedDecl();
    setState(54);
    match(MiniParser::T__2);
    setState(55);
    match(MiniParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NestedDeclContext ------------------------------------------------------------------

MiniParser::NestedDeclContext::NestedDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniParser::DeclContext *> MiniParser::NestedDeclContext::decl() {
  return getRuleContexts<MiniParser::DeclContext>();
}

MiniParser::DeclContext* MiniParser::NestedDeclContext::decl(size_t i) {
  return getRuleContext<MiniParser::DeclContext>(i);
}


size_t MiniParser::NestedDeclContext::getRuleIndex() const {
  return MiniParser::RuleNestedDecl;
}

antlrcpp::Any MiniParser::NestedDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitNestedDecl(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::NestedDeclContext* MiniParser::nestedDecl() {
  NestedDeclContext *_localctx = _tracker.createInstance<NestedDeclContext>(_ctx, getState());
  enterRule(_localctx, 6, MiniParser::RuleNestedDecl);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(60); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(57);
      decl();
      setState(58);
      match(MiniParser::T__3);
      setState(62); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MiniParser::T__0)
      | (1ULL << MiniParser::T__4)
      | (1ULL << MiniParser::T__5))) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclContext ------------------------------------------------------------------

MiniParser::DeclContext::DeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniParser::TypeContext* MiniParser::DeclContext::type() {
  return getRuleContext<MiniParser::TypeContext>(0);
}

tree::TerminalNode* MiniParser::DeclContext::ID() {
  return getToken(MiniParser::ID, 0);
}


size_t MiniParser::DeclContext::getRuleIndex() const {
  return MiniParser::RuleDecl;
}

antlrcpp::Any MiniParser::DeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitDecl(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::DeclContext* MiniParser::decl() {
  DeclContext *_localctx = _tracker.createInstance<DeclContext>(_ctx, getState());
  enterRule(_localctx, 8, MiniParser::RuleDecl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(64);
    type();
    setState(65);
    match(MiniParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeContext ------------------------------------------------------------------

MiniParser::TypeContext::TypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniParser::TypeContext::getRuleIndex() const {
  return MiniParser::RuleType;
}

void MiniParser::TypeContext::copyFrom(TypeContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- BoolTypeContext ------------------------------------------------------------------

MiniParser::BoolTypeContext::BoolTypeContext(TypeContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::BoolTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitBoolType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- StructTypeContext ------------------------------------------------------------------

tree::TerminalNode* MiniParser::StructTypeContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::StructTypeContext::StructTypeContext(TypeContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::StructTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitStructType(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IntTypeContext ------------------------------------------------------------------

MiniParser::IntTypeContext::IntTypeContext(TypeContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::IntTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitIntType(this);
  else
    return visitor->visitChildren(this);
}
MiniParser::TypeContext* MiniParser::type() {
  TypeContext *_localctx = _tracker.createInstance<TypeContext>(_ctx, getState());
  enterRule(_localctx, 10, MiniParser::RuleType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(71);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniParser::T__4: {
        _localctx = dynamic_cast<TypeContext *>(_tracker.createInstance<MiniParser::IntTypeContext>(_localctx));
        enterOuterAlt(_localctx, 1);
        setState(67);
        match(MiniParser::T__4);
        break;
      }

      case MiniParser::T__5: {
        _localctx = dynamic_cast<TypeContext *>(_tracker.createInstance<MiniParser::BoolTypeContext>(_localctx));
        enterOuterAlt(_localctx, 2);
        setState(68);
        match(MiniParser::T__5);
        break;
      }

      case MiniParser::T__0: {
        _localctx = dynamic_cast<TypeContext *>(_tracker.createInstance<MiniParser::StructTypeContext>(_localctx));
        enterOuterAlt(_localctx, 3);
        setState(69);
        match(MiniParser::T__0);
        setState(70);
        match(MiniParser::ID);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationsContext ------------------------------------------------------------------

MiniParser::DeclarationsContext::DeclarationsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniParser::DeclarationContext *> MiniParser::DeclarationsContext::declaration() {
  return getRuleContexts<MiniParser::DeclarationContext>();
}

MiniParser::DeclarationContext* MiniParser::DeclarationsContext::declaration(size_t i) {
  return getRuleContext<MiniParser::DeclarationContext>(i);
}


size_t MiniParser::DeclarationsContext::getRuleIndex() const {
  return MiniParser::RuleDeclarations;
}

antlrcpp::Any MiniParser::DeclarationsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitDeclarations(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::DeclarationsContext* MiniParser::declarations() {
  DeclarationsContext *_localctx = _tracker.createInstance<DeclarationsContext>(_ctx, getState());
  enterRule(_localctx, 12, MiniParser::RuleDeclarations);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(76);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MiniParser::T__0)
      | (1ULL << MiniParser::T__4)
      | (1ULL << MiniParser::T__5))) != 0)) {
      setState(73);
      declaration();
      setState(78);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

MiniParser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniParser::TypeContext* MiniParser::DeclarationContext::type() {
  return getRuleContext<MiniParser::TypeContext>(0);
}

std::vector<tree::TerminalNode *> MiniParser::DeclarationContext::ID() {
  return getTokens(MiniParser::ID);
}

tree::TerminalNode* MiniParser::DeclarationContext::ID(size_t i) {
  return getToken(MiniParser::ID, i);
}


size_t MiniParser::DeclarationContext::getRuleIndex() const {
  return MiniParser::RuleDeclaration;
}

antlrcpp::Any MiniParser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::DeclarationContext* MiniParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 14, MiniParser::RuleDeclaration);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(79);
    type();
    setState(80);
    match(MiniParser::ID);
    setState(85);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniParser::T__6) {
      setState(81);
      match(MiniParser::T__6);
      setState(82);
      match(MiniParser::ID);
      setState(87);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(88);
    match(MiniParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionsContext ------------------------------------------------------------------

MiniParser::FunctionsContext::FunctionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniParser::FunctionContext *> MiniParser::FunctionsContext::function() {
  return getRuleContexts<MiniParser::FunctionContext>();
}

MiniParser::FunctionContext* MiniParser::FunctionsContext::function(size_t i) {
  return getRuleContext<MiniParser::FunctionContext>(i);
}


size_t MiniParser::FunctionsContext::getRuleIndex() const {
  return MiniParser::RuleFunctions;
}

antlrcpp::Any MiniParser::FunctionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitFunctions(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::FunctionsContext* MiniParser::functions() {
  FunctionsContext *_localctx = _tracker.createInstance<FunctionsContext>(_ctx, getState());
  enterRule(_localctx, 16, MiniParser::RuleFunctions);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(93);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniParser::T__7) {
      setState(90);
      function();
      setState(95);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

MiniParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniParser::FunctionContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::ParametersContext* MiniParser::FunctionContext::parameters() {
  return getRuleContext<MiniParser::ParametersContext>(0);
}

MiniParser::ReturnTypeContext* MiniParser::FunctionContext::returnType() {
  return getRuleContext<MiniParser::ReturnTypeContext>(0);
}

MiniParser::DeclarationsContext* MiniParser::FunctionContext::declarations() {
  return getRuleContext<MiniParser::DeclarationsContext>(0);
}

MiniParser::StatementListContext* MiniParser::FunctionContext::statementList() {
  return getRuleContext<MiniParser::StatementListContext>(0);
}


size_t MiniParser::FunctionContext::getRuleIndex() const {
  return MiniParser::RuleFunction;
}

antlrcpp::Any MiniParser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::FunctionContext* MiniParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 18, MiniParser::RuleFunction);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(96);
    match(MiniParser::T__7);
    setState(97);
    match(MiniParser::ID);
    setState(98);
    parameters();
    setState(99);
    returnType();
    setState(100);
    match(MiniParser::T__1);
    setState(101);
    declarations();
    setState(102);
    statementList();
    setState(103);
    match(MiniParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParametersContext ------------------------------------------------------------------

MiniParser::ParametersContext::ParametersContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniParser::DeclContext *> MiniParser::ParametersContext::decl() {
  return getRuleContexts<MiniParser::DeclContext>();
}

MiniParser::DeclContext* MiniParser::ParametersContext::decl(size_t i) {
  return getRuleContext<MiniParser::DeclContext>(i);
}


size_t MiniParser::ParametersContext::getRuleIndex() const {
  return MiniParser::RuleParameters;
}

antlrcpp::Any MiniParser::ParametersContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitParameters(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::ParametersContext* MiniParser::parameters() {
  ParametersContext *_localctx = _tracker.createInstance<ParametersContext>(_ctx, getState());
  enterRule(_localctx, 20, MiniParser::RuleParameters);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(105);
    match(MiniParser::T__8);
    setState(114);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MiniParser::T__0)
      | (1ULL << MiniParser::T__4)
      | (1ULL << MiniParser::T__5))) != 0)) {
      setState(106);
      decl();
      setState(111);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MiniParser::T__6) {
        setState(107);
        match(MiniParser::T__6);
        setState(108);
        decl();
        setState(113);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
    setState(116);
    match(MiniParser::T__9);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnTypeContext ------------------------------------------------------------------

MiniParser::ReturnTypeContext::ReturnTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniParser::ReturnTypeContext::getRuleIndex() const {
  return MiniParser::RuleReturnType;
}

void MiniParser::ReturnTypeContext::copyFrom(ReturnTypeContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ReturnTypeVoidContext ------------------------------------------------------------------

MiniParser::ReturnTypeVoidContext::ReturnTypeVoidContext(ReturnTypeContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::ReturnTypeVoidContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitReturnTypeVoid(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnTypeRealContext ------------------------------------------------------------------

MiniParser::TypeContext* MiniParser::ReturnTypeRealContext::type() {
  return getRuleContext<MiniParser::TypeContext>(0);
}

MiniParser::ReturnTypeRealContext::ReturnTypeRealContext(ReturnTypeContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::ReturnTypeRealContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitReturnTypeReal(this);
  else
    return visitor->visitChildren(this);
}
MiniParser::ReturnTypeContext* MiniParser::returnType() {
  ReturnTypeContext *_localctx = _tracker.createInstance<ReturnTypeContext>(_ctx, getState());
  enterRule(_localctx, 22, MiniParser::RuleReturnType);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(120);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniParser::T__0:
      case MiniParser::T__4:
      case MiniParser::T__5: {
        _localctx = dynamic_cast<ReturnTypeContext *>(_tracker.createInstance<MiniParser::ReturnTypeRealContext>(_localctx));
        enterOuterAlt(_localctx, 1);
        setState(118);
        type();
        break;
      }

      case MiniParser::T__10: {
        _localctx = dynamic_cast<ReturnTypeContext *>(_tracker.createInstance<MiniParser::ReturnTypeVoidContext>(_localctx));
        enterOuterAlt(_localctx, 2);
        setState(119);
        match(MiniParser::T__10);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

MiniParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniParser::StatementContext::getRuleIndex() const {
  return MiniParser::RuleStatement;
}

void MiniParser::StatementContext::copyFrom(StatementContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- AssignmentContext ------------------------------------------------------------------

MiniParser::LvalueContext* MiniParser::AssignmentContext::lvalue() {
  return getRuleContext<MiniParser::LvalueContext>(0);
}

MiniParser::ExpressionContext* MiniParser::AssignmentContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

MiniParser::AssignmentContext::AssignmentContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DeleteContext ------------------------------------------------------------------

MiniParser::ExpressionContext* MiniParser::DeleteContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

MiniParser::DeleteContext::DeleteContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::DeleteContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitDelete(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PrintContext ------------------------------------------------------------------

MiniParser::ExpressionContext* MiniParser::PrintContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

MiniParser::PrintContext::PrintContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::PrintContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitPrint(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnContext ------------------------------------------------------------------

MiniParser::ExpressionContext* MiniParser::ReturnContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

MiniParser::ReturnContext::ReturnContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::ReturnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitReturn(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InvocationContext ------------------------------------------------------------------

tree::TerminalNode* MiniParser::InvocationContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::ArgumentsContext* MiniParser::InvocationContext::arguments() {
  return getRuleContext<MiniParser::ArgumentsContext>(0);
}

MiniParser::InvocationContext::InvocationContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::InvocationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitInvocation(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PrintLnContext ------------------------------------------------------------------

MiniParser::ExpressionContext* MiniParser::PrintLnContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

MiniParser::PrintLnContext::PrintLnContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::PrintLnContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitPrintLn(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ConditionalContext ------------------------------------------------------------------

MiniParser::ExpressionContext* MiniParser::ConditionalContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

std::vector<MiniParser::BlockContext *> MiniParser::ConditionalContext::block() {
  return getRuleContexts<MiniParser::BlockContext>();
}

MiniParser::BlockContext* MiniParser::ConditionalContext::block(size_t i) {
  return getRuleContext<MiniParser::BlockContext>(i);
}

MiniParser::ConditionalContext::ConditionalContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::ConditionalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitConditional(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NestedBlockContext ------------------------------------------------------------------

MiniParser::BlockContext* MiniParser::NestedBlockContext::block() {
  return getRuleContext<MiniParser::BlockContext>(0);
}

MiniParser::NestedBlockContext::NestedBlockContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::NestedBlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitNestedBlock(this);
  else
    return visitor->visitChildren(this);
}
//----------------- WhileContext ------------------------------------------------------------------

MiniParser::ExpressionContext* MiniParser::WhileContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

MiniParser::BlockContext* MiniParser::WhileContext::block() {
  return getRuleContext<MiniParser::BlockContext>(0);
}

MiniParser::WhileContext::WhileContext(StatementContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::WhileContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitWhile(this);
  else
    return visitor->visitChildren(this);
}
MiniParser::StatementContext* MiniParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 24, MiniParser::RuleStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(170);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<MiniParser::NestedBlockContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(122);
      block();
      break;
    }

    case 2: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<MiniParser::AssignmentContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(123);
      lvalue(0);
      setState(124);
      match(MiniParser::T__11);
      setState(127);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case MiniParser::T__8:
        case MiniParser::T__21:
        case MiniParser::T__22:
        case MiniParser::T__34:
        case MiniParser::T__35:
        case MiniParser::T__36:
        case MiniParser::T__37:
        case MiniParser::ID:
        case MiniParser::INTEGER: {
          setState(125);
          expression(0);
          break;
        }

        case MiniParser::T__12: {
          setState(126);
          match(MiniParser::T__12);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(129);
      match(MiniParser::T__3);
      break;
    }

    case 3: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<MiniParser::PrintContext>(_localctx));
      enterOuterAlt(_localctx, 3);
      setState(131);
      match(MiniParser::T__13);
      setState(132);
      expression(0);
      setState(133);
      match(MiniParser::T__3);
      break;
    }

    case 4: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<MiniParser::PrintLnContext>(_localctx));
      enterOuterAlt(_localctx, 4);
      setState(135);
      match(MiniParser::T__13);
      setState(136);
      expression(0);
      setState(137);
      match(MiniParser::T__14);
      setState(138);
      match(MiniParser::T__3);
      break;
    }

    case 5: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<MiniParser::ConditionalContext>(_localctx));
      enterOuterAlt(_localctx, 5);
      setState(140);
      match(MiniParser::T__15);
      setState(141);
      match(MiniParser::T__8);
      setState(142);
      expression(0);
      setState(143);
      match(MiniParser::T__9);
      setState(144);
      dynamic_cast<ConditionalContext *>(_localctx)->thenBlock = block();
      setState(147);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == MiniParser::T__16) {
        setState(145);
        match(MiniParser::T__16);
        setState(146);
        dynamic_cast<ConditionalContext *>(_localctx)->elseBlock = block();
      }
      break;
    }

    case 6: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<MiniParser::WhileContext>(_localctx));
      enterOuterAlt(_localctx, 6);
      setState(149);
      match(MiniParser::T__17);
      setState(150);
      match(MiniParser::T__8);
      setState(151);
      expression(0);
      setState(152);
      match(MiniParser::T__9);
      setState(153);
      block();
      break;
    }

    case 7: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<MiniParser::DeleteContext>(_localctx));
      enterOuterAlt(_localctx, 7);
      setState(155);
      match(MiniParser::T__18);
      setState(156);
      expression(0);
      setState(157);
      match(MiniParser::T__3);
      break;
    }

    case 8: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<MiniParser::ReturnContext>(_localctx));
      enterOuterAlt(_localctx, 8);
      setState(159);
      match(MiniParser::T__19);
      setState(161);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << MiniParser::T__8)
        | (1ULL << MiniParser::T__21)
        | (1ULL << MiniParser::T__22)
        | (1ULL << MiniParser::T__34)
        | (1ULL << MiniParser::T__35)
        | (1ULL << MiniParser::T__36)
        | (1ULL << MiniParser::T__37)
        | (1ULL << MiniParser::ID)
        | (1ULL << MiniParser::INTEGER))) != 0)) {
        setState(160);
        expression(0);
      }
      setState(163);
      match(MiniParser::T__3);
      break;
    }

    case 9: {
      _localctx = dynamic_cast<StatementContext *>(_tracker.createInstance<MiniParser::InvocationContext>(_localctx));
      enterOuterAlt(_localctx, 9);
      setState(164);
      match(MiniParser::ID);
      setState(165);
      match(MiniParser::T__8);
      setState(166);
      arguments();
      setState(167);
      match(MiniParser::T__9);
      setState(168);
      match(MiniParser::T__3);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

MiniParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniParser::StatementListContext* MiniParser::BlockContext::statementList() {
  return getRuleContext<MiniParser::StatementListContext>(0);
}


size_t MiniParser::BlockContext::getRuleIndex() const {
  return MiniParser::RuleBlock;
}

antlrcpp::Any MiniParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::BlockContext* MiniParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 26, MiniParser::RuleBlock);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(172);
    match(MiniParser::T__1);
    setState(173);
    statementList();
    setState(174);
    match(MiniParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementListContext ------------------------------------------------------------------

MiniParser::StatementListContext::StatementListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniParser::StatementContext *> MiniParser::StatementListContext::statement() {
  return getRuleContexts<MiniParser::StatementContext>();
}

MiniParser::StatementContext* MiniParser::StatementListContext::statement(size_t i) {
  return getRuleContext<MiniParser::StatementContext>(i);
}


size_t MiniParser::StatementListContext::getRuleIndex() const {
  return MiniParser::RuleStatementList;
}

antlrcpp::Any MiniParser::StatementListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitStatementList(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::StatementListContext* MiniParser::statementList() {
  StatementListContext *_localctx = _tracker.createInstance<StatementListContext>(_ctx, getState());
  enterRule(_localctx, 28, MiniParser::RuleStatementList);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(179);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MiniParser::T__1)
      | (1ULL << MiniParser::T__13)
      | (1ULL << MiniParser::T__15)
      | (1ULL << MiniParser::T__17)
      | (1ULL << MiniParser::T__18)
      | (1ULL << MiniParser::T__19)
      | (1ULL << MiniParser::ID))) != 0)) {
      setState(176);
      statement();
      setState(181);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LvalueContext ------------------------------------------------------------------

MiniParser::LvalueContext::LvalueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniParser::LvalueContext::getRuleIndex() const {
  return MiniParser::RuleLvalue;
}

void MiniParser::LvalueContext::copyFrom(LvalueContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- LvalueIdContext ------------------------------------------------------------------

tree::TerminalNode* MiniParser::LvalueIdContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::LvalueIdContext::LvalueIdContext(LvalueContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::LvalueIdContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitLvalueId(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LvalueDotContext ------------------------------------------------------------------

MiniParser::LvalueContext* MiniParser::LvalueDotContext::lvalue() {
  return getRuleContext<MiniParser::LvalueContext>(0);
}

tree::TerminalNode* MiniParser::LvalueDotContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::LvalueDotContext::LvalueDotContext(LvalueContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::LvalueDotContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitLvalueDot(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::LvalueContext* MiniParser::lvalue() {
   return lvalue(0);
}

MiniParser::LvalueContext* MiniParser::lvalue(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  MiniParser::LvalueContext *_localctx = _tracker.createInstance<LvalueContext>(_ctx, parentState);
  MiniParser::LvalueContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 30;
  enterRecursionRule(_localctx, 30, MiniParser::RuleLvalue, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    _localctx = _tracker.createInstance<LvalueIdContext>(_localctx);
    _ctx = _localctx;
    previousContext = _localctx;

    setState(183);
    match(MiniParser::ID);
    _ctx->stop = _input->LT(-1);
    setState(190);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        auto newContext = _tracker.createInstance<LvalueDotContext>(_tracker.createInstance<LvalueContext>(parentContext, parentState));
        _localctx = newContext;
        pushNewRecursionContext(newContext, startState, RuleLvalue);
        setState(185);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(186);
        match(MiniParser::T__20);
        setState(187);
        match(MiniParser::ID); 
      }
      setState(192);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

MiniParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniParser::ExpressionContext::getRuleIndex() const {
  return MiniParser::RuleExpression;
}

void MiniParser::ExpressionContext::copyFrom(ExpressionContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- IntegerExprContext ------------------------------------------------------------------

tree::TerminalNode* MiniParser::IntegerExprContext::INTEGER() {
  return getToken(MiniParser::INTEGER, 0);
}

MiniParser::IntegerExprContext::IntegerExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::IntegerExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitIntegerExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TrueExprContext ------------------------------------------------------------------

MiniParser::TrueExprContext::TrueExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::TrueExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitTrueExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IdentifierExprContext ------------------------------------------------------------------

tree::TerminalNode* MiniParser::IdentifierExprContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::IdentifierExprContext::IdentifierExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::IdentifierExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitIdentifierExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BinaryExprContext ------------------------------------------------------------------

std::vector<MiniParser::ExpressionContext *> MiniParser::BinaryExprContext::expression() {
  return getRuleContexts<MiniParser::ExpressionContext>();
}

MiniParser::ExpressionContext* MiniParser::BinaryExprContext::expression(size_t i) {
  return getRuleContext<MiniParser::ExpressionContext>(i);
}

MiniParser::BinaryExprContext::BinaryExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::BinaryExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitBinaryExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NewExprContext ------------------------------------------------------------------

tree::TerminalNode* MiniParser::NewExprContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::NewExprContext::NewExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::NewExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitNewExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NestedExprContext ------------------------------------------------------------------

MiniParser::ExpressionContext* MiniParser::NestedExprContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

MiniParser::NestedExprContext::NestedExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::NestedExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitNestedExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DotExprContext ------------------------------------------------------------------

MiniParser::ExpressionContext* MiniParser::DotExprContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

tree::TerminalNode* MiniParser::DotExprContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::DotExprContext::DotExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::DotExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitDotExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UnaryExprContext ------------------------------------------------------------------

MiniParser::ExpressionContext* MiniParser::UnaryExprContext::expression() {
  return getRuleContext<MiniParser::ExpressionContext>(0);
}

MiniParser::UnaryExprContext::UnaryExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::UnaryExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitUnaryExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InvocationExprContext ------------------------------------------------------------------

tree::TerminalNode* MiniParser::InvocationExprContext::ID() {
  return getToken(MiniParser::ID, 0);
}

MiniParser::ArgumentsContext* MiniParser::InvocationExprContext::arguments() {
  return getRuleContext<MiniParser::ArgumentsContext>(0);
}

MiniParser::InvocationExprContext::InvocationExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::InvocationExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitInvocationExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FalseExprContext ------------------------------------------------------------------

MiniParser::FalseExprContext::FalseExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::FalseExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitFalseExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NullExprContext ------------------------------------------------------------------

MiniParser::NullExprContext::NullExprContext(ExpressionContext *ctx) { copyFrom(ctx); }

antlrcpp::Any MiniParser::NullExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitNullExpr(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::ExpressionContext* MiniParser::expression() {
   return expression(0);
}

MiniParser::ExpressionContext* MiniParser::expression(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  MiniParser::ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, parentState);
  MiniParser::ExpressionContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 32;
  enterRecursionRule(_localctx, 32, MiniParser::RuleExpression, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(212);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<InvocationExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(194);
      match(MiniParser::ID);
      setState(195);
      match(MiniParser::T__8);
      setState(196);
      arguments();
      setState(197);
      match(MiniParser::T__9);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<UnaryExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(199);
      dynamic_cast<UnaryExprContext *>(_localctx)->op = _input->LT(1);
      _la = _input->LA(1);
      if (!(_la == MiniParser::T__21

      || _la == MiniParser::T__22)) {
        dynamic_cast<UnaryExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(200);
      expression(14);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<IdentifierExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(201);
      match(MiniParser::ID);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<IntegerExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(202);
      match(MiniParser::INTEGER);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<TrueExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(203);
      match(MiniParser::T__34);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<FalseExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(204);
      match(MiniParser::T__35);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<NewExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(205);
      match(MiniParser::T__36);
      setState(206);
      match(MiniParser::ID);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<NullExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(207);
      match(MiniParser::T__37);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<NestedExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(208);
      match(MiniParser::T__8);
      setState(209);
      expression(0);
      setState(210);
      match(MiniParser::T__9);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(237);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(235);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<BinaryExprContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lft = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(214);

          if (!(precpred(_ctx, 13))) throw FailedPredicateException(this, "precpred(_ctx, 13)");
          setState(215);
          dynamic_cast<BinaryExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == MiniParser::T__23

          || _la == MiniParser::T__24)) {
            dynamic_cast<BinaryExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(216);
          dynamic_cast<BinaryExprContext *>(_localctx)->rht = expression(14);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<BinaryExprContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lft = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(217);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(218);
          dynamic_cast<BinaryExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == MiniParser::T__21

          || _la == MiniParser::T__25)) {
            dynamic_cast<BinaryExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(219);
          dynamic_cast<BinaryExprContext *>(_localctx)->rht = expression(13);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<BinaryExprContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lft = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(220);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(221);
          dynamic_cast<BinaryExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << MiniParser::T__26)
            | (1ULL << MiniParser::T__27)
            | (1ULL << MiniParser::T__28)
            | (1ULL << MiniParser::T__29))) != 0))) {
            dynamic_cast<BinaryExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(222);
          dynamic_cast<BinaryExprContext *>(_localctx)->rht = expression(12);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<BinaryExprContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lft = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(223);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(224);
          dynamic_cast<BinaryExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == MiniParser::T__30

          || _la == MiniParser::T__31)) {
            dynamic_cast<BinaryExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(225);
          dynamic_cast<BinaryExprContext *>(_localctx)->rht = expression(11);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<BinaryExprContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lft = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(226);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(227);
          dynamic_cast<BinaryExprContext *>(_localctx)->op = match(MiniParser::T__32);
          setState(228);
          dynamic_cast<BinaryExprContext *>(_localctx)->rht = expression(10);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<BinaryExprContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lft = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(229);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(230);
          dynamic_cast<BinaryExprContext *>(_localctx)->op = match(MiniParser::T__33);
          setState(231);
          dynamic_cast<BinaryExprContext *>(_localctx)->rht = expression(9);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<DotExprContext>(_tracker.createInstance<ExpressionContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpression);
          setState(232);

          if (!(precpred(_ctx, 15))) throw FailedPredicateException(this, "precpred(_ctx, 15)");

          setState(233);
          match(MiniParser::T__20);
          setState(234);
          match(MiniParser::ID);
          break;
        }

        } 
      }
      setState(239);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ArgumentsContext ------------------------------------------------------------------

MiniParser::ArgumentsContext::ArgumentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniParser::ExpressionContext *> MiniParser::ArgumentsContext::expression() {
  return getRuleContexts<MiniParser::ExpressionContext>();
}

MiniParser::ExpressionContext* MiniParser::ArgumentsContext::expression(size_t i) {
  return getRuleContext<MiniParser::ExpressionContext>(i);
}


size_t MiniParser::ArgumentsContext::getRuleIndex() const {
  return MiniParser::RuleArguments;
}

antlrcpp::Any MiniParser::ArgumentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniVisitor*>(visitor))
    return parserVisitor->visitArguments(this);
  else
    return visitor->visitChildren(this);
}

MiniParser::ArgumentsContext* MiniParser::arguments() {
  ArgumentsContext *_localctx = _tracker.createInstance<ArgumentsContext>(_ctx, getState());
  enterRule(_localctx, 34, MiniParser::RuleArguments);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(248);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << MiniParser::T__8)
      | (1ULL << MiniParser::T__21)
      | (1ULL << MiniParser::T__22)
      | (1ULL << MiniParser::T__34)
      | (1ULL << MiniParser::T__35)
      | (1ULL << MiniParser::T__36)
      | (1ULL << MiniParser::T__37)
      | (1ULL << MiniParser::ID)
      | (1ULL << MiniParser::INTEGER))) != 0)) {
      setState(240);
      expression(0);
      setState(245);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == MiniParser::T__6) {
        setState(241);
        match(MiniParser::T__6);
        setState(242);
        expression(0);
        setState(247);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool MiniParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 15: return lvalueSempred(dynamic_cast<LvalueContext *>(context), predicateIndex);
    case 16: return expressionSempred(dynamic_cast<ExpressionContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool MiniParser::lvalueSempred(LvalueContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

bool MiniParser::expressionSempred(ExpressionContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 1: return precpred(_ctx, 13);
    case 2: return precpred(_ctx, 12);
    case 3: return precpred(_ctx, 11);
    case 4: return precpred(_ctx, 10);
    case 5: return precpred(_ctx, 9);
    case 6: return precpred(_ctx, 8);
    case 7: return precpred(_ctx, 15);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> MiniParser::_decisionToDFA;
atn::PredictionContextCache MiniParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN MiniParser::_atn;
std::vector<uint16_t> MiniParser::_serializedATN;

std::vector<std::string> MiniParser::_ruleNames = {
  "program", "types", "typeDeclaration", "nestedDecl", "decl", "type", "declarations", 
  "declaration", "functions", "function", "parameters", "returnType", "statement", 
  "block", "statementList", "lvalue", "expression", "arguments"
};

std::vector<std::string> MiniParser::_literalNames = {
  "", "'struct'", "'{'", "'}'", "';'", "'int'", "'bool'", "','", "'fun'", 
  "'('", "')'", "'void'", "'='", "'read'", "'print'", "'endl'", "'if'", 
  "'else'", "'while'", "'delete'", "'return'", "'.'", "'-'", "'!'", "'*'", 
  "'/'", "'+'", "'<'", "'>'", "'<='", "'>='", "'=='", "'!='", "'&&'", "'||'", 
  "'true'", "'false'", "'new'", "'null'"
};

std::vector<std::string> MiniParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "ID", "INTEGER", "WS", "COMMENT"
};

dfa::Vocabulary MiniParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> MiniParser::_tokenNames;

MiniParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x2c, 0xfd, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 
    0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 0x9, 
    0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 0x4, 
    0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 0x12, 
    0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x3, 0x7, 0x3, 0x2d, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 
    0x30, 0xb, 0x3, 0x3, 0x3, 0x5, 0x3, 0x33, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x6, 0x5, 0x3f, 0xa, 0x5, 0xd, 0x5, 0xe, 0x5, 0x40, 0x3, 0x6, 
    0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 
    0x4a, 0xa, 0x7, 0x3, 0x8, 0x7, 0x8, 0x4d, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 
    0x50, 0xb, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x56, 
    0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x59, 0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0xa, 0x7, 0xa, 0x5e, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 0x61, 0xb, 0xa, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 
    0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0x70, 
    0xa, 0xc, 0xc, 0xc, 0xe, 0xc, 0x73, 0xb, 0xc, 0x5, 0xc, 0x75, 0xa, 0xc, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0x7b, 0xa, 0xd, 0x3, 
    0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x82, 0xa, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0x96, 0xa, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0xa4, 0xa, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
    0x5, 0xe, 0xad, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0x10, 0x7, 0x10, 0xb4, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0xb7, 0xb, 0x10, 
    0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x7, 
    0x11, 0xbf, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0xc2, 0xb, 0x11, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0xd7, 
    0xa, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x3, 0x12, 0x3, 0x12, 0x7, 0x12, 0xee, 0xa, 0x12, 0xc, 0x12, 0xe, 
    0x12, 0xf1, 0xb, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x7, 0x13, 0xf6, 
    0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0xf9, 0xb, 0x13, 0x5, 0x13, 0xfb, 0xa, 
    0x13, 0x3, 0x13, 0x2, 0x4, 0x20, 0x22, 0x14, 0x2, 0x4, 0x6, 0x8, 0xa, 
    0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 
    0x24, 0x2, 0x7, 0x3, 0x2, 0x18, 0x19, 0x3, 0x2, 0x1a, 0x1b, 0x4, 0x2, 
    0x18, 0x18, 0x1c, 0x1c, 0x3, 0x2, 0x1d, 0x20, 0x3, 0x2, 0x21, 0x22, 
    0x2, 0x113, 0x2, 0x26, 0x3, 0x2, 0x2, 0x2, 0x4, 0x32, 0x3, 0x2, 0x2, 
    0x2, 0x6, 0x34, 0x3, 0x2, 0x2, 0x2, 0x8, 0x3e, 0x3, 0x2, 0x2, 0x2, 0xa, 
    0x42, 0x3, 0x2, 0x2, 0x2, 0xc, 0x49, 0x3, 0x2, 0x2, 0x2, 0xe, 0x4e, 
    0x3, 0x2, 0x2, 0x2, 0x10, 0x51, 0x3, 0x2, 0x2, 0x2, 0x12, 0x5f, 0x3, 
    0x2, 0x2, 0x2, 0x14, 0x62, 0x3, 0x2, 0x2, 0x2, 0x16, 0x6b, 0x3, 0x2, 
    0x2, 0x2, 0x18, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x1a, 0xac, 0x3, 0x2, 0x2, 
    0x2, 0x1c, 0xae, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xb5, 0x3, 0x2, 0x2, 0x2, 
    0x20, 0xb8, 0x3, 0x2, 0x2, 0x2, 0x22, 0xd6, 0x3, 0x2, 0x2, 0x2, 0x24, 
    0xfa, 0x3, 0x2, 0x2, 0x2, 0x26, 0x27, 0x5, 0x4, 0x3, 0x2, 0x27, 0x28, 
    0x5, 0xe, 0x8, 0x2, 0x28, 0x29, 0x5, 0x12, 0xa, 0x2, 0x29, 0x2a, 0x7, 
    0x2, 0x2, 0x3, 0x2a, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x2d, 0x5, 0x6, 
    0x4, 0x2, 0x2c, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x30, 0x3, 0x2, 0x2, 
    0x2, 0x2e, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x2f, 0x3, 0x2, 0x2, 0x2, 
    0x2f, 0x33, 0x3, 0x2, 0x2, 0x2, 0x30, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x31, 
    0x33, 0x3, 0x2, 0x2, 0x2, 0x32, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x32, 0x31, 
    0x3, 0x2, 0x2, 0x2, 0x33, 0x5, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 0x7, 
    0x3, 0x2, 0x2, 0x35, 0x36, 0x7, 0x29, 0x2, 0x2, 0x36, 0x37, 0x7, 0x4, 
    0x2, 0x2, 0x37, 0x38, 0x5, 0x8, 0x5, 0x2, 0x38, 0x39, 0x7, 0x5, 0x2, 
    0x2, 0x39, 0x3a, 0x7, 0x6, 0x2, 0x2, 0x3a, 0x7, 0x3, 0x2, 0x2, 0x2, 
    0x3b, 0x3c, 0x5, 0xa, 0x6, 0x2, 0x3c, 0x3d, 0x7, 0x6, 0x2, 0x2, 0x3d, 
    0x3f, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x40, 
    0x3, 0x2, 0x2, 0x2, 0x40, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x40, 0x41, 0x3, 
    0x2, 0x2, 0x2, 0x41, 0x9, 0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x5, 0xc, 
    0x7, 0x2, 0x43, 0x44, 0x7, 0x29, 0x2, 0x2, 0x44, 0xb, 0x3, 0x2, 0x2, 
    0x2, 0x45, 0x4a, 0x7, 0x7, 0x2, 0x2, 0x46, 0x4a, 0x7, 0x8, 0x2, 0x2, 
    0x47, 0x48, 0x7, 0x3, 0x2, 0x2, 0x48, 0x4a, 0x7, 0x29, 0x2, 0x2, 0x49, 
    0x45, 0x3, 0x2, 0x2, 0x2, 0x49, 0x46, 0x3, 0x2, 0x2, 0x2, 0x49, 0x47, 
    0x3, 0x2, 0x2, 0x2, 0x4a, 0xd, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x4d, 0x5, 
    0x10, 0x9, 0x2, 0x4c, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x50, 0x3, 0x2, 
    0x2, 0x2, 0x4e, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x4f, 0x3, 0x2, 0x2, 
    0x2, 0x4f, 0xf, 0x3, 0x2, 0x2, 0x2, 0x50, 0x4e, 0x3, 0x2, 0x2, 0x2, 
    0x51, 0x52, 0x5, 0xc, 0x7, 0x2, 0x52, 0x57, 0x7, 0x29, 0x2, 0x2, 0x53, 
    0x54, 0x7, 0x9, 0x2, 0x2, 0x54, 0x56, 0x7, 0x29, 0x2, 0x2, 0x55, 0x53, 
    0x3, 0x2, 0x2, 0x2, 0x56, 0x59, 0x3, 0x2, 0x2, 0x2, 0x57, 0x55, 0x3, 
    0x2, 0x2, 0x2, 0x57, 0x58, 0x3, 0x2, 0x2, 0x2, 0x58, 0x5a, 0x3, 0x2, 
    0x2, 0x2, 0x59, 0x57, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 0x7, 0x6, 0x2, 
    0x2, 0x5b, 0x11, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5e, 0x5, 0x14, 0xb, 0x2, 
    0x5d, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x61, 0x3, 0x2, 0x2, 0x2, 0x5f, 
    0x5d, 0x3, 0x2, 0x2, 0x2, 0x5f, 0x60, 0x3, 0x2, 0x2, 0x2, 0x60, 0x13, 
    0x3, 0x2, 0x2, 0x2, 0x61, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x62, 0x63, 0x7, 
    0xa, 0x2, 0x2, 0x63, 0x64, 0x7, 0x29, 0x2, 0x2, 0x64, 0x65, 0x5, 0x16, 
    0xc, 0x2, 0x65, 0x66, 0x5, 0x18, 0xd, 0x2, 0x66, 0x67, 0x7, 0x4, 0x2, 
    0x2, 0x67, 0x68, 0x5, 0xe, 0x8, 0x2, 0x68, 0x69, 0x5, 0x1e, 0x10, 0x2, 
    0x69, 0x6a, 0x7, 0x5, 0x2, 0x2, 0x6a, 0x15, 0x3, 0x2, 0x2, 0x2, 0x6b, 
    0x74, 0x7, 0xb, 0x2, 0x2, 0x6c, 0x71, 0x5, 0xa, 0x6, 0x2, 0x6d, 0x6e, 
    0x7, 0x9, 0x2, 0x2, 0x6e, 0x70, 0x5, 0xa, 0x6, 0x2, 0x6f, 0x6d, 0x3, 
    0x2, 0x2, 0x2, 0x70, 0x73, 0x3, 0x2, 0x2, 0x2, 0x71, 0x6f, 0x3, 0x2, 
    0x2, 0x2, 0x71, 0x72, 0x3, 0x2, 0x2, 0x2, 0x72, 0x75, 0x3, 0x2, 0x2, 
    0x2, 0x73, 0x71, 0x3, 0x2, 0x2, 0x2, 0x74, 0x6c, 0x3, 0x2, 0x2, 0x2, 
    0x74, 0x75, 0x3, 0x2, 0x2, 0x2, 0x75, 0x76, 0x3, 0x2, 0x2, 0x2, 0x76, 
    0x77, 0x7, 0xc, 0x2, 0x2, 0x77, 0x17, 0x3, 0x2, 0x2, 0x2, 0x78, 0x7b, 
    0x5, 0xc, 0x7, 0x2, 0x79, 0x7b, 0x7, 0xd, 0x2, 0x2, 0x7a, 0x78, 0x3, 
    0x2, 0x2, 0x2, 0x7a, 0x79, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x19, 0x3, 0x2, 
    0x2, 0x2, 0x7c, 0xad, 0x5, 0x1c, 0xf, 0x2, 0x7d, 0x7e, 0x5, 0x20, 0x11, 
    0x2, 0x7e, 0x81, 0x7, 0xe, 0x2, 0x2, 0x7f, 0x82, 0x5, 0x22, 0x12, 0x2, 
    0x80, 0x82, 0x7, 0xf, 0x2, 0x2, 0x81, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x81, 
    0x80, 0x3, 0x2, 0x2, 0x2, 0x82, 0x83, 0x3, 0x2, 0x2, 0x2, 0x83, 0x84, 
    0x7, 0x6, 0x2, 0x2, 0x84, 0xad, 0x3, 0x2, 0x2, 0x2, 0x85, 0x86, 0x7, 
    0x10, 0x2, 0x2, 0x86, 0x87, 0x5, 0x22, 0x12, 0x2, 0x87, 0x88, 0x7, 0x6, 
    0x2, 0x2, 0x88, 0xad, 0x3, 0x2, 0x2, 0x2, 0x89, 0x8a, 0x7, 0x10, 0x2, 
    0x2, 0x8a, 0x8b, 0x5, 0x22, 0x12, 0x2, 0x8b, 0x8c, 0x7, 0x11, 0x2, 0x2, 
    0x8c, 0x8d, 0x7, 0x6, 0x2, 0x2, 0x8d, 0xad, 0x3, 0x2, 0x2, 0x2, 0x8e, 
    0x8f, 0x7, 0x12, 0x2, 0x2, 0x8f, 0x90, 0x7, 0xb, 0x2, 0x2, 0x90, 0x91, 
    0x5, 0x22, 0x12, 0x2, 0x91, 0x92, 0x7, 0xc, 0x2, 0x2, 0x92, 0x95, 0x5, 
    0x1c, 0xf, 0x2, 0x93, 0x94, 0x7, 0x13, 0x2, 0x2, 0x94, 0x96, 0x5, 0x1c, 
    0xf, 0x2, 0x95, 0x93, 0x3, 0x2, 0x2, 0x2, 0x95, 0x96, 0x3, 0x2, 0x2, 
    0x2, 0x96, 0xad, 0x3, 0x2, 0x2, 0x2, 0x97, 0x98, 0x7, 0x14, 0x2, 0x2, 
    0x98, 0x99, 0x7, 0xb, 0x2, 0x2, 0x99, 0x9a, 0x5, 0x22, 0x12, 0x2, 0x9a, 
    0x9b, 0x7, 0xc, 0x2, 0x2, 0x9b, 0x9c, 0x5, 0x1c, 0xf, 0x2, 0x9c, 0xad, 
    0x3, 0x2, 0x2, 0x2, 0x9d, 0x9e, 0x7, 0x15, 0x2, 0x2, 0x9e, 0x9f, 0x5, 
    0x22, 0x12, 0x2, 0x9f, 0xa0, 0x7, 0x6, 0x2, 0x2, 0xa0, 0xad, 0x3, 0x2, 
    0x2, 0x2, 0xa1, 0xa3, 0x7, 0x16, 0x2, 0x2, 0xa2, 0xa4, 0x5, 0x22, 0x12, 
    0x2, 0xa3, 0xa2, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa4, 0x3, 0x2, 0x2, 0x2, 
    0xa4, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa5, 0xad, 0x7, 0x6, 0x2, 0x2, 0xa6, 
    0xa7, 0x7, 0x29, 0x2, 0x2, 0xa7, 0xa8, 0x7, 0xb, 0x2, 0x2, 0xa8, 0xa9, 
    0x5, 0x24, 0x13, 0x2, 0xa9, 0xaa, 0x7, 0xc, 0x2, 0x2, 0xaa, 0xab, 0x7, 
    0x6, 0x2, 0x2, 0xab, 0xad, 0x3, 0x2, 0x2, 0x2, 0xac, 0x7c, 0x3, 0x2, 
    0x2, 0x2, 0xac, 0x7d, 0x3, 0x2, 0x2, 0x2, 0xac, 0x85, 0x3, 0x2, 0x2, 
    0x2, 0xac, 0x89, 0x3, 0x2, 0x2, 0x2, 0xac, 0x8e, 0x3, 0x2, 0x2, 0x2, 
    0xac, 0x97, 0x3, 0x2, 0x2, 0x2, 0xac, 0x9d, 0x3, 0x2, 0x2, 0x2, 0xac, 
    0xa1, 0x3, 0x2, 0x2, 0x2, 0xac, 0xa6, 0x3, 0x2, 0x2, 0x2, 0xad, 0x1b, 
    0x3, 0x2, 0x2, 0x2, 0xae, 0xaf, 0x7, 0x4, 0x2, 0x2, 0xaf, 0xb0, 0x5, 
    0x1e, 0x10, 0x2, 0xb0, 0xb1, 0x7, 0x5, 0x2, 0x2, 0xb1, 0x1d, 0x3, 0x2, 
    0x2, 0x2, 0xb2, 0xb4, 0x5, 0x1a, 0xe, 0x2, 0xb3, 0xb2, 0x3, 0x2, 0x2, 
    0x2, 0xb4, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xb5, 0xb3, 0x3, 0x2, 0x2, 0x2, 
    0xb5, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xb6, 0x1f, 0x3, 0x2, 0x2, 0x2, 0xb7, 
    0xb5, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb9, 0x8, 0x11, 0x1, 0x2, 0xb9, 0xba, 
    0x7, 0x29, 0x2, 0x2, 0xba, 0xc0, 0x3, 0x2, 0x2, 0x2, 0xbb, 0xbc, 0xc, 
    0x3, 0x2, 0x2, 0xbc, 0xbd, 0x7, 0x17, 0x2, 0x2, 0xbd, 0xbf, 0x7, 0x29, 
    0x2, 0x2, 0xbe, 0xbb, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xc2, 0x3, 0x2, 0x2, 
    0x2, 0xc0, 0xbe, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xc1, 0x3, 0x2, 0x2, 0x2, 
    0xc1, 0x21, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc0, 0x3, 0x2, 0x2, 0x2, 0xc3, 
    0xc4, 0x8, 0x12, 0x1, 0x2, 0xc4, 0xc5, 0x7, 0x29, 0x2, 0x2, 0xc5, 0xc6, 
    0x7, 0xb, 0x2, 0x2, 0xc6, 0xc7, 0x5, 0x24, 0x13, 0x2, 0xc7, 0xc8, 0x7, 
    0xc, 0x2, 0x2, 0xc8, 0xd7, 0x3, 0x2, 0x2, 0x2, 0xc9, 0xca, 0x9, 0x2, 
    0x2, 0x2, 0xca, 0xd7, 0x5, 0x22, 0x12, 0x10, 0xcb, 0xd7, 0x7, 0x29, 
    0x2, 0x2, 0xcc, 0xd7, 0x7, 0x2a, 0x2, 0x2, 0xcd, 0xd7, 0x7, 0x25, 0x2, 
    0x2, 0xce, 0xd7, 0x7, 0x26, 0x2, 0x2, 0xcf, 0xd0, 0x7, 0x27, 0x2, 0x2, 
    0xd0, 0xd7, 0x7, 0x29, 0x2, 0x2, 0xd1, 0xd7, 0x7, 0x28, 0x2, 0x2, 0xd2, 
    0xd3, 0x7, 0xb, 0x2, 0x2, 0xd3, 0xd4, 0x5, 0x22, 0x12, 0x2, 0xd4, 0xd5, 
    0x7, 0xc, 0x2, 0x2, 0xd5, 0xd7, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xc3, 0x3, 
    0x2, 0x2, 0x2, 0xd6, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xcb, 0x3, 0x2, 
    0x2, 0x2, 0xd6, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xcd, 0x3, 0x2, 0x2, 
    0x2, 0xd6, 0xce, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xcf, 0x3, 0x2, 0x2, 0x2, 
    0xd6, 0xd1, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xd2, 0x3, 0x2, 0x2, 0x2, 0xd7, 
    0xef, 0x3, 0x2, 0x2, 0x2, 0xd8, 0xd9, 0xc, 0xf, 0x2, 0x2, 0xd9, 0xda, 
    0x9, 0x3, 0x2, 0x2, 0xda, 0xee, 0x5, 0x22, 0x12, 0x10, 0xdb, 0xdc, 0xc, 
    0xe, 0x2, 0x2, 0xdc, 0xdd, 0x9, 0x4, 0x2, 0x2, 0xdd, 0xee, 0x5, 0x22, 
    0x12, 0xf, 0xde, 0xdf, 0xc, 0xd, 0x2, 0x2, 0xdf, 0xe0, 0x9, 0x5, 0x2, 
    0x2, 0xe0, 0xee, 0x5, 0x22, 0x12, 0xe, 0xe1, 0xe2, 0xc, 0xc, 0x2, 0x2, 
    0xe2, 0xe3, 0x9, 0x6, 0x2, 0x2, 0xe3, 0xee, 0x5, 0x22, 0x12, 0xd, 0xe4, 
    0xe5, 0xc, 0xb, 0x2, 0x2, 0xe5, 0xe6, 0x7, 0x23, 0x2, 0x2, 0xe6, 0xee, 
    0x5, 0x22, 0x12, 0xc, 0xe7, 0xe8, 0xc, 0xa, 0x2, 0x2, 0xe8, 0xe9, 0x7, 
    0x24, 0x2, 0x2, 0xe9, 0xee, 0x5, 0x22, 0x12, 0xb, 0xea, 0xeb, 0xc, 0x11, 
    0x2, 0x2, 0xeb, 0xec, 0x7, 0x17, 0x2, 0x2, 0xec, 0xee, 0x7, 0x29, 0x2, 
    0x2, 0xed, 0xd8, 0x3, 0x2, 0x2, 0x2, 0xed, 0xdb, 0x3, 0x2, 0x2, 0x2, 
    0xed, 0xde, 0x3, 0x2, 0x2, 0x2, 0xed, 0xe1, 0x3, 0x2, 0x2, 0x2, 0xed, 
    0xe4, 0x3, 0x2, 0x2, 0x2, 0xed, 0xe7, 0x3, 0x2, 0x2, 0x2, 0xed, 0xea, 
    0x3, 0x2, 0x2, 0x2, 0xee, 0xf1, 0x3, 0x2, 0x2, 0x2, 0xef, 0xed, 0x3, 
    0x2, 0x2, 0x2, 0xef, 0xf0, 0x3, 0x2, 0x2, 0x2, 0xf0, 0x23, 0x3, 0x2, 
    0x2, 0x2, 0xf1, 0xef, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xf7, 0x5, 0x22, 0x12, 
    0x2, 0xf3, 0xf4, 0x7, 0x9, 0x2, 0x2, 0xf4, 0xf6, 0x5, 0x22, 0x12, 0x2, 
    0xf5, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xf6, 0xf9, 0x3, 0x2, 0x2, 0x2, 0xf7, 
    0xf5, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xf8, 0x3, 0x2, 0x2, 0x2, 0xf8, 0xfb, 
    0x3, 0x2, 0x2, 0x2, 0xf9, 0xf7, 0x3, 0x2, 0x2, 0x2, 0xfa, 0xf2, 0x3, 
    0x2, 0x2, 0x2, 0xfa, 0xfb, 0x3, 0x2, 0x2, 0x2, 0xfb, 0x25, 0x3, 0x2, 
    0x2, 0x2, 0x17, 0x2e, 0x32, 0x40, 0x49, 0x4e, 0x57, 0x5f, 0x71, 0x74, 
    0x7a, 0x81, 0x95, 0xa3, 0xac, 0xb5, 0xc0, 0xd6, 0xed, 0xef, 0xf7, 0xfa, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

MiniParser::Initializer MiniParser::_init;
