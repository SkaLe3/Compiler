#include "ErrorHandler.h"
#include "Data/Token.h"

#include <sstream>
#include <memory>

ErrorHandler::ErrorHandler() : bFatalError(false)
{
	m_Errors = std::make_shared<std::vector<Error>>();
}

void ErrorHandler::ReportError(Error error)
{
	m_Errors->push_back(error);
}

std::shared_ptr<std::vector<Error>> ErrorHandler::GetErrors()
{
	return m_Errors;
}

Error ErrorHandler::CreateSyntaxError(const std::string& errorMessage, uint32_t line, uint32_t pos, EErrorInstigator instigator)
{
	return CreateError(errorMessage, line, pos, instigator, EErrorType::SyntaxError);
}

Error ErrorHandler::CreateSyntaxError(const std::string& errorMessage, const Token& token, EErrorInstigator instigator)
{
	if ((ETokenCode)token.Code == ETokenCode::Eof)
	{
		return CreateError(" at end; " + errorMessage, token, instigator, EErrorType::SyntaxError);
	}
	else
	{
		return CreateError(" at '" + token.Lexeme + "'; " + errorMessage, token, instigator, EErrorType::SyntaxError );
	}
}

Error ErrorHandler::CreateGeneralError(const std::string& errorMessage, EErrorInstigator instigator)
{
	return CreateError(errorMessage, 0, 0, instigator, EErrorType::DriverError);
}

void ErrorHandler::GotFatalError()
{
	bFatalError = true;
}

bool ErrorHandler::HasFatalError()
{
	return bFatalError;
}

Error ErrorHandler::CreateError(const std::string& errorMessage, uint32_t line, uint32_t pos, EErrorInstigator instigator, EErrorType type)
{
	return { errorMessage, line, pos, instigator, type };
}

Error ErrorHandler::CreateError(const std::string& errorMessage, const Token& token, EErrorInstigator instigator, EErrorType type)
{
	return { errorMessage, token.Line, token.Position, instigator, type };
}
