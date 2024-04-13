#ifndef ERROR_HANDLER_H_
#define ERROR_HANDLER_H_

#include "Error.h"

#include <vector>
#include <string>
#include <memory>

struct Token;

class ErrorHandler
{
public:
	ErrorHandler();

	void ReportError(Error error);
	std::shared_ptr<std::vector<Error>> GetErrors();

	static Error CreateSyntaxError(const std::string& errorMessage, uint32_t line, uint32_t pos, EErrorInstigator instigator);
	static Error CreateSyntaxError(const std::string& errorMessage, const Token& token, EErrorInstigator instigator);
	static Error CreateGeneralError(const std::string& errorMessage, EErrorInstigator instigator);

	void GotFatalError();
	bool HasFatalError();

private:

	static Error CreateError(const std::string& errorMessage, uint32_t line, uint32_t pos, EErrorInstigator instigator, EErrorType type);
	static Error CreateError(const std::string& errorMessage,const Token& token, EErrorInstigator instigator, EErrorType type);
private:
	std::shared_ptr<std::vector<Error>> m_Errors;
	bool bFatalError;
};

#endif /* ERROR_HANDLER_H_ */