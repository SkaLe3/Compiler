#include "Compiler.h"

#include <iostream>

void Compiler::Compile(const std::string& filePath)
{
	// @TODO check for proper file type and make error
	m_Lexer.Scan(filePath);
}
