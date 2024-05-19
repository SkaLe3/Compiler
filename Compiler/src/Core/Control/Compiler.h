#pragma once

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "CodeGenerator/Generator.h"

#include <string>
#include <vector>
#include <memory>

class ErrorHandler;
struct Token;

class Compiler 
{
public:
	Compiler(std::shared_ptr<ErrorHandler> errorHandler);
	void Compile(const std::string& inputfilePath, const std::string& outputfilePath);

	std::shared_ptr<LexerData> GetLexerData();
	Ref<ASTNode> GetAST();
	bool Assemble(const std::string& filePath);
	bool Link(const std::string& filePath);
private:
	bool ExecuteCommand(const std::string& command, std::string& output);


private:
	std::unique_ptr<Lexer> m_Lexer;
	std::unique_ptr<Parser> m_Parser;
	std::unique_ptr<Generator> m_Generator;
	std::shared_ptr<ErrorHandler> m_ErrorHandler;

	std::shared_ptr<std::vector<Token>>	 m_TokenSequence;
	Ref<ASTNode> m_AST;

};
