#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <cstdint>

enum class ETokenCode : uint32_t
{
	None = 0,
	DelimiterBase = 301,
	DelimiterAssign,

	KW_Base = 401,
	KW_PROGRAM,
	KW_VAR,
	KW_BEGIN,
	KW_END,
	KW_INTEGER,
	KW_FLOAT,
	KW_IF,
	KW_THEN,
	KW_ELSE,
	KW_ENDIF,
	
	ConstantBase = 501,
	IdentifierBase = 1001
};


struct Token
{
	size_t Line;
	size_t Position;
	uint32_t Code;
	std::string Lexeme;
};

#endif /* TOKEN_H_ */