#ifndef COMPILER_H_
#define COMPILER_H_

#include "Lexer.h"
#include <string>

class ErrorHandler;

class Compiler 
{
public:
	Compiler(std::shared_ptr<ErrorHandler> errorHandler);
	void Compile(const std::string& filePath);

	std::shared_ptr<LexerData> GetLexerData();
private:

	void CheckSourceExtension(const std::string& filePath);

private:
	std::unique_ptr<Lexer> m_Lexer; // May be store in shared_ptr and create in driver
	std::shared_ptr<ErrorHandler> m_ErrorHandler;

};

	// @TODO:
	// Parser
	// SemanticAnalyzer
	// CodeGenerator
	// ErrorHandler
#endif /* COMPILER_H_ */
