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



private:
	std::unique_ptr<Lexer> m_Lexer; // May be store in shared_ptr and create in driver
	std::shared_ptr<ErrorHandler> m_ErrorHandler;

};

#endif /* COMPILER_H_ */
