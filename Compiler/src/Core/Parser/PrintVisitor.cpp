#include "PrintVisitor.h"

#include "Data/SymbolTables.h"
#include "Data/Nodes.h"

std::string AST::PrintVisitor::Print(Ref<ASTNode> ast)
{
	Ref<NSignalProgram> program = static_pointer_cast<NSignalProgram>(ast);
	if (program)
		SafeAccept(program);
	else
	{
		m_SS << " ";
		// TODO : Make error
	}
	return m_SS.str();
}

void AST::PrintVisitor::Visit(NSignalProgram& node)
{
	m_SS << "<signal-program>\n";

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

	PrintAttribute(KeywordToString(node.Var));
	SafeAccept(node.DeclList);

	EndNode();
}

void AST::PrintVisitor::Visit(NDeclarationsList& node)
{
	StartNode("<declarations-list>");

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

void AST::PrintVisitor::PrintAttribute(const std::string& str)
{
	AddOffset();
	m_SS << m_Offset << str << "\n";
	RemoveOffset();
}

void AST::PrintVisitor::StartNode(const std::string& str)
{
	AddOffset();
	m_SS << m_Offset << str << "\n";
}

void AST::PrintVisitor::EndNode()
{
	RemoveOffset();
}

void AST::PrintVisitor::PrintNullptr()
{
	AddOffset();
	m_SS << m_Offset << "<empty>\n";
	RemoveOffset();
}

std::string AST::PrintVisitor::KeywordToString(ETokenCode key)
{
	return +key + " " + Reverse_KeyWordsTable[+key]; 
}

std::string AST::PrintVisitor::ConstantToString(int64_t key)
{
	return key + " " + Reverse_ConstantsTable[key]; 
}

std::string AST::PrintVisitor::IdentifierToString(uint32_t key)
{
	return +key + " " + Reverse_IdentifiersTable[+key];
}

std::string AST::PrintVisitor::DelimToString(ETokenCode key)
{
	if (key == ETokenCode::None)
		return "<Error Symbol>";
	return +key + " " + char(+key);
}
