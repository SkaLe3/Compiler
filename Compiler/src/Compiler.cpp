#include "Compiler.h"
#include "ErrorHandler.h"
#include <iostream>

Compiler::Compiler(std::shared_ptr<ErrorHandler> errorHandler) : m_ErrorHandler(errorHandler)
{
	m_Lexer = std::make_unique<Lexer>(errorHandler);
}

void Compiler::Compile(const std::string& filePath)
{
	CheckSourceExtension(filePath);
	
	m_Lexer->Scan(filePath);

}

std::shared_ptr<LexerData> Compiler::GetLexerData()
{
	return m_Lexer->GetLexerData();
}

void Compiler::CheckSourceExtension(const std::string& filePath)
{
	size_t dotPos = filePath.find('.');
	if (dotPos != std::string::npos)
	{
		std::string extension = filePath.substr(dotPos + 1);
		if (extension != "sig")
		{
			auto error = ErrorHandler::CreateGeneralError("File IO", std::string("Incorrect input file type: .") + extension + "\n message: expected \".sig\"");
			m_ErrorHandler->ReportError(error);
		}
	}
}
