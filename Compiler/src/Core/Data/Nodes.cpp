#include "Nodes.h"



std::vector<Ref<ASTNode>> NSignalProgram::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NSignalProgram::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NProgram::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NProgram::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NBlock::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NBlock::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NVariableDeclarations::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NVariableDeclarations::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NDeclarationsList::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NDeclarationsList::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NDeclaration::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NDeclaration::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NAttribute::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NAttribute::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NStmtsList::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NStmtsList::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NIfStmt::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NIfStmt::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NAssignStmt::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NAssignStmt::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NConditionStmt::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NConditionStmt::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NIncompleteConditionStmt::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NIncompleteConditionStmt::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NAlternativePart::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NAlternativePart::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NConditionalExpr::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NConditionalExpr::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NVariableIdentifier::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NVariableIdentifier::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NProcedureIdentifier::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NProcedureIdentifier::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NIdentifier::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NIdentifier::ToString()
{
	return std::string();
}

std::vector<Ref<ASTNode>> NConstant::GetData()
{
	return std::vector<Ref<ASTNode>>();
}

std::string NConstant::ToString()
{
	return std::string();
}

namespace AST
{
	Ref<ASTNode> MakeSignalProgram(Ref<ASTNode> program)
	{
		return std::make_shared<NSignalProgram>(program);
	}
	Ref<ASTNode> MakeProgram(ETokenCode program, Ref<ASTNode> procId, ETokenCode sc, Ref<ASTNode> block, ETokenCode dot)
	{
		return std::make_shared<NProgram>(program, procId, sc, block, dot);
	}
	Ref<ASTNode> MakeBlock(Ref<ASTNode> varDecl, ETokenCode begin, Ref<ASTNode> stmtList, ETokenCode end)
	{
		return std::make_shared<NBlock>(varDecl, begin, stmtList, end);
	}
	Ref<ASTNode> MakeVariableDeclarations(ETokenCode var, Ref<ASTNode> declList)
	{
		return std::make_shared<NVariableDeclarations>(var, declList);
	}
	Ref<ASTNode> MakeDeclarationsList(Ref<ASTNode> decl, Ref<ASTNode> declList)
	{
		return std::make_shared<NDeclarationsList>(decl, declList);
	}
	Ref<ASTNode> MakeDeclaration(Ref<ASTNode> varId, ETokenCode colon, Ref<ASTNode> atr, ETokenCode sc)
	{
		return std::make_shared<NDeclaration>(varId, colon, atr, sc);
	}
	Ref<ASTNode> MakeAttribute(ETokenCode type)
	{
		return std::make_shared<NAttribute>(type);
	}
	Ref<ASTNode> MakeStmtsList(Ref<ASTNode> stmt, Ref<ASTNode> stmtsList)
	{
		return std::make_shared<NStmtsList>(stmt, stmtsList);
	}
	Ref<ASTNode> MakeIfStmts(Ref<ASTNode> condStmt, ETokenCode eif, ETokenCode sc)
	{
		return std::make_shared<NIfStmt>(condStmt, eif, sc);
	}
	Ref<ASTNode> MakeAssignStmt(Ref<ASTNode> varId, ETokenCode op, Ref<ASTNode> expr, ETokenCode sc)
	{
		return std::make_shared<NAssignStmt>(varId, op, expr, sc);
	}
	Ref<ASTNode> MakeConditionStmt(Ref<ASTNode> incCondStmt, Ref<ASTNode> alt)
	{
		return std::make_shared<NConditionStmt>(incCondStmt, alt);
	}
	Ref<ASTNode> MakeIncompleteConditionStmt(ETokenCode _if, Ref<ASTNode> condExpr, ETokenCode then, Ref<ASTNode> stmtsList)
	{
		return std::make_shared<NIncompleteConditionStmt>(_if, condExpr, then, stmtsList);
	}
	Ref<ASTNode> MakeAlternativePart(ETokenCode _else, Ref<ASTNode> stmtsList)
	{
		return std::make_shared<NAlternativePart>(_else, stmtsList);
	}
	Ref<ASTNode> MakeConditionalExpr(Ref<ASTNode> expr1, ETokenCode op, Ref<NExpr> expr2)
	{
		return std::make_shared<NConditionalExpr>(expr1, op, expr2);
	}
	Ref<ASTNode> MakeVariableIdentifier(Ref<ASTNode> id)
	{
		return std::make_shared<NVariableIdentifier>(id);
	}
	Ref<ASTNode> MakeProcedureIdentifier(Ref<ASTNode> id)
	{
		return std::make_shared<NProcedureIdentifier>(id);
	}
	Ref<ASTNode> MakeIdentifier(ETokenCode id)
	{
		return std::make_shared<NIdentifier>(id);
	}
	Ref<ASTNode> MakeConstant(int64_t value)
	{
		return std::make_shared<NConstant>(value);
	}

}