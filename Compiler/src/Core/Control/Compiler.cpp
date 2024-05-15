#include "Compiler.h"
#include "Errors/ErrorHandler.h"


Compiler::Compiler(std::shared_ptr<ErrorHandler> errorHandler) : m_ErrorHandler(errorHandler)
{
	m_TokenSequence = std::make_shared<std::vector<Token>>();
	m_Lexer = std::make_unique<Lexer>(m_TokenSequence, errorHandler);
	m_Parser = std::make_unique<Parser>(m_TokenSequence, errorHandler);
	m_Generator = std::make_unique<Generator>(errorHandler);
}

void Compiler::Compile(const std::string& inputfilePath, const std::string& outputfilePath)
{
	m_Lexer->Scan(inputfilePath);
	m_Parser->Parse();
	m_AST = m_Parser->GetAST();
	Generator::SetInOut(m_AST, outputfilePath);
	m_Generator->Generate();
}

std::shared_ptr<LexerData> Compiler::GetLexerData()
{
	return m_Lexer->GetLexerData();
}

Ref<ASTNode> Compiler::GetAST()
{
	return m_AST;
}

