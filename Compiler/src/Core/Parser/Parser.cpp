#include "Parser.h"
#include "Errors/ErrorHandler.h"

Parser::Parser(std::shared_ptr<std::vector<Token>>& tokenSequence, std::shared_ptr<ErrorHandler> errorHandler)
	: m_ErrorHandler(errorHandler), m_Instigator(EErrorInstigator::Parser), m_TokenSequense(tokenSequence)
{
}

void Parser::Parse()
{
	m_CurrentToken = m_TokenSequense->begin();
	m_AST = ParseTranslationUnit();
}

Ref<ASTNode> Parser::ParseTranslationUnit()
{
	if (IsAtEnd())
		return AST::MakeSignalProgram(nullptr);
	Ref<ASTNode> program = ParseProgram();
	return AST::MakeSignalProgram(program);
}

Ref<ASTNode> Parser::ParseProgram()
{
	if (!Match(ETokenCode::KW_PROGRAM))
	{
		auto error = CreateSyntaxError("'PROGRAM' expected at the start of the translation unit", *Peek());	
		m_ErrorHandler->ReportError(error);
		return nullptr;
	}
	auto program = PreviousCode();

	auto procedureIdentifier = ParseProcedureIdentifier();
	if (!procedureIdentifier)
	{
		auto error = CreateSyntaxError("Missing program name. Expected identifier", *Peek());
		m_ErrorHandler->ReportError(error);
		Synchronize();
	}

	auto semicolon = Consume(ETokenCode::D_Semicolon, "';' expected at the end of the program declaration");
	auto block = ParseBlock();
	auto dot = Consume(ETokenCode::D_Dot, "'.' expected at the end of the program.");

	return AST::MakeProgram(program, procedureIdentifier, TokenCode(semicolon), block, TokenCode(dot));
}

Ref<ASTNode> Parser::ParseBlock()
{
	auto varDecl = ParseVariableDeclarations();
	auto begin = Consume(ETokenCode::KW_BEGIN, "'BEGIN' expected");
	auto stmtList = ParseStatementsList();
	auto end = Consume(ETokenCode::KW_END, "'END' expected");

	return AST::MakeBlock(varDecl, TokenCode(begin), stmtList, TokenCode(end));
}

Ref<ASTNode> Parser::ParseVariableDeclarations()
{
	if (!Match(ETokenCode::KW_VAR))
		return AST::MakeVariableDeclarations(ETokenCode::Empty, nullptr, true);
	auto var = PreviousCode();

	auto declList = ParseDeclarationsList();

	return AST::MakeVariableDeclarations(var, declList);
}

Ref<ASTNode> Parser::ParseDeclarationsList()
{
	if (Check(ETokenCode::KW_BEGIN) || IsAtEnd())
		return AST::MakeDeclarationsList(nullptr, nullptr, true);

	auto decl = ParseDeclaration();	
	if (!decl)
		Synchronize();
	auto declList = ParseDeclarationsList();

	return AST::MakeDeclarationsList(decl, declList);
}

Ref<ASTNode> Parser::ParseDeclaration()
{
	auto varId = ParseVariableIndetifier();
	if (!varId)
	{
		auto error = CreateSyntaxError("Identifier expected at variable declaration or 'BEGIN' is missing", *Peek());
		m_ErrorHandler->ReportError(error);
		return nullptr;
	}

	auto colon = Consume(ETokenCode::D_Colon, "':' expected after identifier at variable declaration");
	if (!IsValid(colon))
		return nullptr;

	auto attribute = ParseAttribute();
	if (!attribute)
		return nullptr;

	auto semicolon = Consume(ETokenCode::D_Semicolon, "';' expected at the end of the variable declaration");
	if (!IsValid(semicolon))
		return nullptr;

	return AST::MakeDeclaration(varId, TokenCode(colon), attribute, TokenCode(semicolon));
}

Ref<ASTNode> Parser::ParseAttribute()
{
	if (!Match(ETokenCode::KW_INTEGER, ETokenCode::KW_FLOAT))
	{
		auto error = CreateSyntaxError("Type specifier expected", *Peek());
		m_ErrorHandler->ReportError(error);
		return nullptr;
	}
	auto attribute = PreviousCode();
	
	return AST::MakeAttribute(attribute);
}

