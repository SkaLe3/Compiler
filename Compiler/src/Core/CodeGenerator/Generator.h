#pragma once
#include "Data/Visitor.h"
#include "Errors/Error.h"

#include <fstream>
#include <string>
#include <unordered_set>

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
	void EmitVarDecl(const std::string& ident);
	void EmitLabel(const std::string& label);
	void EmitLabel(uint32_t label);
	void EmitCommand(const std::string& command, const std::string& reg1, const std::string& reg2);
	void EmitCommandVarToReg(const std::string& command, const std::string& variable);
	void EmitCommandRegToVar(const std::string& command, const std::string& variable);
	void EmitCommandConst(const std::string& command, const std::string& value);
	void EmitJump(const std::string& jump, uint32_t label);
	Error CreateSemanticError(const std::string& errorMessage, const Token& token);

	void GenExprMov(Ref<ASTNode> node);
	void CheckIfDeclared(const std::string& ident, const Token& token);

private:
	static Ref<ASTNode> m_AST;
	static std::string m_OutputfilePath;
	std::fstream m_Outputfile;

	std::shared_ptr<ErrorHandler> m_ErrorHandler;
	EErrorInstigator m_Instigator;

private:	// States
	std::string	m_LastIdentifier;
	std::string m_LastConstant;
	std::string m_ProcedureIdentifier;
	std::string m_Reg = "eax";
	uint32_t m_LabelCounter = 0;

	std::unordered_set<std::string> m_Variables;

};