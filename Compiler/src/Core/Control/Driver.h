#pragma once

#include "Compiler.h"
#include "Utilities/CompilerInterface.h"

#include <string>
#include <memory>


class ErrorHandler;

struct Options
{
	std::string SourceFile;
	std::string OutputFile;
	bool ListingOnly = false;
	bool Verbose = false;
};

class Driver final
{
public:
	Driver();

	bool CreateOptionsFromCLArguments(int argc, char* argv[]);
	void Start();
	void SetUI(std::unique_ptr<CompilerInterface>&& ui);

private:
	bool CheckSourceExtension(const std::string& filePath);
	bool CorrectOutExtension(std::string& filePath, const std::string& expected);
	void Terminate();

	bool Assemble();

private:
	Options m_Options;
	std::unique_ptr<Compiler> m_Compiler;
	std::shared_ptr<ErrorHandler> m_ErrorHandler;
	std::unique_ptr<CompilerInterface> m_UI;
};
