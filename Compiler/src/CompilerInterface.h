#ifndef COMPILERINTERFACE_H_
#define COMPILERINTERFACE_H_

#include "ErrorHandler.h"
#include "Lexer.h"

#include <memory>

class CompilerInterface
{
public:
	virtual void DisplayErrors() = 0;
	virtual void DisplayTokens() = 0;
	virtual void DisplayIdentifiersTable() = 0;
	void SetErrorHandler(std::shared_ptr<ErrorHandler> errorHandler);
	void SetLexerData(std::shared_ptr<LexerData> lexerData);
protected:
	std::shared_ptr<ErrorHandler> m_ErrorHandler;
	std::shared_ptr<LexerData> m_LexerData;
};

class CLInterface : public CompilerInterface
{
public:
	virtual void DisplayErrors() override;
	virtual void DisplayTokens() override;
	virtual void DisplayIdentifiersTable() override;

private:


};


#endif /* COMPILERINTERFACE_H_ */