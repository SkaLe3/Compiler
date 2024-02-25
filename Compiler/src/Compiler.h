#ifndef COMPILER_H_
#define COMPILER_H_

#include "Lexer.h"
#include <string>

class Compiler 
{
public:
	void Compile(const std::string& filePath);
private:

private:
	Lexer m_Lexer; // May be store in shared_ptr and create in driver

	// @TODO:
	// Parser
	// SemanticAnalyzer
	// CodeGenerator
	// ErrorHandler
};

#endif /* COMPILER_H_ */
