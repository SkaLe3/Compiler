#include "Generator.h"
#include "Errors/ErrorHandler.h" 
#include "Data/Nodes.h"





Ref<ASTNode> Generator::m_AST;
std::string Generator::m_OutputfilePath;

Generator::Generator(std::shared_ptr<ErrorHandler> errorHandler)
	: m_ErrorHandler(errorHandler), m_Instigator(EErrorInstigator::CodeGenerator)
{

}

void Generator::SetInOut(Ref<ASTNode> ast, const std::string& filePath)
{
	m_AST = ast;
	m_OutputfilePath = filePath;
}

void Generator::Generate()
{
	m_Outputfile.open(m_OutputfilePath, std::ios::out);
	if (!m_Outputfile.is_open())
	{
		auto error = ErrorHandler::CreateGeneralError(std::string("Can't open file for write: ") + m_OutputfilePath, EErrorInstigator::FileIO);
		m_ErrorHandler->ReportError(error);
		m_ErrorHandler->GotFatalError();
		return;
	}

	Ref<NSignalProgram> program = static_pointer_cast<NSignalProgram>(m_AST);
	if (program)
		SafeAccept(static_pointer_cast<NSignalProgram>(m_AST));

	m_Outputfile.close();
}

void Generator::Visit(NSignalProgram& node)
{
	SafeAccept(node.Program);
}

void Generator::Visit(NProgram& node)
{
	Emit(".386");
	Emit(".model flat, C");

}

void Generator::Visit(NBlock& node)
{

}

void Generator::Visit(NVariableDeclarations& node)
{

}

void Generator::Visit(NDeclarationsList& node)
{

}

void Generator::Visit(NDeclaration& node)
{

}

void Generator::Visit(NAttribute& node)
{

}

void Generator::Visit(NStmt& node)
{

}

void Generator::Visit(NStmtsList& node)
{

}

void Generator::Visit(NIfStmt& node)
{

}

void Generator::Visit(NAssignStmt& node)
{

}

void Generator::Visit(NConditionStmt& node)
{

}

void Generator::Visit(NIncompleteConditionStmt& node)
{

}

void Generator::Visit(NAlternativePart& node)
{

}

void Generator::Visit(NConditionalExpr& node)
{

}

void Generator::Visit(NExpr& node)
{

}

void Generator::Visit(NVariableIdentifier& node)
{

}

void Generator::Visit(NProcedureIdentifier& node)
{

}

void Generator::Visit(NIdentifier& node)
{

}

void Generator::Visit(NConstant& node)
{

}

void Generator::Emit(const std::string& str)
{
	m_Outputfile << str << "\n";
}