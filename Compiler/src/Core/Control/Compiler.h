#pragma once

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

#include <string>
#include <vector>
#include <memory>

class ErrorHandler;
struct Token;

class Compiler 
{
public:
	Compiler(std::shared_ptr<ErrorHandler> errorHandler);
	void Compile(const std::string& filePath);

	std::shared_ptr<LexerData> GetLexerData();
	Ref<ASTNode> GetAST();
private:



private:
	std::unique_ptr<Lexer> m_Lexer;
	std::unique_ptr<Parser> m_Parser;
	std::shared_ptr<ErrorHandler> m_ErrorHandler;

	std::shared_ptr<std::vector<Token>>	 m_TokenSequence;
	Ref<ASTNode> m_AST;

};
