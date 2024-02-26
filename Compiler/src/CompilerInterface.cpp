#include "CompilerInterface.h"
#include "Token.h"
#include <iostream>
#include <iomanip>
#include <vector>
void CompilerInterface::SetErrorHandler(std::shared_ptr<ErrorHandler> errorHandler)
{
	m_ErrorHandler = errorHandler;
}

void CompilerInterface::SetLexerData(std::shared_ptr<LexerData> lexerData)
{
	m_LexerData = lexerData;
}

void CLInterface::DisplayErrors()
{
	std::cout << "\033[0;31m============ \033[1;31mError List: \033[0;31m============\033[0m\n\n";
 	
	for (Error& error : *m_ErrorHandler->GetErrors())
	{
		std::cout << error.GetMessage() << "\n";
	}
	std::cout << std::endl;
	std::cout << "\033[0;31m=====================================\033[0m\n\n";
}

void CLInterface::DisplayTokens()
{
	std::cout << "\033[0;32m============ \033[1;32mToken List: \033[0;32m============\033[0m\n";

	std::cout <<  " Line" <<  "   Pos" << "   Code " << "          Lexeme\n" << std::endl;

	for (const Token& token : m_LexerData->Tokens)
	{
		int32_t padding = (4 - std::to_string(token.Line).size()) / 2;
		std::cout << "[\033[1;36m" << std::right << std::setw(4 - padding) << token.Line << "\033[0m" << std::setw(padding+1) << "]";
		padding = (4 - std::to_string(token.Position).size()) / 2;
		std::cout<< "[\033[1;36m" << std::setw(4 - padding) << token.Position << "\033[0m" << std::setw(padding+1) << "]";

		padding = (8 - std::to_string(token.Code).size()) / 2;
		std::cout << "\033[1;31m" << std::setw(8 - padding) << token.Code << "\033[0m" << std::setw(padding + 1) << "=";

		padding = (22 - token.Lexeme.size()) / 2;
		std::cout << std::setw(22 - padding + 12) << std::right << "<\033[0;33m" + token.Lexeme + "\033[0m>" << std::setw(padding+1) << " " << std::endl;
	}

	std::cout << std::endl;
	std::cout << "\033[0;32m=====================================\033[0m\n\n";
}

void CLInterface::DisplayIdentifiersTable()
{
	std::cout << "\033[0;36m========= \033[0;36mIdentifiers Table: \033[0;36m========\033[0m\n\n";

	std::cout << "| Code" << "|          Lexeme         |" << std::endl;
	std::cout << "+-----+-------------------------+" << std::endl;

	// Print each token
	for (auto& record : m_LexerData->IdentifiersTable)
	{
		std::cout << std::left << "|" << std::setw(5) << record.second << "|";

		int32_t lexemeSize = record.first.size();
		int32_t padding = (float)(25 - lexemeSize) / 2.0;
		int32_t width = 25 - padding;

		std::cout << std::setw(width+12) << std::right << "<\033[0;33m" + record.first + "\033[0m>" << std::setw(padding)  << "|" << std::endl;
	}

	std::cout << std::endl;
	std::cout << "\033[0;36m=====================================\033[0m\n\n";
}
