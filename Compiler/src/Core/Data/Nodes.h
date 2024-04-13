#ifndef NODES_H_
#define NODES_H_

#include "ASTNode.h"

struct NSignalProgram : public ASTNode
{
	NSignalProgram(Ref<ASTNode> program) : Program(program) {}

	Ref<ASTNode> Program;
	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;

};
struct NProgram : public ASTNode
{
	NProgram(ETokenCode program, Ref<ASTNode> procId, ETokenCode sc, Ref<ASTNode> block, ETokenCode dot)
		: Program(program), ProcIdentifier(procId), Semicolon(sc), Block(block), Dot(dot) {}

	ETokenCode Program;
	Ref<ASTNode> ProcIdentifier;
	ETokenCode Semicolon;
	Ref<ASTNode> Block;
	ETokenCode Dot;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NBlock : public ASTNode
{
	NBlock(Ref<ASTNode> varDecl, ETokenCode begin, Ref<ASTNode> stmtList, ETokenCode end)
	: VarDecl(varDecl), Begin(begin), StmtsList(stmtList), End(end) {}

	Ref<ASTNode> VarDecl;
	ETokenCode Begin;
	Ref<ASTNode> StmtsList;
	ETokenCode End;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};

struct NVariableDeclarations : public ASTNode
{
	NVariableDeclarations(ETokenCode var, Ref<ASTNode> declList)
		: Var(var), DeclList(declList) {}

	ETokenCode Var;
	Ref<ASTNode> DeclList;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NDeclarationsList : public ASTNode
{
	NDeclarationsList(Ref<ASTNode> decl, Ref<ASTNode> declList)
		: Decl(decl), DeclList(declList) {}

	Ref<ASTNode> Decl;
	Ref<ASTNode> DeclList;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NDeclaration : public ASTNode
{
	NDeclaration(Ref<ASTNode> varId, ETokenCode colon, Ref<ASTNode> atr, ETokenCode sc)
		: VarIdentifier(varId), Colon(colon), Attribute(atr), Semicolon(sc) {}

	Ref<ASTNode> VarIdentifier;
	ETokenCode Colon;
	Ref<ASTNode> Attribute;
	ETokenCode Semicolon;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NAttribute : public ASTNode
{
	NAttribute(ETokenCode type) : Type(type) {}

	ETokenCode Type;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NStmt : public ASTNode
{

};
struct NStmtsList : public NStmt
{
	NStmtsList(Ref<ASTNode> stmt, Ref<ASTNode> stmtsList)
		: Stmt(stmt), StmtsList(stmtsList) {}

	Ref<ASTNode> Stmt;
	Ref<ASTNode> StmtsList;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NIfStmt : public NStmt
{
	NIfStmt(Ref<ASTNode> condStmt, ETokenCode eif, ETokenCode sc)
		: CondStmt(condStmt), Endif(eif), Semicolon(sc) {}

	Ref<ASTNode> CondStmt;
	ETokenCode Endif;
	ETokenCode Semicolon;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NAssignStmt : public NStmt
{
	NAssignStmt(Ref<ASTNode> varId, ETokenCode op, Ref<ASTNode> expr, ETokenCode sc)
		: VarIdentifier(varId), Op(op), Expr(expr), Semicolon(sc) {}

	Ref<ASTNode> VarIdentifier;
	ETokenCode Op;
	Ref<ASTNode> Expr;
	ETokenCode Semicolon;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};

struct NConditionStmt : public NStmt
{
	NConditionStmt(Ref<ASTNode> incCondStmt, Ref<ASTNode> alt)
		: IncompleteCondStmt(incCondStmt), AltPart(alt) {}

	Ref<ASTNode> IncompleteCondStmt;
	Ref<ASTNode> AltPart;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NIncompleteConditionStmt : public ASTNode
{
	NIncompleteConditionStmt(ETokenCode _if, Ref<ASTNode> condExpr, ETokenCode then, Ref<ASTNode> stmtsList)
		: If(_if), CondExpr(condExpr), Then(then), StmtsList(stmtsList) {}

	ETokenCode If;
	Ref<ASTNode> CondExpr;
	ETokenCode Then;
	Ref<ASTNode> StmtsList;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NAlternativePart : public ASTNode
{
	
	NAlternativePart(ETokenCode _else, Ref<ASTNode> stmtsList)
		: Else(_else), StmtsList(stmtsList) {}

	ETokenCode Else;
	Ref<ASTNode> StmtsList;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NConditionalExpr : public ASTNode
{
	NConditionalExpr(Ref<ASTNode> expr1, ETokenCode op, Ref<ASTNode> expr2)
		: Expr1(expr1), Op(op), Expr2(expr2) {}

	Ref<ASTNode> Expr1;
	ETokenCode Op;
	Ref<ASTNode> Expr2;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NExpr : public ASTNode
{

};
struct NVariableIdentifier : public NExpr
{
	NVariableIdentifier(Ref<ASTNode> id) : Identifier(id) {}

	Ref<ASTNode> Identifier;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};

struct NProcedureIdentifier : public ASTNode
{
	NProcedureIdentifier(Ref<ASTNode> id) : Identifier(id) {}

	Ref<ASTNode> Identifier;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};

struct NIdentifier : public ASTNode
{
	NIdentifier(ETokenCode id) : Identifier(id) {}

	ETokenCode Identifier;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};
struct NConstant : public NExpr
{
	NConstant(int64_t value) : Val(value) {}

	int64_t Val;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
};

#endif // NODES_H_
