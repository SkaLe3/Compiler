#include "PrintVisitor.h"

#include "Data/SymbolTables.h"
#include "Data/Nodes.h"
#include "Utilities/Log.h"

std::string AST::PrintVisitor::Print(Ref<ASTNode> ast)
{
	Ref<NSignalProgram> program = static_pointer_cast<NSignalProgram>(ast);
	if (program)
		SafeAccept(program);
	else
	{
		m_SS << "Empty translation unit!";
	}
	return m_SS.str();
}

void AST::PrintVisitor::Visit(NSignalProgram& node)
{
	m_SS << "|<signal-program>\n";

	SafeAccept(node.Program);

}

void AST::PrintVisitor::Visit(NProgram& node)
{
	StartNode("<program>");
	
	PrintAttribute(KeywordToString(node.Program));
	SafeAccept(node.ProcIdentifier);	   
	PrintAttribute(DelimToString(node.Semicolon));
	SafeAccept(node.Block);
	PrintAttribute(DelimToString(node.Dot));

	EndNode();
}

void AST::PrintVisitor::Visit(NBlock& node)
{
	StartNode("<block>");

	SafeAccept(node.VarDecl);
	PrintAttribute(KeywordToString(node.Begin));
	SafeAccept(node.StmtsList);
	PrintAttribute(KeywordToString(node.End));

	EndNode();
}

void AST::PrintVisitor::Visit(NVariableDeclarations& node)
{
	StartNode("<variable-declarations>");

	if (node.Empty)
	{
		PrintEmpty();
		EndNode();
		return;
	}

	PrintAttribute(KeywordToString(node.Var));
	SafeAccept(node.DeclList);

	EndNode();
}

void AST::PrintVisitor::Visit(NDeclarationsList& node)
{
	StartNode("<declarations-list>");

	if (node.Empty)
	{
		PrintEmpty();
		EndNode();
		return;
	}
	SafeAccept(node.Decl);
	SafeAccept(node.DeclList);

	EndNode();
}

void AST::PrintVisitor::Visit(NDeclaration& node)
{
	StartNode("<declaration>");

	SafeAccept(node.VarIdentifier);
	PrintAttribute(DelimToString(node.Colon));
	SafeAccept(node.Attribute);
	PrintAttribute(DelimToString(node.Semicolon));

	EndNode();
}

void AST::PrintVisitor::Visit(NAttribute& node)
{
	StartNode("<attribute>");

	PrintAttribute(KeywordToString(node.Type));

	EndNode();
}

void AST::PrintVisitor::Visit(NStmtsList& node)
{
	StartNode("<statements-list>");

	if (node.Empty)
	{
		PrintEmpty();
		EndNode();
		return;
	}

	SafeAccept(node.Stmt);
	SafeAccept(node.StmtsList);
	EndNode();
}

void AST::PrintVisitor::Visit(NStmt& node)
{
	StartNode("<statement>");



	EndNode();
}



void AST::PrintVisitor::Visit(NIfStmt& node)
{
	StartNode("<if-statement>");

	SafeAccept(node.CondStmt);
	PrintAttribute(KeywordToString(node.Endif));
	PrintAttribute(DelimToString(node.Semicolon));

	EndNode();
}

void AST::PrintVisitor::Visit(NAssignStmt& node)
{
	StartNode("<assign-statement>");

	SafeAccept(node.VarIdentifier);
	PrintAttribute(KeywordToString(node.Op));
	SafeAccept(node.Expr);
	PrintAttribute(DelimToString(node.Semicolon));

	EndNode();
}

void AST::PrintVisitor::Visit(NConditionStmt& node)
{
	StartNode("<condition-statement>");

	SafeAccept(node.IncompleteCondStmt);
	SafeAccept(node.AltPart);

	EndNode();
}

void AST::PrintVisitor::Visit(NIncompleteConditionStmt& node)
{
	StartNode("<incomplete-condition-statement>");

	PrintAttribute(KeywordToString(node.If));
	SafeAccept(node.CondExpr);
	PrintAttribute(KeywordToString(node.Then));
	SafeAccept(node.StmtsList);

	EndNode();
}

void AST::PrintVisitor::Visit(NAlternativePart& node)
{
	StartNode("<alternative-part>");

	if (node.Empty)
	{
		PrintEmpty();
		EndNode();
		return;
	}

	PrintAttribute(KeywordToString(node.Else));
	SafeAccept(node.StmtsList);

	EndNode();
}

void AST::PrintVisitor::Visit(NConditionalExpr& node)
{
	StartNode("<conditional-expression>");

	SafeAccept(node.Expr1);
	PrintAttribute(DelimToString(node.Op));
	SafeAccept(node.Expr2);

	EndNode();
}

void AST::PrintVisitor::Visit(NExpr& node)
{
	StartNode("<expression>");


	EndNode();
}

void AST::PrintVisitor::Visit(NVariableIdentifier& node)
{
	StartNode("<variable-identifier>");

	SafeAccept(node.Identifier);

	EndNode();
}

void AST::PrintVisitor::Visit(NProcedureIdentifier& node)
{
	StartNode("<procedure-identifier>");
	
	SafeAccept(node.Identifier);

	EndNode();
}

void AST::PrintVisitor::Visit(NIdentifier& node)
{
	StartNode("<identifier>");

	PrintAttribute(IdentifierToString(node.Identifier));

	EndNode();
}

void AST::PrintVisitor::Visit(NConstant& node)
{
	StartNode("<unsigned-integer>");

	PrintAttribute(ConstantToString(node.Val));

	EndNode();
}


void AST::PrintVisitor::AddOffset()
{
	m_Offset += m_Delim;
}

void AST::PrintVisitor::RemoveOffset()
{
	if (!m_Offset.empty()) m_Offset.pop_back();
}

void AST::PrintVisitor::PrintEmpty()
{
	PrintAttribute("<empty>");
}

void AST::PrintVisitor::PrintAttribute(const std::string& str)
{
	StartNode(str);
	RemoveOffset();
}

void AST::PrintVisitor::StartNode(const std::string& str)
{
	AddOffset();
	m_SS << m_Offset << "|" << str << "\n";
}

void AST::PrintVisitor::EndNode()
{
	RemoveOffset();
}


void AST::PrintVisitor::PrintNullptr()
{
	PrintAttribute(std::string(CRIMSON) + "<error-nullptr>" + RESET);
}

std::string AST::PrintVisitor::KeywordToString(ETokenCode key)
{
	if (key == ETokenCode::Empty)
		return std::string(LEMON) + "<error-symbol>" + RESET;
	return TEAL + std::to_string(+key) + RESET + " [" + TEAL + Reverse_KeyWordsTable[+key] + RESET + "]";
}

std::string AST::PrintVisitor::ConstantToString(uint32_t key)
{
	return AZURE + std::to_string(key) + RESET + " [" + AZURE + Reverse_ConstantsTable[key] + RESET + "]";
}

std::string AST::PrintVisitor::IdentifierToString(uint32_t key)
{
	return MAGENTA + std::to_string(key) + RESET + " [" + MAGENTA + Reverse_IdentifiersTable[+key] + RESET + "]";
}

std::string AST::PrintVisitor::DelimToString(ETokenCode key)
{
	if (key == ETokenCode::Empty)
		return std::string(LEMON) + "<error-symbol>" + RESET;
	return LIME + std::to_string(+key) + RESET+ " [" + LIME + char(+key) + RESET+"]";
}
