#pragma once

#include "Data/Token.h"

#include <string>
#include <memory>
#include <list>
#include <vector>

template<typename T>
using Ref = std::shared_ptr<T>;

namespace AST
{
	class Visitor;
}

struct ASTNode 
{
public:
	virtual ~ASTNode() = default;
	virtual std::vector<Ref<ASTNode>> GetData() = 0;
	virtual std::string ToString() = 0;

	virtual void Accept(AST::Visitor& visitor) = 0;

};

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
	Ref<ASTNode> MakeSignalProgram(Ref<ASTNode> program);
	Ref<ASTNode> MakeProgram(ETokenCode program, Ref<ASTNode> procId, ETokenCode sc, Ref<ASTNode> block, ETokenCode dot);
	Ref<ASTNode> MakeBlock(Ref<ASTNode> varDecl, ETokenCode begin, Ref<ASTNode> stmtList, ETokenCode end);
	Ref<ASTNode> MakeVariableDeclarations(ETokenCode var, Ref<ASTNode> declList);
	Ref<ASTNode> MakeDeclarationsList(Ref<ASTNode> decl, Ref<ASTNode> declList);
	Ref<ASTNode> MakeDeclaration(Ref<ASTNode> varId, ETokenCode colon, Ref<ASTNode> atr, ETokenCode sc);
	Ref<ASTNode> MakeAttribute(ETokenCode type);
	Ref<ASTNode> MakeStmtsList(Ref<ASTNode> stmt, Ref<ASTNode> stmtsList);
	Ref<ASTNode> MakeIfStmts(Ref<ASTNode> condStmt, ETokenCode eif, ETokenCode sc);
	Ref<ASTNode> MakeAssignStmt(Ref<ASTNode> varId, ETokenCode op, Ref<NExpr> expr, ETokenCode sc);
	Ref<ASTNode> MakeConditionStmt(Ref<ASTNode> incCondStmt, Ref<ASTNode> alt);
	Ref<ASTNode> MakeIncompleteConditionStmt(ETokenCode _if, Ref<ASTNode> condExpr, ETokenCode then, Ref<ASTNode> stmtsList);
	Ref<ASTNode> MakeAlternativePart(ETokenCode _else, Ref<ASTNode> stmtsList);
	Ref<ASTNode> MakeConditionalExpr(Ref<NExpr> expr1, ETokenCode op, Ref<NExpr> expr2);
	Ref<ASTNode> MakeVariableIdentifier(Ref<ASTNode> id);
	Ref<ASTNode> MakeProcedureIdentifier(Ref<ASTNode> id);
	Ref<ASTNode> MakeIdentifier(uint32_t id);
	Ref<ASTNode> MakeConstant(int64_t value);

}