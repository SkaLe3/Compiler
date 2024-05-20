#pragma once
#include "Data/Visitor.h"
#include "Errors/Error.h"

#include <fstream>
#include <string>
#include <unordered_set>

#define SafeAccept(expr) do { if (IsValid(expr.get())) { (expr)->Accept(*this); } } while (false)
#define VISIT(type) virtual void Visit(type node)

class ErrorHandler;

class Generator : public AST::Visitor
{
public:
	Generator(std::shared_ptr<ErrorHandler> errorHandler);
	static void SetInOut(Ref<ASTNode> ast, const std::string& filePath);
	void Generate();

	VISIT(NSignalProgram&);
	VISIT(NProgram&);
	VISIT(NBlock&);
	VISIT(NVariableDeclarations&);
	VISIT(NDeclarationsList&);
	VISIT(NDeclaration&);
	VISIT(NAttribute&);
	VISIT(NStmt&);
	VISIT(NStmtsList&);
	VISIT(NIfStmt&);
	VISIT(NAssignStmt&);
	VISIT(NConditionStmt&);
	VISIT(NIncompleteConditionStmt&);
	VISIT(NAlternativePart&);
	VISIT(NConditionalExpr&);
	VISIT(NExpr&);
	VISIT(NVariableIdentifier&);
	VISIT(NProcedureIdentifier&);
	VISIT(NIdentifier&);
	VISIT(NConstant&);


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