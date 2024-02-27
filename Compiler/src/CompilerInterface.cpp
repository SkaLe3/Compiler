#include "CompilerInterface.h"
#include "Token.h"
#include "Log.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>


std::string GetInstigatorColor(EErrorInstigator inst)
{
	switch (inst)
	{
	case EErrorInstigator::Lexer:
		return "\033[0;36m";
	default:
		return "\033[0m";
	}
}

void CompilerInterface::SetErrorHandler(std::shared_ptr<ErrorHandler> errorHandler)
{
	m_ErrorHandler = errorHandler;
}

void CompilerInterface::SetLexerData(std::shared_ptr<LexerData> lexerData)
{
	m_LexerData = lexerData;
}

void CompilerInterface::SetInfoFileName(const std::string& fileName)
{
	m_InfoFileName = fileName;

	m_Ofs.open(m_InfoFileName);

	if (!m_Ofs)
	{
		LOG_ERROR("Error opening file:", m_InfoFileName);
	}
}

void CompilerInterface::SetOutToFileEnabled(bool option)
{
	bOutToFile = option;
}


CLInterface::~CLInterface()
{
	m_Ofs.close();
}

void CLInterface::OutErrors()
{
	if (m_ErrorHandler->GetErrors()->empty())
		return;

	std::cout << "\n\033[0;31m============ \033[1;31mError List: \033[0;31m============\033[0m\n\n";
 	
	for (Error& error : *m_ErrorHandler->GetErrors())
	{	
		std::cout << "[";
		std::cout << GetInstigatorColor(error.GetEnumInstigator());
		std::cout << error.GetInstigator() << "\033[0m" << "] ";

		if (error.GetLine() != "0")
			std::cout << "(\033[1;36m" << error.GetLine() << "\033[0m,\033[1;36m" << error.GetPosition() << "\033[0m): ";

		std::cout << "\033[0;31m" << error.GetType() << "\033[0m: " << error.GetMessage() << "\n";
	}
	std::cout << "\033[0;31m=====================================\033[0m\n\n";

	if (!bOutToFile)
		return;


	m_Ofs << "============ Error List: ============\n\n";

	for (Error& error : *m_ErrorHandler->GetErrors())
	{
		m_Ofs << "[" << error.GetInstigator() << "] ";

		if (error.GetLine() != "0")
			m_Ofs << "(" << error.GetLine() << "," << error.GetPosition() << "): ";

		m_Ofs << error.GetType()<< ": " << error.GetMessage() << "\n";
	}
	m_Ofs << "=====================================\n\n";
}

void CLInterface::OutTokens()
{
	const uint32_t lineWidth = 4;
	const uint32_t posWidth = 4;
	const uint32_t codeWidth = 8;
	const uint32_t lexemeWidth = 22;

	std::cout << "\033[0;32m============ \033[1;32mToken List: \033[0;32m============\033[0m\n";
	std::cout <<  " Line" <<  "   Pos" << "   Code" << "           Lexeme\n" << std::endl;

	for (const Token& token : m_LexerData->Tokens)
	{
		int32_t padding = (lineWidth - std::to_string(token.Line).size()) / 2;
		std::cout << "[\033[1;36m" << std::right << std::setw(lineWidth - padding) << token.Line << "\033[0m" << std::setw(padding+1) << "]";

		padding = (posWidth - std::to_string(token.Position).size()) / 2;
		std::cout<< "[\033[1;36m" << std::setw(posWidth - padding) << token.Position << "\033[0m" << std::setw(padding+1) << "]";

		padding = (codeWidth - std::to_string(token.Code).size()) / 2;
		std::cout << "\033[1;31m" << std::setw(codeWidth - padding) << token.Code << "\033[0m" << std::setw(padding + 1) << "=";

		padding = (lexemeWidth - token.Lexeme.size()) / 2;
		std::cout << std::setw(lexemeWidth - padding + 12) << std::right << "<\033[0;33m" + token.Lexeme + "\033[0m>" << std::setw(padding+1) << " " << std::endl;
	}

	std::cout << "\033[0;32m=====================================\033[0m\n\n";



	m_Ofs << "============ Token List: ============\n";
	m_Ofs << " Line" << "   Pos" << "   Code" << "           Lexeme\n" << std::endl;

	for (const Token& token : m_LexerData->Tokens)
	{
		int32_t padding = (lineWidth - std::to_string(token.Line).size()) / 2;
		m_Ofs << std::right << std::setw(lineWidth - padding) << token.Line << std::setw(padding + 1) << "]";

		padding = (posWidth - std::to_string(token.Position).size()) / 2;
		m_Ofs << std::setw(posWidth - padding) << token.Position << std::setw(padding + 1) << "]";

		padding = (codeWidth - std::to_string(token.Code).size()) / 2;
		m_Ofs << std::setw(codeWidth - padding) << token.Code << std::setw(padding + 1) << "=";

		padding = (lexemeWidth - token.Lexeme.size()) / 2;
		m_Ofs << std::setw(lexemeWidth - padding + 12) << std::right << "<" + token.Lexeme + ">" << std::setw(padding + 1) << " " << std::endl;
	}

	m_Ofs << "=====================================\n\n";
	
}

void CLInterface::OutIdentifiersTable()
{
	DisplayTable(m_LexerData->IdentifiersTable, "Identifiers Table");
}

void CLInterface::OutConstantsTable()
{
	DisplayTable(m_LexerData->ConstantsTable, "Constants Table");
}

void CLInterface::OutKeywordsTable()
{
	DisplayTable(m_LexerData->KeyWordsTable, "Keywords Table");
}

void CLInterface::OutOptions()
{
	std::cout << "Source file: \n";
	std::cout << "Out file: \n";
}

void CLInterface::UsageHint(char* name)
{
	std::cout << "Usage: " << name << " <source_file> [options...] <out_file>\n";
}

void CLInterface::DisplayTable(const std::unordered_map<std::string, uint32_t>& table, const std::string& tableHeader)
{
	const uint32_t lexemeWidth = 25;

	std::cout << "========== \033[0;36m" << tableHeader << ":\033[0m =========\n\n";

	std::cout << "| Code" << "|          Lexeme         |" << std::endl;
	std::cout << "+-----+-------------------------+" << std::endl;

	for (auto& record : table)
	{
		std::cout << std::left << "|\033[1;31m" << std::setw(5) << record.second << "\033[0m|";

		int32_t padding = (float)(lexemeWidth - record.first.size()) / 2.0;
		std::cout << std::setw(lexemeWidth - padding + 12) << std::right << "<\033[0;33m" + record.first + "\033[0m>" << std::setw(padding) << "|" << std::endl;
	}

	std::cout << "=====================================\n\n";


	m_Ofs << "========== " << tableHeader << ": =========\n\n";

	m_Ofs << "| Code" << "|          Lexeme         |" << std::endl;
	m_Ofs << "+-----+-------------------------+" << std::endl;

	for (auto& record : table)
	{
		m_Ofs << std::left << "|" << std::setw(5) << record.second << "|";

		int32_t padding = (float)(lexemeWidth - record.first.size()) / 2.0;
		m_Ofs << std::setw(lexemeWidth - padding + 12) << std::right << "<" + record.first + ">" << std::setw(padding) << "|" << std::endl;
	}

	m_Ofs << "=====================================\n\n";
}
