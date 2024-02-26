#ifndef DRIVER_H_
#define DRIVER_H_
 
#include "Compiler.h"
#include "CompilerInterface.h"

#include <string>
#include <memory>


class ErrorHandler;

struct Options
{
	std::string SourceFile;
};

class Driver final
{
public:
	Driver();

	void CreateOptionsFromCLArguments(int argc, char* argv[]);
	void Start();
	void SetUI(std::unique_ptr<CompilerInterface>&& ui);

private:
	Options m_Options;
	std::unique_ptr<Compiler> m_Compiler;
	std::shared_ptr<ErrorHandler> m_ErrorHandler;
	std::unique_ptr<CompilerInterface> m_UI;
};

#endif /* DRIVER_H_ */