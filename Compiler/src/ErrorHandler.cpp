#include "ErrorHandler.h"
#include "Errors.h"

#include <sstream>
#include <memory>

ErrorHandler::ErrorHandler()
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

Error ErrorHandler::CreateSyntaxError(const std::string& instigator, const std::string& errorMessage, uint32_t line, uint32_t pos)
{
	std::stringstream ss;
	ss <<"[" << instigator << "]" << "(" << line << "," << pos << "): " << "syntax error: " + errorMessage;
	return Error(ss.str());

}

Error ErrorHandler::CreateGeneralError(const std::string& instigator, const std::string& errorMessage)
{
	std::stringstream ss;
	ss << "[" << instigator << "] " << "driver error: " + errorMessage;
	return Error(ss.str());
}
