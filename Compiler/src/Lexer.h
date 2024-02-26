#ifndef LEXER_H_
#define LEXER_H_

#include <cstdint>
#include <fstream>
#include <array>
#include <vector>
#include <iostream> //temp
#include <unordered_map>

#include "Token.h"

enum class ESymbolCategories : uint8_t
{
	None = 0,
	WhiteSpace,
	Identifier,
	Constant,
	UnaryDelimiter,
	MultiDelimiter,
	Comment,
	End

};

struct LexerData
{
	std::vector<Token> Tokens;
	std::unordered_map<std::string, uint32_t> ConstantsTable;
	std::unordered_map<std::string, uint32_t> IdentifiersTable;
	std::unordered_map<std::string, uint32_t> KeyWordsTable;
};

class ErrorHandler;

class Lexer 
{
public:
	Lexer(std::shared_ptr<ErrorHandler> errorHandler);

	void Scan(const std::string& filePath);

	std::shared_ptr<LexerData> GetLexerData();

private:
	void Next();

	void SetupSymbolCategories();
	void SetupKeywordTable();

	void WhiteSpaceState();
	void IdentifierState();
	void ConstantState();
	void UnaryDelimiterState();
	void MultiDelimiterState();

	void CommentState();
	void InCommentState(size_t line, size_t pos);
	void EndCommentState(size_t line, size_t pos);

private:
	std::ifstream m_InputFile;
	
	// Lexer state
	size_t m_Line;
	size_t m_Position;
	char m_CurrentCharacter;
	ESymbolCategories m_CurrentSymbol;
	std::array<ESymbolCategories, 128> m_Attributes;
	
	// Token-related
	std::string m_TokenBuffer;
	std::vector<Token> m_Tokens;

	// Tables
	std::unordered_map<std::string, uint32_t> m_ConstantsTable;
	std::unordered_map<std::string, uint32_t> m_IdentifiersTable;
	std::unordered_map<std::string, uint32_t> m_KeyWordsTable;

	std::shared_ptr<ErrorHandler> m_ErrorHandler;
};

#endif /* LEXER_H_ */