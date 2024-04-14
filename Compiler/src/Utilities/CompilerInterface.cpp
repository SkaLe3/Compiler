#include "CompilerInterface.h"
#include "Data/Token.h"
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
		return TEAL;
	default:
		return RESET;
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


CLI::~CLI()
{
	m_Ofs.close();
}

void CLI::OutErrors()
{
	if (m_ErrorHandler->GetErrors()->empty())
		return;

	std::cout << CRIMSON <<"============ Error List: ============\n\n" << RESET;
 	
	for (Error& error : *m_ErrorHandler->GetErrors())
	{	
		std::cout << "[";
		std::cout << GetInstigatorColor(error.GetEnumInstigator());
		std::cout << error.GetInstigator() << RESET << "] ";

		if (error.GetLine() != "0")
			std::cout << "(" << TEAL << error.GetLine() << RESET << "," << TEAL << error.GetPosition() << RESET << "): ";

		std::cout << CRIMSON << error.GetType() << RESET << ": " << error.GetMessage() << "\n";
	}
	std::cout << CRIMSON << "=====================================\n\n" << RESET;

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

void CLI::OutLexerResult()
{
	OutTokens();
	OutIdentifiersTable();
	OutConstantsTable();
	OutKeywordsTable();
}

void CLI::OutTokens()
{
	const uint32_t lineWidth = 4;
	const uint32_t posWidth = 4;
	const uint32_t codeWidth = 8;
	const uint32_t lexemeWidth = 22;

	std::cout << LIME << "============ Token List: ============\n" << RESET;
	std::cout <<  " Line" <<  "   Pos" << "   Code" << "           Lexeme\n" << std::endl;

	for (const Token& token : *(m_LexerData->Tokens))
	{

		int32_t padding = (lineWidth - std::to_string(token.Line).size()) / 2;
		std::cout << "|" << TEAL << std::right << std::setw(lineWidth - padding) << token.Line << RESET << std::setw(padding + 1) << "]";

		padding = (posWidth - std::to_string(token.Position).size()) / 2;
		std::cout<< "[" << TEAL << std::setw(posWidth - padding) << token.Position << RESET << std::setw(padding+1) << "]";

		padding = (codeWidth - std::to_string(+token.Code).size()) / 2;
		std::cout << CRIMSON << std::setw(codeWidth - padding) << +token.Code << RESET << std::setw(padding + 1) << "=";

		size_t lex = token.Lexeme.size();
		padding = (lexemeWidth - lex) / 2;
		std::cout << std::setw(lexemeWidth - lex - padding) << std::right << "<" << LEMON <<  token.Lexeme << RESET << ">" << std::setw(padding+1) << " " << std::endl;
	}

	std::cout << LIME << "=====================================\n\n" << RESET;



	m_Ofs << "============ Token List: ============\n";
	m_Ofs << " Line" << "   Pos" << "   Code" << "           Lexeme\n" << std::endl;

	for (const Token& token : *(m_LexerData->Tokens))
	{
		int32_t padding = (lineWidth - std::to_string(token.Line).size()) / 2;
		m_Ofs << "|" << std::right << std::setw(lineWidth - padding) << token.Line << std::setw(padding + 1) << "]";

		padding = (posWidth - std::to_string(token.Position).size()) / 2;
		m_Ofs << "[" << std::setw(posWidth - padding) << token.Position << std::setw(padding + 1) << "]";

		padding = (codeWidth - std::to_string(+token.Code).size()) / 2;
		m_Ofs << std::setw(codeWidth - padding) << +token.Code << std::setw(padding + 1) << "=";

		padding = (lexemeWidth - token.Lexeme.size()) / 2;
		m_Ofs << std::setw(lexemeWidth - padding) << std::right << "<" + token.Lexeme + ">" << std::setw(padding + 1) << " " << std::endl;
	}

	m_Ofs << "=====================================\n\n";
	
}

void CLI::OutIdentifiersTable()
{
	DisplayTable(m_LexerData->IdentifiersTable, "Identifiers Table");
}

void CLI::OutConstantsTable()
{
	DisplayTable(m_LexerData->ConstantsTable, "Constants Table");
}

void CLI::OutKeywordsTable()
{
	DisplayTable(m_LexerData->KeyWordsTable, "Keywords Table");
}

void CLI::OutAST(const std::string& ast)
{
	std::cout << "Syntax Tree:\n";
	std::cout << ast << "\n";
}

void CLI::OutOptions()
{
	std::cout << "Source file: \n";
	std::cout << "Out file: \n";
}

void CLI::UsageHint(char* name)
{
	std::cout << "Usage: " << "./ssc" << " <source_file> [options...] <out_file>\n";
}

void CLI::DisplayTable(const std::unordered_map<std::string, uint32_t>& table, const std::string& tableHeader)
{
	const uint32_t lexemeWidth = 25;

	std::cout << "========== " << TEAL << tableHeader << ": " << RESET << "========= \n\n";

	std::cout << "| Code" << "|          Lexeme         |" << std::endl;
	std::cout << "+-----+-------------------------+" << std::endl;

	for (auto& record : table)
	{
		std::cout << std::left << "|" << CRIMSON << std::setw(5) << record.second << RESET << "|";

		size_t lex = record.first.size();
		int32_t padding = (float)(lexemeWidth - lex) / 2.0;
		std::cout << std::setw(lexemeWidth - lex - padding) << std::right << "<" << LEMON << record.first << RESET << ">" << std::setw(padding) << " |" << std::endl;
	}

	std::cout << "=====================================\n\n";


	m_Ofs << "========== " << tableHeader << ": =========\n\n";

	m_Ofs << "| Code" << "|          Lexeme         |" << std::endl;
	m_Ofs << "+-----+-------------------------+" << std::endl;

	for (auto& record : table)
	{
		m_Ofs << std::left << "|" << std::setw(5) << record.second << "|";

		int32_t padding = (float)(lexemeWidth - record.first.size()) / 2.0;
		m_Ofs << std::setw(lexemeWidth - padding) << std::right << "<" + record.first + ">" << std::setw(padding) << "|" << std::endl;
	}

	m_Ofs << "=====================================\n\n";
}