Ref<ASTNode> Parser::ParseStatementsList()
{
	if (Check(ETokenCode::KW_END) || Check(ETokenCode::KW_ELSE) || Check(ETokenCode::KW_ENDIF))
		return AST::MakeStmtsList(nullptr, nullptr, true);

	auto stmt = ParseStatement();
	if (!stmt)
		return nullptr;

	auto stmtList = ParseStatementsList();
	return AST::MakeStmtsList(stmt, stmtList);
}

Ref<ASTNode> Parser::ParseStatement()
{
	auto stmt = ParseIfStatement();
	if (stmt)
		return stmt;

	stmt = ParseAssignStatement();	
	if (!stmt)
		Synchronize();

	return stmt;
}

Ref<ASTNode> Parser::ParseIfStatement()
{
	auto condStmt = ParseConditionStatement();
	if (!condStmt)
		return nullptr;

	auto endiff = Consume(ETokenCode::KW_ENDIF, "'ENDIF' expected");
	if (!IsValid(endiff))
		return AST::MakeIfStmt(condStmt, ETokenCode::Empty, ETokenCode::Empty);

	auto semicolon = Consume(ETokenCode::D_Semicolon, "';' expected at the end of the if statement");
	if(!IsValid(semicolon))
		return AST::MakeIfStmt(condStmt, TokenCode(endiff), ETokenCode::Empty);

	return AST::MakeIfStmt(condStmt, TokenCode(endiff), TokenCode(semicolon));
}

Ref<ASTNode> Parser::ParseAssignStatement()
{
	auto varId = ParseVariableIndetifier();
	if (!varId)
		return nullptr;
	auto assign = Consume(ETokenCode::DelimiterAssign, "':=' expected for assign expression");
	if (!IsValid(assign))
	{
		Synchronize();
		return AST::MakeAssignStmt(varId, ETokenCode::Empty, nullptr, ETokenCode::Empty);
	}

	auto expr = ParseExpression();
	if (!expr)
	{
		Synchronize();
		return	AST::MakeAssignStmt(varId, TokenCode(assign), nullptr, ETokenCode::Empty);
	}

	auto semicolon = Consume(ETokenCode::D_Semicolon, "';' expected at the end of the expression");
	if (!IsValid(semicolon))
		return	AST::MakeAssignStmt(varId, TokenCode(assign), expr, ETokenCode::Empty);

	return AST::MakeAssignStmt(varId, TokenCode(assign), expr, TokenCode(semicolon));
}


Ref<ASTNode> Parser::ParseConditionStatement()
{
	auto condStmt = ParseIncompleteConditionStatement();
	if (!condStmt)
		return nullptr;

	auto altPart = ParseAlternativePart();
	return AST::MakeConditionStmt(condStmt, altPart);
}

Ref<ASTNode> Parser::ParseIncompleteConditionStatement()
{
	if (!Match(ETokenCode::KW_IF))
		return nullptr;

	auto kwif = PreviousCode();
	auto condExpr = ParseConditionalExpression();
	if (!condExpr)
	{
		Synchronize();
		return AST::MakeIncompleteConditionStmt(kwif, nullptr, ETokenCode::Empty, nullptr);
	}

	auto kwthen = Consume(ETokenCode::KW_THEN, "'THEN' expected");
	if (!IsValid(kwthen))
		return AST::MakeIncompleteConditionStmt(kwif, condExpr, ETokenCode::Empty, nullptr);

	auto stmtList = ParseStatementsList();

	return AST::MakeIncompleteConditionStmt(kwif, condExpr, TokenCode(kwthen), stmtList);
}

Ref<ASTNode> Parser::ParseAlternativePart()
{
	if (!Check(ETokenCode::KW_ELSE))
		return AST::MakeAlternativePart(ETokenCode::Empty, nullptr, true);

	if (!Match(ETokenCode::KW_ELSE))
		return nullptr;

	auto kwelse = PreviousCode();
	auto stmtList = ParseStatementsList();

	return AST::MakeAlternativePart(kwelse, stmtList);
}

Ref<ASTNode> Parser::ParseConditionalExpression()
{
	auto expr1 = ParseExpression();
	if (!expr1)
		return nullptr;

	auto equal = Consume(ETokenCode::D_Equal, "'=' expected in conditional expression");
	if (!IsValid(equal))
	{
		SynchronizeSafe();
		return nullptr;
	}

	auto expr2 = ParseExpression();
	if (!expr2)
	{
		SynchronizeSafe();
		return nullptr;
	}

	return AST::MakeConditionalExpr(expr1, TokenCode(equal), expr2);
}

