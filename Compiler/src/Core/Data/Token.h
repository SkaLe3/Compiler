#pragma once


#include <string>
#include <cstdint>

enum class ETokenCode : uint32_t
{
	None = 0,
	Eof,
	Empty,

	D_Dot = 46,
	D_Colon = 58,
	D_Semicolon = 59,
	D_Equal = 61,


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
template <typename T>
constexpr auto operator+(T e) noexcept
-> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>>
{
	return static_cast<std::underlying_type_t<T>>(e);
}


struct Token
{
	size_t Line;
	size_t Position;
	uint32_t Code;
	std::string Lexeme;

	bool IsIdentifier() { return Code > 1001; }
};
