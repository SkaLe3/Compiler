#ifndef ERROR_HANDLER_H_
#define ERROR_HANDLER_H_

#include <vector>
#include <string>
#include <memory>

#include "Errors.h"

class ErrorHandler
{
public:
	ErrorHandler();

	void ReportError(Error error);
	std::shared_ptr<std::vector<Error>> GetErrors();

	static Error CreateSyntaxError(const std::string& instigator, const std::string& errorMessage, uint32_t line, uint32_t pos);
	static Error CreateGeneralError(const std::string& instigator, const std::string& errorMessage);

private:
	std::shared_ptr<std::vector<Error>> m_Errors;
};

#endif /* ERROR_HANDLER_H_ */