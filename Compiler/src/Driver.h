#ifndef DRIVER_H_
#define DRIVER_H_

#include "Compiler.h"
#include <string>

struct Options
{
	std::string SourceFile;
};

class Driver final
{
public:
	Driver();

	void CreateOptionsFromCLI(int argc, char* argv[]);
	void Start();

private:
	Compiler m_Compiler;
	Options m_Options;

};

#endif /* DRIVER_H_ */