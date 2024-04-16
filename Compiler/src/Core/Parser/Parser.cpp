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
		return nullptr;
	Ref<ASTNode> program = ParseProgram();
	return AST::MakeSignalProgram(program);
}

Ref<ASTNode> Parser::ParseProgram()
{
	if (!Match(ETokenCode::KW_PROGRAM))
	{
		auto error = CreateSyntaxError("'PROGRAM' expected at the start of the translation unit.", *Peek());	
		m_ErrorHandler->ReportError(error);
		return nullptr;
	}
	auto program = (ETokenCode)Previous()->Code;

	auto procedureIdentifier = ParseProcedureIdentifier();
	if (!procedureIdentifier)
	{
		auto error = CreateSyntaxError("Missing program name. Expected identifier", *Peek());
		m_ErrorHandler->ReportError(error);
		Synchronize();
	}
	auto semicolon = Consume(ETokenCode::D_Semicolon, "';' expected at the end of the program declaration.");
	ETokenCode semicolonCode = ETokenCode::Empty;
	if (semicolon != m_TokenSequense->end())
		semicolonCode = (ETokenCode)semicolon->Code;


	auto block = ParseBlock();

	auto dot = Consume(ETokenCode::D_Dot, "'.' expected at the end of the program.");
	ETokenCode dotCode = ETokenCode::Empty;
	if (dot != m_TokenSequense->end())
		dotCode = (ETokenCode)dot->Code;
	auto node = AST::MakeProgram(program, procedureIdentifier, semicolonCode, block, dotCode);

	return node;
}

Ref<ASTNode> Parser::ParseBlock()
{
	auto varDecl = ParseVariableDeclarations();

	auto begin = Consume(ETokenCode::KW_BEGIN, "'BEGIN' expected");
	ETokenCode beginCode = ETokenCode::Empty;
	if (begin != m_TokenSequense->end())
		beginCode = (ETokenCode)begin->Code;

	auto stmtList = ParseStatementsList();
	auto end = Consume(ETokenCode::KW_END, "'END' expected");
	ETokenCode endCode = ETokenCode::Empty;
	if (end != m_TokenSequense->end())
		endCode = (ETokenCode)end->Code;
	auto node = AST::MakeBlock(varDecl, beginCode, stmtList, endCode);

	return node;
}

Ref<ASTNode> Parser::ParseVariableDeclarations()
{
	if (!Match(ETokenCode::KW_VAR))
	{
		return AST::MakeVariableDeclarations(ETokenCode::Empty, nullptr, true);
	}
	auto var = (ETokenCode)Previous()->Code;

	auto declList = ParseDeclarationsList();

	auto node = AST::MakeVariableDeclarations(var, declList);
	return node;
}

Ref<ASTNode> Parser::ParseDeclarationsList()
{
	if (Check(ETokenCode::KW_BEGIN) || IsAtEnd())
		return AST::MakeDeclarationsList(nullptr, nullptr, true);

	bool bIsDecl = false;
	auto decl = ParseDeclaration(bIsDecl);
			
	auto declList = ParseDeclarationsList();

	auto node = AST::MakeDeclarationsList(decl, declList);

	return node;
}

Ref<ASTNode> Parser::ParseDeclaration(bool& bIsDecl)
{
	auto varId = ParseVariableIndetifier();
	if (!varId)
	{
		auto error = CreateSyntaxError("Identifier expected at variable declaration or 'BEGIN' is missing", *Peek());
		m_ErrorHandler->ReportError(error);
		Synchronize();

		return nullptr;
	}
	auto colon = Consume(ETokenCode::D_Colon, "':' expected after identifier at variable declaration");
	ETokenCode colonCode = ETokenCode::Empty;
	if (colon == m_TokenSequense->end())
	{
		Synchronize();
		return nullptr;
	}
	colonCode = (ETokenCode)colon->Code;
	auto attribute = ParseAttribute();
	if (!attribute)
	{
		Synchronize();
		return nullptr;
	}
	auto semicolon = Consume(ETokenCode::D_Semicolon, "';' expected at the end of the variable declaration");
	ETokenCode semicolonCode = ETokenCode::Empty;
	if (semicolon == m_TokenSequense->end())
	{
		Synchronize();
		return nullptr;
	}
	semicolonCode = (ETokenCode)semicolon->Code;

	auto node = AST::MakeDeclaration(varId, colonCode, attribute, semicolonCode);
	return node;
}

Ref<ASTNode> Parser::ParseAttribute()
{
	if (!Match(ETokenCode::KW_INTEGER, ETokenCode::KW_FLOAT))
	{
		auto error = CreateSyntaxError("Type specifier expected", *Peek());
		m_ErrorHandler->ReportError(error);
		return nullptr;
	}
	auto attribute = (ETokenCode)Previous()->Code;
	
	auto node = AST::MakeAttribute(attribute);
	return node;
}

Ref<ASTNode> Parser::ParseStatementsList()
{
	if (Check(ETokenCode::KW_END) || Check(ETokenCode::KW_ELSE) || Check(ETokenCode::KW_ENDIF))
		return AST::MakeStmtsList(nullptr, nullptr, true);

	auto stmt = ParseStatement();
	if (!stmt)
		return nullptr;
	auto stmtList = ParseStatementsList();
	auto node = AST::MakeStmtsList(stmt, stmtList);
	return node;
}

