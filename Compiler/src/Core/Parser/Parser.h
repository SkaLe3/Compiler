#pragma once

#include "Data/Token.h"
#include "Data/ASTNode.h"

#include "Errors/Error.h"

#include <vector>
#include <memory>


class ErrorHandler;

class Parser
{
public:
	Parser(std::shared_ptr<std::vector<Token>>& tokenSequence, std::shared_ptr<ErrorHandler> errorHandler);

	void Parse();

	Ref<ASTNode> ParseTranslationUnit();
	Ref<ASTNode> ParseProgram();
	Ref<ASTNode> ParseBlock();
	Ref<ASTNode> ParseVariableDeclarations();
	Ref<ASTNode> ParseDeclarationsList();
	Ref<ASTNode> ParseDeclaration(bool& bIsDecl);
	Ref<ASTNode> ParseAttribute();
	Ref<ASTNode> ParseStatementsList();
	Ref<ASTNode> ParseStatement();
	Ref<ASTNode> ParseIfStatement();
	Ref<ASTNode> ParseAssignStatement();
	Ref<ASTNode> ParseConditionStatement();
	Ref<ASTNode> ParseIncompleteConditionStatement();
	Ref<ASTNode> ParseAlternativePart();
	Ref<ASTNode> ParseConditionalExpression();
	Ref<ASTNode> ParseExpression();
	Ref<ASTNode> ParseVariableIndetifier();
	Ref<ASTNode> ParseProcedureIdentifier();
	Ref<ASTNode> ParseIdentifier();
	Ref<ASTNode> ParseConstant();

	Ref<ASTNode> GetAST();

private:


	template <typename... TokenKind>
	bool Match(const ETokenCode& first, const TokenKind&... tokenKinds)
	{
		if (Match(first))
			return true;
		return Match(tokenKinds...);
	}

	bool Match(const ETokenCode& expected);
	bool Check(const ETokenCode& expected);
	std::vector<Token>::iterator Advance();
	bool IsAtEnd();
	std::vector<Token>::iterator Peek();
	std::vector<Token>::iterator Previous();
	std::vector<Token>::iterator Consume(ETokenCode kind, const std::string& message);
	void Synchronize();

private:
	Error CreateSyntaxError(const std::string& errorMessage, const Token& token);

private:
	std::shared_ptr<std::vector<Token>> m_TokenSequense;
	std::vector<Token>::iterator m_CurrentToken;
	Ref<ASTNode> m_AST;


	std::shared_ptr<ErrorHandler> m_ErrorHandler;

	EErrorInstigator m_Instigator;
};


