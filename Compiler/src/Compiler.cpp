#include "Compiler.h"
#include "ErrorHandler.h"


Compiler::Compiler(std::shared_ptr<ErrorHandler> errorHandler) : m_ErrorHandler(errorHandler)
{
	m_Lexer = std::make_unique<Lexer>(errorHandler);
}

void Compiler::Compile(const std::string& filePath)
{
	m_Lexer->Scan(filePath);
}

std::shared_ptr<LexerData> Compiler::GetLexerData()
{
	return m_Lexer->GetLexerData();
}


