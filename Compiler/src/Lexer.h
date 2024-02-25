#ifndef LEXER_H_
#define LEXER_H_

#include <cstdint>
#include <fstream>
#include <array>
#include <vector>
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

class Lexer 
{
public:
	Lexer();

	void Scan(const std::string& filePath);
	void Next();

private:
	void SetupSymbolCategories();
	void SetupKeywordTable();

	void WhiteSpaceState();
	void IdentifierState();
	void ConstantState();
	void UnaryDelimiterState();
	void MultiDelimiterState();
	void CommentState();
private:

	size_t m_Line;
	size_t m_Position;
	std::string m_TokenBuffer;

	std::ifstream m_InputFile;

	char m_CurrentCharacter;
	ESymbolCategories m_CurrentSymbol;

	std::array<ESymbolCategories, 128> m_Attributes;

	std::vector<Token> m_Tokens;

	std::unordered_map<std::string, uint32_t> m_ConstantsTable;
	std::unordered_map<std::string, uint32_t> m_IdentifiersTable;
	std::unordered_map<std::string, uint32_t> m_KeyWordsTable;



};

#endif /* LEXER_H_ */