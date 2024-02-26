#include "Driver.h"
#include "ErrorHandler.h"
#include "Errors.h"
#include "Lexer.h"
// temp
#include <iostream>
Driver::Driver()
{
	m_ErrorHandler = std::make_shared<ErrorHandler>();
	m_Compiler = std::make_unique<Compiler>(m_ErrorHandler);
}

void Driver::CreateOptionsFromCLArguments(int argc, char* argv[])
{
	// @TODO: Check flags 
	if (argc > 1)
		m_Options.SourceFile = argv[1];
	else
		// @TODO: Ask View for filename input
		m_Options.SourceFile = "test01.sig";

}

void Driver::Start()
{
	m_Compiler->Compile(m_Options.SourceFile);
	m_UI->SetLexerData(m_Compiler->GetLexerData());
	m_UI->DisplayErrors();
	m_UI->DisplayTokens();
	m_UI->DisplayIdentifiersTable();
}

void Driver::SetUI(std::unique_ptr<CompilerInterface>&& ui)
{
	m_UI = std::move(ui);
	m_UI->SetErrorHandler(m_ErrorHandler);
}
