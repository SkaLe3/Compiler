#include "Driver.h"
#include "Errors/ErrorHandler.h"
#include "Errors/Error.h"
#include "Lexer/Lexer.h"
#include "Utilities/Log.h"
#include "Parser/PrintVisitor.h"

#include <cstdlib>

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
// 		m_Options.SourceFile = ".\\tests\\parser_false_test7.sig";
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
	for (int i = 2; i < argc-1; i++)
	{
		if (std::string(argv[i]) == "-o")
		{
			m_Options.OutputFile = argv[i + 1];
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

	m_Compiler->Compile(m_Options.SourceFile);

	m_UI->SetLexerData(m_Compiler->GetLexerData());
	m_UI->OutErrors();

	if (m_ErrorHandler->HasFatalError())
	{
		Terminate();
		return;
	}

	m_UI->OutLexerResult();
	AST::PrintVisitor printer;
	m_UI->OutAST(printer.Print(m_Compiler->GetAST()));	// TODO : Add output to file
														// TODO : Output more information

}

void Driver::SetUI(std::unique_ptr<CompilerInterface>&& ui)
{
	m_UI = std::move(ui);
	m_UI->SetErrorHandler(m_ErrorHandler);
}
