#pragma once
#include "Data/Visitor.h"
#include "Errors/Error.h"

#include <fstream>
#include <string>

#define SafeAccept(expr) do { if (IsValid(expr.get())) { (expr)->Accept(*this); } } while (false)

class ErrorHandler;

class Generator : public AST::Visitor
{
public:
	Generator(std::shared_ptr<ErrorHandler> errorHandler);
	static void SetInOut(Ref<ASTNode> ast, const std::string& filePath);
	void Generate();

	virtual void Visit(NSignalProgram& node);
	virtual void Visit(NProgram& node);
	virtual void Visit(NBlock& node);
	virtual void Visit(NVariableDeclarations& node);
	virtual void Visit(NDeclarationsList& node);
	virtual void Visit(NDeclaration& node);
	virtual void Visit(NAttribute& node);
	virtual void Visit(NStmt& node);
	virtual void Visit(NStmtsList& node);
	virtual void Visit(NIfStmt& node);
	virtual void Visit(NAssignStmt& node);
	virtual void Visit(NConditionStmt& node);
	virtual void Visit(NIncompleteConditionStmt& node);
	virtual void Visit(NAlternativePart& node);
	virtual void Visit(NConditionalExpr& node);
	virtual void Visit(NExpr& node);
	virtual void Visit(NVariableIdentifier& node);
	virtual void Visit(NProcedureIdentifier& node);
	virtual void Visit(NIdentifier& node);
	virtual void Visit(NConstant& node);

private:
	void Emit(const std::string& str);

private:
	static Ref<ASTNode> m_AST;
	static std::string m_OutputfilePath;
	std::fstream m_Outputfile;

	std::shared_ptr<ErrorHandler> m_ErrorHandler;
	EErrorInstigator m_Instigator;
};