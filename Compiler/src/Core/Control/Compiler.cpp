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

bool Compiler::Assemble(const std::string& filePath)
{
	std::string assembleCmd = "ml /c /Zd /coff " + filePath;
	std::string output;
	if (!ExecuteCommand(assembleCmd, output))
	{
		auto error = ErrorHandler::CreateBuildError("Error executing assembler command: " + assembleCmd + "\nOutput: " + output, EErrorInstigator::Compiler);
		m_ErrorHandler->ReportError(error);
		m_ErrorHandler->GotFatalError();
		return false;
	}	
	return true;
}

bool Compiler::Link(const std::string& filePath)
{
	std::string linkCmd = "link /SUBSYSTEM:CONSOLE " + filePath;
	std::string output;
	if (!ExecuteCommand(linkCmd, output))
	{
		auto error = ErrorHandler::CreateBuildError("Error executing linker command: " + linkCmd + "\nOutput: " + output, EErrorInstigator::Compiler);
		m_ErrorHandler->ReportError(error);
		m_ErrorHandler->GotFatalError();
		return false;
	}
	return true;
}

bool Compiler::ExecuteCommand(const std::string& command, std::string& output)
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	// Create pipes for standard output and standard error
	HANDLE hStdOutRead, hStdOutWrite;
	HANDLE hStdErrRead, hStdErrWrite;
	if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0) || !CreatePipe(&hStdErrRead, &hStdErrWrite, &sa, 0))
	{
		auto error = ErrorHandler::CreateBuildError("CreatePipe failed", EErrorInstigator::Compiler);
		m_ErrorHandler->ReportError(error);
		m_ErrorHandler->GotFatalError();
		return false;
	}

	// Ensure the write handles are not inherited
	SetHandleInformation(hStdOutWrite, HANDLE_FLAG_INHERIT, 0);
	SetHandleInformation(hStdErrWrite, HANDLE_FLAG_INHERIT, 0);

	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdOutput = hStdOutWrite;
	si.hStdError = hStdErrWrite;
	si.dwFlags |= STARTF_USESTDHANDLES;
	ZeroMemory(&pi, sizeof(pi));

	// Create a mutable copy of the command
	char cmd[256];
	strncpy(cmd, command.c_str(), sizeof(cmd));
	cmd[sizeof(cmd) - 1] = '\0';  // Ensure null-termination

	// Create the process
	if (!CreateProcessA(
		NULL,      // No module name (use command line)
		cmd,       // Command line
		NULL,      // Process handle not inheritable
		NULL,      // Thread handle not inheritable
		TRUE,      // Set handle inheritance to TRUE
		0,         // No creation flags
		NULL,      // Use parent's environment block
		NULL,      // Use parent's starting directory 
		&si,       // Pointer to STARTUPINFO structure
		&pi)       // Pointer to PROCESS_INFORMATION structure
		)
	{
		auto error = ErrorHandler::CreateBuildError(std::string("CreateProcess failed:  ") + std::to_string(uint64_t(GetLastError())), EErrorInstigator::Compiler);
		m_ErrorHandler->ReportError(error);
		m_ErrorHandler->GotFatalError();
		return false;
	}
	// Close the write ends of the pipes
	CloseHandle(hStdOutWrite);
	CloseHandle(hStdErrWrite);

	// Read the output from the child process
	DWORD bytesRead;
	CHAR buffer[256];
	while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL) || ReadFile(hStdErrRead, buffer, sizeof(buffer) - 1, &bytesRead, NULL))
	{
		buffer[bytesRead] = '\0';
		output += buffer;
	}

	// Wait until child process exits
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exitCode;
	if (!GetExitCodeProcess(pi.hProcess, &exitCode))
	{
		auto error = ErrorHandler::CreateBuildError(std::string("GetExitCodeProcess failed "), EErrorInstigator::Compiler);
		m_ErrorHandler->ReportError(error);
		m_ErrorHandler->GotFatalError();
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return false;
	}

	// Close process and thread handles
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(hStdOutRead);
	CloseHandle(hStdErrRead);

	return exitCode == 0;
}

