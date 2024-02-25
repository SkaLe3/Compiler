#include "Lexer.h"
#include <iostream>

Lexer::Lexer() : m_Line(1), m_Position(0), m_CurrentCharacter(0)
{
	SetupSymbolCategories();
	SetupKeywordTable();
}


void Lexer::Scan(const std::string& filePath)
{
	m_InputFile.open(filePath);

	if (!m_InputFile.is_open())
	{
		// @TODO: Handle error
		return;
	}
	Next();

	while (m_CurrentSymbol != ESymbolCategories::End)
	{
		switch (m_CurrentSymbol)
		{
		case ESymbolCategories::WhiteSpace:
			WhiteSpaceState();
			break;
		case ESymbolCategories::Identifier:
			IdentifierState();
			break;
		case ESymbolCategories::Constant:
			ConstantState();
			break;
		case ESymbolCategories::UnaryDelimiter:
			UnaryDelimiterState();
			break;
		case ESymbolCategories::MultiDelimiter:
			MultiDelimiterState();
			break;
		case ESymbolCategories::Comment:
			CommentState();
			break;
		default:
			// Report error
			break;
		}

	}
	std::cout << "Tokens:\n";
	int32_t tokenInd = 0;
	for (Token& token : m_Tokens)
	{
		std::cout << "[" << tokenInd << "] <" << token.Lexeme << "> = " << token.Code << " | " << token.Line << " : " << token.Position << "\n";
		tokenInd++;
	}

}

void Lexer::Next()
{
	m_CurrentCharacter = m_InputFile.get();
	if (m_CurrentCharacter == '\n')
	{
		m_Line++;
		m_Position = 0;
	}
	else if (m_CurrentCharacter == '\t')
	{
		m_Position += 4;
	}
	else
	{
		m_Position++;
	}
	if (m_CurrentCharacter != EOF)
	{
		m_CurrentSymbol = m_Attributes[m_CurrentCharacter];
	}
	else
	{
		m_CurrentSymbol = ESymbolCategories::End;
	}
}

void Lexer::SetupSymbolCategories()
{
	m_Attributes.fill(ESymbolCategories::None);

	for (size_t i = 8; i < 14; i++)
		m_Attributes[i] = ESymbolCategories::WhiteSpace;
	m_Attributes[32] = ESymbolCategories::WhiteSpace;

	for (size_t i = 48; i < 58; i++)
		m_Attributes[i] = ESymbolCategories::Constant;

	for (size_t i = 64; i < 91; i++)
		m_Attributes[i] = ESymbolCategories::Identifier; // [@-A-Z]
	for (size_t i = 97; i < 123; i++)
		m_Attributes[i] = ESymbolCategories::Identifier; // [a-z]
	m_Attributes['_'] = ESymbolCategories::Identifier;

	m_Attributes['('] = ESymbolCategories::Comment;

	m_Attributes[':'] = ESymbolCategories::MultiDelimiter;
	m_Attributes[';'] = ESymbolCategories::UnaryDelimiter;
	m_Attributes['='] = ESymbolCategories::UnaryDelimiter;
	m_Attributes['.'] = ESymbolCategories::UnaryDelimiter;
}

void Lexer::SetupKeywordTable()
{
	m_KeyWordsTable["PROGRAM"]	= static_cast<uint32_t>(ETokenCode::KW_PROGRAM);
	m_KeyWordsTable["VAR"]		= static_cast<uint32_t>(ETokenCode::KW_VAR);
	m_KeyWordsTable["BEGIN"]	= static_cast<uint32_t>(ETokenCode::KW_BEGIN);
	m_KeyWordsTable["END"]		= static_cast<uint32_t>(ETokenCode::KW_END);
	m_KeyWordsTable["INTEGER"]	= static_cast<uint32_t>(ETokenCode::KW_INTEGER);
	m_KeyWordsTable["FLOAT"]	= static_cast<uint32_t>(ETokenCode::KW_FLOAT);
	m_KeyWordsTable["IF"]		= static_cast<uint32_t>(ETokenCode::KW_IF);
	m_KeyWordsTable["THEN"]		= static_cast<uint32_t>(ETokenCode::KW_THEN);
	m_KeyWordsTable["ELSE"]		= static_cast<uint32_t>(ETokenCode::KW_ELSE);
	m_KeyWordsTable["ENDIF"]	= static_cast<uint32_t>(ETokenCode::KW_ENDIF);
}

void Lexer::WhiteSpaceState()
{
	while (m_CurrentSymbol == ESymbolCategories::WhiteSpace)
	{
		Next();
	}
}

void Lexer::IdentifierState()
{
	size_t lexemeLine = m_Line;
	size_t lexemeStartPosition = m_Position;

	while (m_CurrentSymbol == ESymbolCategories::Identifier ||
		m_CurrentSymbol == ESymbolCategories::Constant)
	{
		m_TokenBuffer += m_CurrentCharacter;
		Next();
	}

	uint32_t lexemeCode;

	if (auto kwRecord = m_KeyWordsTable.find(m_TokenBuffer); kwRecord != m_KeyWordsTable.end())
	{
		lexemeCode = kwRecord->second;
	}
	else if (auto identifierRecord = m_IdentifiersTable.find(m_TokenBuffer); identifierRecord == m_IdentifiersTable.end())
	{
		lexemeCode = static_cast<uint32_t>(ETokenCode::IdentifierBase) + m_IdentifiersTable.size(); // @TODO: Create function
		m_IdentifiersTable[m_TokenBuffer] = lexemeCode;
	}
	else
	{
		lexemeCode = identifierRecord->second;
	}
	
	m_Tokens.emplace_back(lexemeLine, lexemeStartPosition, lexemeCode, m_TokenBuffer);
	m_TokenBuffer.clear();
}

void Lexer::ConstantState()
{
	size_t lexemeLine = m_Line;
	size_t lexemeStartPosition = m_Position;

	while (m_CurrentSymbol == ESymbolCategories::Constant)
	{
		m_TokenBuffer += m_CurrentCharacter;
		Next();
	}
	uint32_t lexemeCode;

	if (auto tableRecord = m_ConstantsTable.find(m_TokenBuffer); tableRecord == m_ConstantsTable.end())
	{
		lexemeCode = static_cast<uint32_t>(ETokenCode::ConstantBase) + m_ConstantsTable.size(); // @TODO: Create function
		m_ConstantsTable[m_TokenBuffer] = lexemeCode;
	}
	else
	{
		lexemeCode = tableRecord->second;
	}


	m_Tokens.emplace_back(lexemeLine, lexemeStartPosition, lexemeCode, m_TokenBuffer);
	m_TokenBuffer.clear();
}

void Lexer::UnaryDelimiterState()
{

}

void Lexer::MultiDelimiterState()
{

}

void Lexer::CommentState()
{

}