Ref<ASTNode> Parser::ParseStatement()
{
	Ref<ASTNode> node = nullptr;
	
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
	{
		return nullptr;
	}
	auto endiff = Consume(ETokenCode::KW_ENDIF, "'ENDIF' expected");
	ETokenCode endifCode = ETokenCode::Empty;
	if (endiff == m_TokenSequense->end())
	{
		return AST::MakeIfStmt(condStmt, ETokenCode::Empty, ETokenCode::Empty);
	}
		endifCode = (ETokenCode)endiff->Code;
	auto semicolon = Consume(ETokenCode::D_Semicolon, "';' expected at the end of the if statement");
	ETokenCode semicolonCode = ETokenCode::Empty;
	if (semicolon == m_TokenSequense->end())
	{
		return AST::MakeIfStmt(condStmt, endifCode, ETokenCode::Empty);
	}
	semicolonCode = (ETokenCode)semicolon->Code;

	auto node = AST::MakeIfStmt(condStmt, endifCode, semicolonCode);
	return node;
}

Ref<ASTNode> Parser::ParseAssignStatement()
{
	auto varId = ParseVariableIndetifier();
	if (!varId)
		return nullptr;
	auto assign = Consume(ETokenCode::DelimiterAssign, "':=' expected for assign expression");
	ETokenCode assignCode = ETokenCode::Empty;
	if (assign == m_TokenSequense->end())
	{
		Synchronize();
		return AST::MakeAssignStmt(varId, ETokenCode::Empty, nullptr, ETokenCode::Empty);
	}
	assignCode = (ETokenCode)assign->Code;

	auto expr = ParseExpression();
	if (!expr)
	{
		Synchronize();
		return	AST::MakeAssignStmt(varId, assignCode, nullptr, ETokenCode::Empty);
	}

	auto semicolon = Consume(ETokenCode::D_Semicolon, "';' expected at the end of the expression");
	ETokenCode semicolonCode = ETokenCode::Empty;
	if (semicolon == m_TokenSequense->end())
	{
		return	AST::MakeAssignStmt(varId, assignCode, expr, ETokenCode::Empty);
	}
		semicolonCode = (ETokenCode)semicolon->Code;

	auto node = AST::MakeAssignStmt(varId, assignCode, expr, semicolonCode);
	return node;
}


Ref<ASTNode> Parser::ParseConditionStatement()
{
	auto condStmt = ParseIncompleteConditionStatement();
	if (!condStmt)
	{
		return nullptr;
	}
	auto altPart = ParseAlternativePart();
	auto node = AST::MakeConditionStmt(condStmt, altPart);
	return node;
}

Ref<ASTNode> Parser::ParseIncompleteConditionStatement()
{
	if (!Match(ETokenCode::KW_IF))
	{
		return nullptr;
	}
	auto kwif = (ETokenCode)Previous()->Code;
	auto condExpr = ParseConditionalExpression();
	if (!condExpr)
	{
		Synchronize();
		return AST::MakeIncompleteConditionStmt(kwif, nullptr, ETokenCode::Empty, nullptr);
	}
	auto kwthen = Consume(ETokenCode::KW_THEN, "'THEN' expected");
	ETokenCode thenCode = ETokenCode::Empty;
	if (kwthen == m_TokenSequense->end())
	{
		return AST::MakeIncompleteConditionStmt(kwif, condExpr, ETokenCode::Empty, nullptr);
	}
		thenCode = (ETokenCode)kwthen->Code;
	auto stmtList = ParseStatementsList();

	auto node = AST::MakeIncompleteConditionStmt(kwif, condExpr, thenCode, stmtList);

	return node;
}

Ref<ASTNode> Parser::ParseAlternativePart()
{
	if (!Check(ETokenCode::KW_ELSE))
		return AST::MakeAlternativePart(ETokenCode::Empty, nullptr, true);

	if (!Match(ETokenCode::KW_ELSE))
	{
		return nullptr;
	}
	auto kwelse = (ETokenCode)Previous()->Code;
	auto stmtList = ParseStatementsList();

	auto node = AST::MakeAlternativePart(kwelse, stmtList);
	return node;
}

Ref<ASTNode> Parser::ParseConditionalExpression()
{
	auto expr1 = ParseExpression();
	if (!expr1)
	{
		return nullptr;
	}

	auto equal = Consume(ETokenCode::D_Equal, "'=' expected in conditional expression");
	ETokenCode equalCode = ETokenCode::Empty;
	if (equal == m_TokenSequense->end())
	{
		SynchronizeSafe();
		return nullptr;
	}
	equalCode = (ETokenCode)equal->Code;

	auto expr2 = ParseExpression();
	if (!expr2)
	{
		SynchronizeSafe();
		return nullptr;
	}

	auto node = AST::MakeConditionalExpr(expr1, equalCode, expr2);
	return node;
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
	{
		return nullptr;
	}
	auto identifier = Previous()->Code;
	auto node = AST::MakeIdentifier(identifier);

	return node;
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
	auto node = AST::MakeConstant(constant);

	return node;
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

Error Parser::CreateSyntaxError(const std::string& errorMessage, const Token& token)
{
	return ErrorHandler::CreateSyntaxError(errorMessage, token, m_Instigator);
}

