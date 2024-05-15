#include "Driver.h"
#include "Errors/ErrorHandler.h"
#include "Errors/Error.h"
#include "Lexer/Lexer.h"
#include "Utilities/Log.h"
#include "Parser/PrintVisitor.h"

#include <cstdlib>
#include <filesystem>

Driver::Driver()
{
	m_ErrorHandler = std::make_shared<ErrorHandler>();
	m_Compiler = std::make_unique<Compiler>(m_ErrorHandler);
}

void Driver::CreateOptionsFromCLArguments(int argc, char* argv[])
{
	if (argc < 2)
	{
		m_UI->UsageHint(argv[0]);
		Terminate();
		return;
		// 		m_Options.SourceFile = ".\\tests\\parser_true_test1.sig";
		// 		return;
	}

	bool matches = CheckSourceExtension(argv[1]);
	if (!matches)
	{
		m_UI->OutErrors();
		Terminate();
		return;
	}

	m_Options.SourceFile = argv[1];
	// TODO : Fix corner cases error 
	for (int i = 2; i < argc - 1; i++)
	{
		if (std::string(argv[i]) == "-o")
		{
			m_Options.OutputFile = argv[i + 1];
		}
		else if (std::string(argv[i]) == "-S")
		{
			m_Options.ListingOnly = true;
		}
	}

	std::filesystem::path filePath(m_Options.OutputFile);
	if (filePath.extension().empty())
	{
		if (m_Options.ListingOnly)
		{
			m_Options.OutputFile += ".asm";
		}
		else
		{
			m_Options.OutputFile += ".exe";
		}
	}
	else
	{
		if (m_Options.ListingOnly)
		{
			if (!CorrectOutExtension(m_Options.OutputFile, ".asm"))
			{
				m_UI->OutErrors();
				Terminate();
				return;
			}	
		}
		else
		{
			if (!CorrectOutExtension(m_Options.OutputFile, ".exe"));
			{
				m_UI->OutErrors();
				Terminate();
				return;
			}
		}
	}
}

bool Driver::CheckSourceExtension(const std::string& filePath)
{
	size_t dotPos = filePath.rfind('.');
	if (dotPos != std::string::npos)
	{
		std::string extension = filePath.substr(dotPos + 1);
		if (extension != "sig")
		{
			auto error = ErrorHandler::CreateGeneralError(std::string("Incorrect input file type: .") + extension + "\n message: expected file extension \".sig\"", EErrorInstigator::FileIO);
			m_ErrorHandler->ReportError(error);
			return false;
		}
	}
	return true;
}

bool Driver::CorrectOutExtension(std::string& filePath, const std::string& expected)
{
	std::filesystem::path fileName(filePath);
	if (fileName.extension() != expected)
	{
		auto error = ErrorHandler::CreateGeneralError("Incorrect output file type: " + fileName.extension().string() + "\nMessage: expected file extension \"" + expected + "\"", EErrorInstigator::FileIO);
		m_ErrorHandler->ReportError(error);
		return false;
	}
	return true;
}

void Driver::Terminate()
{
	LOG_ERROR("Compilation terminated");
	exit(EXIT_FAILURE);
}

void Driver::Start()
{
	LOG_STATE("Build started...", "\n");

	m_UI->SetInfoFileName("comp_info.txt");
	m_UI->SetOutToFileEnabled(true);

	m_Compiler->Compile(m_Options.SourceFile, m_Options.OutputFile);

	m_UI->SetLexerData(m_Compiler->GetLexerData());
	m_UI->OutErrors();

	if (m_ErrorHandler->HasFatalError())
	{
		Terminate();
		return;
	}

	m_UI->OutLexerResult();
	AST::PrintVisitor printer;
	m_UI->OutAST(printer.Print(m_Compiler->GetAST()));

}

void Driver::SetUI(std::unique_ptr<CompilerInterface>&& ui)
{
	m_UI = std::move(ui);
	m_UI->SetErrorHandler(m_ErrorHandler);
}
