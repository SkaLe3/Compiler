#pragma once

#include "Data/Visitor.h"
#include "Data/Token.h"
#include "Data/ASTNode.h"

#include <sstream>
#include <string>

#define SafeAcceptPrint(expr) do { if (IsValid(expr.get())) { (expr)->Accept(*this); } else { PrintNullptr(); } } while (false)

namespace AST
{
	class PrintVisitor : public Visitor
	{
	public:

		std::string Print(Ref<ASTNode> ast);

		virtual void Visit(NSignalProgram& node) override;
		virtual void Visit(NProgram& node) override;
		virtual void Visit(NBlock& node) override;
		virtual void Visit(NVariableDeclarations& node) override;
		virtual void Visit(NDeclarationsList& node) override;
		virtual void Visit(NDeclaration& node) override;
		virtual void Visit(NAttribute& node) override;
		virtual void Visit(NStmt& node) override;
		virtual void Visit(NStmtsList& node) override;
		virtual void Visit(NIfStmt& node) override;
		virtual void Visit(NAssignStmt& node) override;
		virtual void Visit(NConditionStmt& node) override;
		virtual void Visit(NIncompleteConditionStmt& node) override;
		virtual void Visit(NAlternativePart& node) override;
		virtual void Visit(NConditionalExpr& node) override;
		virtual void Visit(NExpr& node) override;
		virtual void Visit(NVariableIdentifier& node) override;
		virtual void Visit(NProcedureIdentifier& node) override;
		virtual void Visit(NIdentifier& node) override;
		virtual void Visit(NConstant& node) override;

	public:
		void SetDelimeter(const std::string& delim) { m_Delim = delim; }
	private:
		void AddOffset();
		void RemoveOffset();

		void PrintEmpty();
		void PrintAttribute(const std::string& str);
		void StartNode(const std::string& str);
		void EndNode();

		void PrintNullptr();
		std::string KeywordToString(ETokenCode key);
		std::string ConstantToString(uint32_t key);
		std::string IdentifierToString(uint32_t key);
		std::string DelimToString(ETokenCode key);
	private:
		std::stringstream m_SS;
		std::string m_Delim = "-";
		std::string m_Offset;
	};
}