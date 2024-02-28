#include "Error.h"

Error::Error(const std::string& msg, size_t line, size_t pos, EErrorInstigator inst, EErrorType type) 
	: m_Message(msg), m_Line(line), m_Position(pos), m_Instigator(inst), m_ErrorType(type)
{

}

std::string Error::GetMessage()
{
	return m_Message;
}

std::string Error::GetInstigator()
{
	return InstigatorToString(m_Instigator);
}

std::string Error::GetType()
{
	return TypeToString(m_ErrorType);
}

std::string Error::GetLine()
{
	return std::to_string(m_Line);
}

std::string Error::GetPosition()
{
	return std::to_string(m_Position);
}

EErrorInstigator Error::GetEnumInstigator()
{
	return m_Instigator;
}

std::string Error::InstigatorToString(EErrorInstigator instigator)
{
	switch (instigator)
	{
	case EErrorInstigator::FileIO:
		return "File IO";
	case EErrorInstigator::Lexer:
		return "Lexer";
	case EErrorInstigator::Parser:
		return "Parser";
	case EErrorInstigator::CodeGenerator:
		return "Code Generator";
	default:
		return "Unknown";
	}
}

std::string Error::TypeToString(EErrorType type)
{
	switch (type)
	{
	case EErrorType::SyntaxError:
		return "syntax error";
	case EErrorType::DriverError:
		return "driver error";
	default:
		return "unknown error";
	}
}
