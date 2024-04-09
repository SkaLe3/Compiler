#ifndef COMPILERINTERFACE_H_
#define COMPILERINTERFACE_H_

#include "Errors/ErrorHandler.h"
#include "Lexer/Lexer.h"
#include "Log.h"

#include <memory>


class CompilerInterface
{
public:
	virtual void OutErrors() = 0;
	virtual void OutTokens() = 0;
	virtual void OutIdentifiersTable() = 0;
	virtual void OutConstantsTable() = 0;
	virtual void OutKeywordsTable() = 0;

	virtual void OutOptions() = 0;

	virtual void UsageHint(char* name) = 0;

	void SetErrorHandler(std::shared_ptr<ErrorHandler> errorHandler);
	void SetLexerData(std::shared_ptr<LexerData> lexerData);
	void SetInfoFileName(const std::string& fileName);
	void SetOutToFileEnabled(bool option);
protected:
	std::shared_ptr<ErrorHandler> m_ErrorHandler;
	std::shared_ptr<LexerData> m_LexerData;

	std::string m_InfoFileName;
	bool bOutToFile;
	std::ofstream m_Ofs;
};

class CLI : public CompilerInterface
{
public:
	~CLI();
	virtual void OutErrors() override;
	virtual void OutTokens() override;
	virtual void OutIdentifiersTable() override;
	virtual void OutConstantsTable() override;
	virtual void OutKeywordsTable() override;

	virtual void OutOptions() override;

	virtual void UsageHint(char* name) override;

private:

	void DisplayTable(const std::unordered_map<std::string, uint32_t>& table, const std::string& tableHeader);

};


#endif /* COMPILERINTERFACE_H_ */