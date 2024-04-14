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
		auto error = CreateSyntaxError("Missing program name.", *Peek());
		m_ErrorHandler->ReportError(error);
	}
	auto semicolon = Consume((ETokenCode)(uint32_t)';', "';' expected at the end of the program declaration.");
	ETokenCode semicolonCode = ETokenCode::None;
	if (semicolon != m_TokenSequense->end())
		semicolonCode = (ETokenCode)semicolon->Code;

//	if (!semicolon)
//		Synchronize();

	auto block = ParseBlock();

	auto dot = Consume((ETokenCode)(uint32_t)'.', "'.' expected at the end of the program.");
	ETokenCode dotCode = ETokenCode::None;
	if (dot != m_TokenSequense->end())
		dotCode = (ETokenCode)dot->Code;
	auto node = AST::MakeProgram(program, procedureIdentifier, semicolonCode, block, dotCode);

	return node;
}

Ref<ASTNode> Parser::ParseBlock()
{
	auto varDecl = ParseVariableDeclarations();
	Advance();
	auto begin = Consume(ETokenCode::KW_BEGIN, "'BEGIN' expected but '" + m_CurrentToken->Lexeme + "' found");
	ETokenCode beginCode = ETokenCode::None;
	if (begin != m_TokenSequense->end())
		beginCode = (ETokenCode)begin->Code;

	auto stmtList = ParseStatementsList();
	auto end = Consume(ETokenCode::KW_END, "'END' expected but '" + m_CurrentToken->Lexeme + "' found");
	ETokenCode endCode = ETokenCode::None;
	if (begin != m_TokenSequense->end())
		endCode = (ETokenCode)end->Code;
	auto node = AST::MakeBlock(varDecl, beginCode, stmtList, endCode);

	return node;
}

Ref<ASTNode> Parser::ParseVariableDeclarations()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseDeclarationsList()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseDeclaration()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseAttribute()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseStatementsList()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseStatement()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseAssignExpression()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseConditionStatement()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseIncompleteConditionStatement()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseAlternativePart()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseConditionalExpression()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseExpression()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseVariableIndetifier()
{
	return nullptr;
}

Ref<ASTNode> Parser::ParseProcedureIdentifier()
{
	auto identifier = ParseIdentifier();
	return AST::MakeProcedureIdentifier(identifier);
}

Ref<ASTNode> Parser::ParseIdentifier()
{
	if (!Match(ETokenCode::IdentifierBase))
	{
		auto error = CreateSyntaxError("Illegal symbol" + m_CurrentToken->Lexeme + ". Identifier expected", *Peek());
		m_ErrorHandler->ReportError(error);
		return nullptr;
	}
	auto identifier = Previous()->Code;
	auto node = AST::MakeIdentifier(identifier);

	return node;
}

Ref<ASTNode> Parser::ParseConstant()
{
	return nullptr;
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
	if (Peek()->Code > 501 && Peek()->Code < 1001)
		return +expected == 501;
	if (Peek()->Code > 1001)
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
	//Advance();

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
		case +ETokenCode::KW_THEN:
		case +ETokenCode::KW_ELSE:
		case +ETokenCode::KW_ENDIF:
			return;

		}
		Advance();
	}
}

Error Parser::CreateSyntaxError(const std::string& errorMessage, const Token& token)
{
	return ErrorHandler::CreateSyntaxError(errorMessage, token, m_Instigator);
}

