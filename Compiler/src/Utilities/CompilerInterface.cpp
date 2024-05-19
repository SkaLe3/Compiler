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
	case EErrorInstigator::Parser:
		return MAGENTA;
	case EErrorInstigator::CodeGenerator:
		return 	AZURE;
	case EErrorInstigator::FileIO:
		return LEMON;
	case EErrorInstigator::Compiler:
		return LIME;
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
		if (token.Code == +ETokenCode::Eof)
			break;

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
	std::cout << AZURE;
	std::cout << "============ Syntax Tree ============\n\n";
	std::cout << RESET;
	std::cout << ast << "\n";
	std::cout << AZURE;
	std::cout << "=====================================\n\n";
	std::cout << RESET;

	std::string ast_uncolored = ast;
	RemoveColors(ast_uncolored);
	m_Ofs << "============ Syntax Tree ============\n\n";
	m_Ofs << ast_uncolored << "\n";
	m_Ofs << "=====================================\n\n";

}

void CLI::OutOptions(const std::string& source, const std::string& out)
{
	std::cout << "Source file: " << source << "\n";
	std::cout << "Out file: " << out <<"\n";

	m_Ofs << "Source file: " << source << "\n";
	m_Ofs << "Out file: " << out << "\n";
}

void CLI::UsageHint()
{
	std::cout << "\nUsage: .\\ssc [options] <source_file> [options]\n";
	std::cout << "Options:\n";
	std::cout << "  -o <file>       Place the output into <file>\n";
	std::cout << "  -S              Compile only; do not assemble or link\n";
	std::cout << "  -v              Verbose mode; show detailed compiler operations\n";
	std::cout << "  -h, --help      Display this information\n\n";
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


void CLI::RemoveColors(std::string& str)
{
	size_t startPos = 0;

	while ((startPos = str.find("", startPos)) != std::string::npos)
	{
		size_t endPos = str.find('m', startPos);
		if (endPos != std::string::npos)
		{
			str.erase(startPos, endPos - startPos + 1);
		}
		else
		{
			break;
		}
	}
}