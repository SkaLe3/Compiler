#pragma once

#include "Data/ASTNode.h"

struct NSignalProgram;
struct NProgram;
struct NBlock;
struct NProcedureIdentifier;
struct NVariableIdentifier;
struct NVariableDeclarations;
struct NDeclarationsList;
struct NDeclaration;
struct NAttribute;
struct NStmt;
struct NIfStmt;
struct NAssignStmt;
struct NStmtsList;
struct NConditionStmt;
struct NIncompleteConditionStmt;
struct NAlternativePart;
struct NConditionalExpr;
struct NExpr;
struct NVariableIdentifier;
struct NIdentifier;
struct NConstant;

namespace AST
{
	class Visitor
	{
	public:
		virtual ~Visitor() = default;
		virtual void Visit(NSignalProgram& node) = 0;
		virtual void Visit(NProgram& node) = 0;
		virtual void Visit(NBlock& node) = 0;
		virtual void Visit(NVariableDeclarations& node) = 0;
		virtual void Visit(NDeclarationsList& node) = 0;
		virtual void Visit(NDeclaration& node) = 0;
		virtual void Visit(NAttribute& node) = 0;
		virtual void Visit(NStmt& node) = 0;
		virtual void Visit(NStmtsList& node) = 0;
		virtual void Visit(NIfStmt& node) = 0;
		virtual void Visit(NAssignStmt& node) = 0;
		virtual void Visit(NConditionStmt& node) = 0;
		virtual void Visit(NIncompleteConditionStmt& node) = 0;
		virtual void Visit(NAlternativePart& node) = 0;
		virtual void Visit(NConditionalExpr& node) = 0;
		virtual void Visit(NExpr& node) = 0;
		virtual void Visit(NVariableIdentifier& node) = 0;
		virtual void Visit(NProcedureIdentifier& node) = 0;
		virtual void Visit(NIdentifier& node) = 0;
		virtual void Visit(NConstant& node) = 0;
	};
}