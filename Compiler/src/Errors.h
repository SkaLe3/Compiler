#ifndef ERRORS_H_
#define ERRORS_H_

#include <string>

class Error
{
public:
	Error(const std::string& msg);
	std::string GetMessage() { return m_Message; }

protected:
	// @TODO: add instigator, position, type
	std::string m_Message;
};


#endif /* ERRORS_H_*/