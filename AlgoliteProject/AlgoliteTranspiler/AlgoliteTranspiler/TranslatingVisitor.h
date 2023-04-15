#pragma once
#include "Nodes.h"

struct TranslatingVisitor : Visitor {
	virtual void handle(ArgumentNode& n);
	virtual void handle(FunctionDefinitionNode& n);
	virtual void handle(ParenExpressionNode& n);
	virtual void handle(BinaryOperationNode& n);
	virtual void handle(UnaryOperationNode& n);
	virtual void handle(IntegerLiteralNode& n);
	virtual void handle(DoubleLiteralNode& n);
	virtual void handle(BooleanLiteralNode& n);
	virtual void handle(StringLiteralNode& n);
	virtual void handle(VariableNode& n);
	virtual void handle(IfNode& n);
	virtual void handle(WhileNode& n);
	virtual void handle(ForNode& n);
	virtual void handle(DefaultNode& n);
	virtual void handle(CaseNode& n);
	virtual void handle(SwitchNode& n);
	virtual void handle(NonAnalysisBlockNode& n);
	virtual void handle(BlockNode& n);
	virtual void handle(FunctionCallNode& n);
	virtual void handle(FunctionCallAsStatementNode& n);
	virtual void handle(CoutNode& n);
	virtual void handle(IncDecNode& n);
	virtual void handle(BreakNode& n);
	virtual void handle(ContinueNode& n);
	virtual void handle(ReturnNode& n);
	virtual void handle(VariableDeclarationNode& n);
	virtual void handle(AssignmentNode& n);
	virtual void handle(Program& n);
};