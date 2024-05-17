#include "Compiler.h"
#include "Errors/ErrorHandler.h"

#include <windows.h>

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

void Compiler::Assemble(const std::string& filePath)
{
	std::string assembleCmd = "ml /c /Zd /coff " + filePath;
	
}

void Compiler::Link(const std::string& filePath)
{
	std::string linkCmd = "link /SUBSYSTEM:CONSOLE " + filePath;
}

bool Compiler::ExecuteCommand(const std::string& command)
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char cmd[256];
	strncpy(cmd, command.c_str(), sizeof(cmd));
	cmd[sizeof(cmd) - 1] = 0;  // Ensure null-termination

	if (!CreateProcessA(
		NULL,      // No module name (use command line)
		cmd,       // Command line
		NULL,      // Process handle not inheritable
		NULL,      // Thread handle not inheritable
		FALSE,     // Set handle inheritance to FALSE
		0,         // No creation flags
		NULL,      // Use parent's environment block
		NULL,      // Use parent's starting directory 
		&si,       // Pointer to STARTUPINFO structure
		&pi)       // Pointer to PROCESS_INFORMATION structure
		)
	{
		auto error = ErrorHandler::CreateBuildError(std::string("CreateProcess failed:  ") + GetLastError(), EErrorInstigator::Compiler);
		m_ErrorHandler->ReportError(error);
		m_ErrorHandler->GotFatalError();
		std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
		return false;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return true;
}