Ref<ASTNode> Parser::ParseExpression()
{
	auto expr = ParseVariableIndetifier();
	if (expr)
		return expr;
	expr = ParseConstant();
	return expr;
}

Ref<ASTNode> Parser::ParseVariableIndetifier()
{
	auto identifier = ParseIdentifier();
	if (!identifier)
		return nullptr;
	return AST::MakeVariableIdentifier(identifier);
}

Ref<ASTNode> Parser::ParseProcedureIdentifier()
{
	auto identifier = ParseIdentifier();
	if (!identifier)
		return nullptr;
	return AST::MakeProcedureIdentifier(identifier);
}

Ref<ASTNode> Parser::ParseIdentifier()
{
	if (!Match(ETokenCode::IdentifierBase))
		return nullptr;

	auto identifier = Previous()->Code;
	return AST::MakeIdentifier(identifier, *Peek());
}

Ref<ASTNode> Parser::ParseConstant()
{
	if (!Match(ETokenCode::ConstantBase))
	{
		auto error = CreateSyntaxError("Illegal symbol. expression expected", *Peek());
		m_ErrorHandler->ReportError(error);
		return nullptr;
	}
	auto constant = Previous()->Code;
	return AST::MakeConstant(constant, *Peek());
}

Ref<ASTNode> Parser::GetAST()
{
	return m_AST;
}

bool Parser::Match(const ETokenCode& expected)
{
	if (Check(expected))
	{
		Advance();
		return true;
	}
	return false;
}


bool Parser::Check(const ETokenCode& expected)
{
	if (IsAtEnd()) return false;
	if (Peek()->Code >= 501 && Peek()->Code < 1001)
		return +expected == 501;
	if (Peek()->Code >= 1001)
		return +expected == 1001;
	return Peek()->Code == +expected;
}

std::vector<Token>::iterator Parser::Advance()
{
	if (!IsAtEnd()) m_CurrentToken++;
	return Previous();
}

bool Parser::IsAtEnd()
{
	return Peek()->Code == +ETokenCode::Eof;
}

std::vector<Token>::iterator Parser::Peek()
{
	return m_CurrentToken;
}

std::vector<Token>::iterator Parser::Previous()
{
	return m_CurrentToken - 1;
}

std::vector<Token>::iterator Parser::Consume(ETokenCode kind, const std::string& message)
{
	if (Check(kind)) return Advance();

	auto error = CreateSyntaxError(message, *Peek());
	m_ErrorHandler->ReportError(error);
	return m_TokenSequense->end();
}

void Parser::Synchronize()
{

	Advance();

	while (!IsAtEnd())
	{
		if (Previous()->Code == +ETokenCode::D_Semicolon) return;

		switch (Peek()->Code)
		{
		case +ETokenCode::KW_PROGRAM:
		case +ETokenCode::KW_VAR:
		case +ETokenCode::KW_BEGIN:
		case +ETokenCode::KW_END:
		case +ETokenCode::KW_IF:
			return;

		}
		Advance();
	}
}

void Parser::SynchronizeSafe()
{
	Advance();
	while (!IsAtEnd())
	{
		if (Previous()->Code == +ETokenCode::D_Semicolon) return;

		switch (Peek()->Code)
		{
		case +ETokenCode::KW_PROGRAM:
		case +ETokenCode::KW_VAR:
		case +ETokenCode::KW_BEGIN:
		case +ETokenCode::KW_END:
		case +ETokenCode::KW_IF:
		case +ETokenCode::D_Semicolon:
			return;

		}
		Advance();
	}
}

bool Parser::IsValid(std::vector<Token>::iterator it)
{
	return it != m_TokenSequense->end();
}

ETokenCode Parser::TokenCode(std::vector<Token>::iterator it)
{
	if (!IsValid(it))
		return ETokenCode::Empty;
	return (ETokenCode)it->Code;
}

ETokenCode Parser::PreviousCode()
{
	return (ETokenCode)Previous()->Code;
}

Error Parser::CreateSyntaxError(const std::string& errorMessage, const Token& token)
{
	return ErrorHandler::CreateSyntaxError(errorMessage, token, m_Instigator);
}

