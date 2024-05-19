#include "Driver.h"
#include "Errors/ErrorHandler.h"
#include "Errors/Error.h"
#include "Lexer/Lexer.h"
#include "Utilities/Log.h"
#include "Parser/PrintVisitor.h"
#include "Utilities/Timer.h"

#include <cstdlib>
#include <filesystem>

Driver::Driver()
{
	m_ErrorHandler = std::make_shared<ErrorHandler>();
	m_Compiler = std::make_unique<Compiler>(m_ErrorHandler);
}

bool Driver::CreateOptionsFromCLArguments(int argc, char* argv[])
{
	if (argc < 2)
	{
		m_UI->UsageHint();
		Terminate();
		return false;
// 		 		m_Options.SourceFile = "codegen_true_test.sig";
// 		 		m_Options.ListingOnly = false;
// 		 		m_Options.Verbose = true;
// 		 		m_Options.OutputFile = "";
	}
	else
	{
		bool matches = CheckSourceExtension(argv[1]);
		if (!matches)
		{
			m_UI->OutErrors();
			Terminate();
			return false;
		}
		bool gotSource = false;
		for (int i = 1; i < argc; i++)
		{
			if (std::string(argv[i])[0] != '-')
			{
				if (gotSource)
					continue;
				m_Options.SourceFile = argv[i];
				gotSource = true;
			}
			else if (std::string(argv[i]) == "-o")
			{
				if (i != argc - 1)
					m_Options.OutputFile = argv[++i];
			}
			else if (std::string(argv[i]) == "-S")
			{
				m_Options.ListingOnly = true;
			}
			else if (std::string(argv[i]) == "-v")
			{
				m_Options.Verbose = true;
			}
			else if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "-help")
			{
				m_UI->UsageHint();
				return false;
			}
		}
	}
	if (m_Options.OutputFile.empty())
	{
		std::filesystem::path sourcePath(m_Options.SourceFile);
		if (m_Options.ListingOnly)
			m_Options.OutputFile = sourcePath.filename().replace_extension(".s").string();
		else
			m_Options.OutputFile = sourcePath.filename().replace_extension(".exe").string();
	}
	std::filesystem::path filePath(m_Options.OutputFile);
	if (filePath.extension().empty())
	{
		if (m_Options.ListingOnly)
		{
			m_Options.OutputFile += ".s";
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
			if (!CorrectOutExtension(m_Options.OutputFile, ".s"))
			{
				m_UI->OutErrors();
				Terminate();
				return false;
			}
		}
		else
		{

			if (!CorrectOutExtension(m_Options.OutputFile, ".exe"))
			{
				m_UI->OutErrors();
				Terminate();
				return false;
			}
		}
	}
	return true;
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

bool Driver::Assemble()
{
	if (m_Options.ListingOnly)
		return true;
	std::filesystem::path filePath(m_Options.OutputFile);
	bool success = m_Compiler->Assemble(filePath.replace_extension(".asm").string());
	success &= m_Compiler->Link(filePath.replace_extension(".obj").string());

	return success;
}

void Driver::Start()
{
	LOG_STATE("Build started...", "\n");
	Timer timer;

	m_UI->SetInfoFileName("comp_info.txt");
	m_UI->SetOutToFileEnabled(true);

	std::filesystem::path outPath(m_Options.OutputFile);
	if (!m_Options.ListingOnly)
		outPath.replace_extension(".asm");

	m_Compiler->Compile(m_Options.SourceFile, outPath.string());
	bool success = !m_ErrorHandler->HasFatalError();
	if (success)
		success &= Assemble();

	m_UI->SetLexerData(m_Compiler->GetLexerData());
	m_UI->OutErrors();

	if (m_ErrorHandler->HasFatalError())
	{
		Terminate();
		return;
	}
	if (m_Options.Verbose)
	{
		m_UI->OutOptions(m_Options.SourceFile, m_Options.OutputFile);
		m_UI->OutLexerResult();
		AST::PrintVisitor printer;
		m_UI->OutAST(printer.Print(m_Compiler->GetAST()));
	}

	if (success)
	{
		LOG_STATE("========== Build: 1 succeeded ==========");
		LOG_STATE("========== Elapsed", timer.ElapsedFormat(), "==========");
	}

}

void Driver::SetUI(std::unique_ptr<CompilerInterface>&& ui)
{
	m_UI = std::move(ui);
	m_UI->SetErrorHandler(m_ErrorHandler);
}
