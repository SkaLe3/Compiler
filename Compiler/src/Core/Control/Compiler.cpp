#include "Compiler.h"
#include "Errors/ErrorHandler.h"


Compiler::Compiler(std::shared_ptr<ErrorHandler> errorHandler) : m_ErrorHandler(errorHandler)
{
	m_TokenSequence = std::make_shared<std::vector<Token>>();
	m_Lexer = std::make_unique<Lexer>(m_TokenSequence, errorHandler);
	m_Parser = std::make_unique<Parser>(m_TokenSequence, errorHandler);
}

void Compiler::Compile(const std::string& filePath)
{
	m_Lexer->Scan(filePath);
	m_Parser->Parse();
	m_AST = m_Parser->GetAST();
}

std::shared_ptr<LexerData> Compiler::GetLexerData()
{
	return m_Lexer->GetLexerData();
}

Ref<ASTNode> Compiler::GetAST()
{
	return m_AST;
}

