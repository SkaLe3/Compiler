#pragma once

#include "ASTNode.h"

namespace AST
{
	class Visitor;
}

struct NSignalProgram : public ASTNode
{
	NSignalProgram(Ref<ASTNode> program) : Program(program) {}

	Ref<ASTNode> Program;
	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;

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
	virtual void Accept(AST::Visitor& visitor) override;
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
	virtual void Accept(AST::Visitor& visitor) override;
};

struct NVariableDeclarations : public ASTNode
{
	NVariableDeclarations(ETokenCode var, Ref<ASTNode> declList, bool empty)
		: Var(var), DeclList(declList), Empty(empty) {}

	ETokenCode Var;
	Ref<ASTNode> DeclList;

	bool Empty = false;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;
};
struct NDeclarationsList : public ASTNode
{
	NDeclarationsList(Ref<ASTNode> decl, Ref<ASTNode> declList, bool empty)
		: Decl(decl), DeclList(declList), Empty(empty) {}

	Ref<ASTNode> Decl;
	Ref<ASTNode> DeclList;

	bool Empty = false;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;
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
	virtual void Accept(AST::Visitor& visitor) override;
};
struct NAttribute : public ASTNode
{
	NAttribute(ETokenCode type) : Type(type) {}

	ETokenCode Type;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;
};
struct NStmt : public ASTNode
{

};
struct NStmtsList : public NStmt
{
	NStmtsList(Ref<ASTNode> stmt, Ref<ASTNode> stmtsList, bool empty)
		: Stmt(stmt), StmtsList(stmtsList), Empty(empty) {}

	Ref<ASTNode> Stmt;
	Ref<ASTNode> StmtsList;

	bool Empty = false;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;
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
	virtual void Accept(AST::Visitor& visitor) override;
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
	virtual void Accept(AST::Visitor& visitor) override;
};

struct NConditionStmt : public NStmt
{
	NConditionStmt(Ref<ASTNode> incCondStmt, Ref<ASTNode> alt)
		: IncompleteCondStmt(incCondStmt), AltPart(alt) {}

	Ref<ASTNode> IncompleteCondStmt;
	Ref<ASTNode> AltPart;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;
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
	virtual void Accept(AST::Visitor& visitor) override;
};
struct NAlternativePart : public ASTNode
{
	
	NAlternativePart(ETokenCode _else, Ref<ASTNode> stmtsList, bool empty)
		: Else(_else), StmtsList(stmtsList), Empty(empty) {}

	ETokenCode Else;
	Ref<ASTNode> StmtsList;

	bool Empty = false;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;
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
	virtual void Accept(AST::Visitor& visitor) override;
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
	virtual void Accept(AST::Visitor& visitor) override;
};

struct NProcedureIdentifier : public ASTNode
{
	NProcedureIdentifier(Ref<ASTNode> id) : Identifier(id) {}

	Ref<ASTNode> Identifier;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;
};

struct NIdentifier : public ASTNode
{
	NIdentifier(uint32_t id, const Token& tok) : token(tok), Identifier(id) {}

	uint32_t Identifier;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;

	Token token;
};
struct NConstant : public NExpr
{
	NConstant(uint32_t value, const Token& tok) : token(tok), Val(value) {}

	uint32_t Val;

	virtual std::vector<Ref<ASTNode>> GetData() override;
	virtual std::string ToString() override;
	virtual void Accept(AST::Visitor& visitor) override;

	Token token;
};
