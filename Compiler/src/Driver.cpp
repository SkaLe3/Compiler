#include "Driver.h"

Driver::Driver()
{

}

void Driver::CreateOptionsFromCLI(int argc, char* argv[])
{
	if (argc > 1)
		m_Options.SourceFile = argv[1];
	else
		m_Options.SourceFile = "test01.sig";

}

void Driver::Start()
{
	m_Compiler.Compile(m_Options.SourceFile);
}
