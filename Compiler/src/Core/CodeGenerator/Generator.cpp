#include "Generator.h"
#include "Errors/ErrorHandler.h" 
#include "Data/Nodes.h"
#include "Data/SymbolTables.h"


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
	Emit(".386");
	Emit(".model flat, C");

	if (node.Program == nullptr)
	{
		Emit(".code");
		Emit("empty:");
		Emit("end empty");
		return;
	}
	SafeAccept(node.Program);

}

void Generator::Visit(NProgram& node)
{
	SafeAccept(node.ProcIdentifier);
	SafeAccept(node.Block);
}

void Generator::Visit(NBlock& node)
{
	Emit(".data");
	SafeAccept(node.VarDecl);
	Emit(".code");
	Emit("main proc");
	Emit("\tjmp " + m_ProcedureIdentifier);
	EmitLabel(m_ProcedureIdentifier);
	SafeAccept(node.StmtsList);
	Emit("main endp");
	Emit("end main");
}

void Generator::Visit(NVariableDeclarations& node)
{
	SafeAccept(node.DeclList);
}

void Generator::Visit(NDeclarationsList& node)
{
	SafeAccept(node.Decl);
	SafeAccept(node.DeclList);
}

void Generator::Visit(NDeclaration& node)
{
	SafeAccept(node.VarIdentifier);
	EmitVarDecl(m_LastIdentifier);
	m_Variables.insert(m_LastIdentifier);
}

void Generator::Visit(NAttribute& node)
{

}

void Generator::Visit(NStmt& node)
{

}

void Generator::Visit(NStmtsList& node)
{
	SafeAccept(node.Stmt);
	SafeAccept(node.StmtsList);
}

void Generator::Visit(NIfStmt& node)
{
	SafeAccept(node.CondStmt);
}

void Generator::Visit(NAssignStmt& node)
{
	m_Reg = "eax";
	SafeAccept(node.Expr);
	GenExprMov(node.Expr);

	SafeAccept(node.VarIdentifier);
	EmitCommandRegToVar("mov", m_LastIdentifier);

}

void Generator::Visit(NConditionStmt& node)
{
	uint32_t label = m_LabelCounter;

	SafeAccept(node.IncompleteCondStmt);
	EmitLabel(label);

	SafeAccept(node.AltPart);
	EmitLabel(++label);
}

void Generator::Visit(NIncompleteConditionStmt& node)
{
	SafeAccept(node.CondExpr);
	EmitJump("jne", m_LabelCounter++);

	SafeAccept(node.StmtsList);
	EmitJump("jmp", m_LabelCounter++);
}

void Generator::Visit(NAlternativePart& node)
{
	SafeAccept(node.StmtsList);
}

void Generator::Visit(NConditionalExpr& node)
{
	m_Reg = "eax";
	SafeAccept(node.Expr1);
	GenExprMov(node.Expr1);
	m_Reg = "ebx";
	SafeAccept(node.Expr2);
	GenExprMov(node.Expr2);
	EmitCommand("cmp", "eax", "ebx");
}

void Generator::Visit(NExpr& node)
{

}

void Generator::Visit(NVariableIdentifier& node)
{
	SafeAccept(node.Identifier);
}

void Generator::Visit(NProcedureIdentifier& node)
{
	SafeAccept(node.Identifier);
	m_ProcedureIdentifier = m_LastIdentifier;
}

void Generator::Visit(NIdentifier& node)
{
	m_LastIdentifier = Reverse_IdentifiersTable[node.Identifier];
}

void Generator::Visit(NConstant& node)
{
	m_LastConstant = Reverse_ConstantsTable[node.Val];
}

void Generator::Emit(const std::string& str)
{
	m_Outputfile << str << "\n";
}

void Generator::EmitVarDecl(const std::string& ident)
{
	std::string line;
	line += "\t" + ident + " DWORD ?";
	Emit(line);
}

void Generator::EmitLabel(const std::string& label)
{
	Emit(label + ":");
}
void Generator::EmitLabel(uint32_t label)
{
	Emit("?L" + std::to_string(label) + ":");
}

void Generator::EmitCommand(const std::string& command, const std::string& reg1, const std::string& reg2)
{
	Emit("\t" + command + " " + reg1 + ", " + reg2);
}

void Generator::EmitCommandVarToReg(const std::string& command, const std::string& variable)
{
	std::string line;
	line = "\t" + command + " " + m_Reg + ", " + "DWORD PTR [" + variable + "]";
	Emit(line);
}

void Generator::EmitCommandRegToVar(const std::string& command, const std::string& variable)
{
	std::string line;
	line = "\t" + command + " " + "DWORD PTR [" + variable + "]" + ", " + m_Reg;
	Emit(line);
}


void Generator::EmitCommandConst(const std::string& command, const std::string& value)
{
	std::string line;
	line = "\t" + command + " " + m_Reg + ", " + value;
	Emit(line);
}

void Generator::EmitJump(const std::string& jump, uint32_t label)
{
	std::string line;
	line = "\t" + jump + " ?L" + std::to_string(label);
	Emit(line);
}

Error Generator::CreateSemanticError(const std::string& errorMessage, const Token& token)
{
	return ErrorHandler::CreateSemanticError(errorMessage, token, m_Instigator);
}

void Generator::GenExprMov(Ref<ASTNode> node)
{
	if (Ref<NVariableIdentifier> variable =  dynamic_pointer_cast<NVariableIdentifier>(node); variable)
	{
		CheckIfDeclared(m_LastIdentifier, static_pointer_cast<NIdentifier>(variable->Identifier)->token);
		EmitCommandVarToReg("mov", m_LastIdentifier);
	}
	else
	{
		EmitCommandConst("mov", m_LastConstant);
	}
}

void Generator::CheckIfDeclared(const std::string& ident, const Token& token)
{
	auto it = m_Variables.find(ident);
	if (it == m_Variables.end())
	{
		auto error = CreateSemanticError("'" + ident + "': undeclared identifier", token);
		m_ErrorHandler->ReportError(error);
		m_ErrorHandler->GotFatalError();
	}
}

