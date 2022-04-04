#pragma once
#include <string>
#include <vector>
using namespace std;

struct Node;
struct ExpressionNode;
struct StatementNode;
struct ArgumentNode;
struct FunctionDefinitionNode;
struct ParenExpressionNode;
struct BinaryOperationNode;
struct UnaryOperationNode;
struct IntegerLiteralNode;
struct DoubleLiteralNode;
struct BooleanLiteralNode;
struct StringLiteralNode;
struct VariableNode;
struct IfNode;
struct WhileNode;
struct ForNode;
struct CaseNode;
struct SwitchNode;
struct BlockNode;
struct FunctionCallNode;
struct FunctionCallAsStatementNode;
struct CoutNode;
struct IncDecNode;
struct BreakNode;
struct ContinueNode;
struct ReturnNode;
struct VariableDefinitionPart;
struct VariableDeclarationNode;
struct AssignmentNode;
struct Program;

struct Visitor {
	virtual void handle(ArgumentNode& n) = 0;
	virtual void handle(FunctionDefinitionNode& n) = 0;
	virtual void handle(ParenExpressionNode& n) = 0;
	virtual void handle(BinaryOperationNode& n) = 0;
	virtual void handle(UnaryOperationNode& n) = 0;
	virtual void handle(IntegerLiteralNode& n) = 0;
	virtual void handle(DoubleLiteralNode& n) = 0;
	virtual void handle(BooleanLiteralNode& n) = 0;
	virtual void handle(StringLiteralNode& n) = 0;
	virtual void handle(VariableNode& n) = 0;
	virtual void handle(IfNode& n) = 0;
	virtual void handle(WhileNode& n) = 0;
	virtual void handle(ForNode& n) = 0;
	virtual void handle(CaseNode& n) = 0;
	virtual void handle(SwitchNode& n) = 0;
	virtual void handle(BlockNode& n) = 0;
	virtual void handle(FunctionCallNode& n) = 0;
	virtual void handle(FunctionCallAsStatementNode& n) = 0;
	virtual void handle(CoutNode& n) = 0;
	virtual void handle(IncDecNode& n) = 0;
	virtual void handle(BreakNode& n) = 0;
	virtual void handle(ContinueNode& n) = 0;
	virtual void handle(ReturnNode& n) = 0;
	virtual void handle(VariableDeclarationNode& n) = 0;
	virtual void handle(AssignmentNode& n) = 0;
	virtual void handle(Program& n) = 0;
};

struct Node {
	virtual void visit(Visitor& v) = 0;
};

struct ExpressionNode : public Node {};
struct StatementNode : public Node {};

struct ArgumentNode : public Node {
	string type;
	string name;
	bool is_array;
	vector<unique_ptr<ExpressionNode>> arrays_sizes;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct FunctionDefinitionNode : public Node {
	string type;
	string name;
	bool hasDefinition;
	vector<unique_ptr<ArgumentNode>> arguments;
	vector<unique_ptr<StatementNode>> commands;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct ParenExpressionNode : public ExpressionNode {
	unique_ptr<ExpressionNode> child;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};


struct BinaryOperationNode : public ExpressionNode {
	vector<unique_ptr<ExpressionNode>> children;
	string operation;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct UnaryOperationNode : public ExpressionNode {
	unique_ptr<ExpressionNode> child;
	string operation;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct IntegerLiteralNode : public ExpressionNode {
	long long value;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct DoubleLiteralNode : public ExpressionNode {
	double value;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct BooleanLiteralNode : public ExpressionNode {
	bool value;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct StringLiteralNode : public ExpressionNode {
	string value;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct VariableNode : public ExpressionNode {
	string name;
	vector<unique_ptr<ExpressionNode>> index_expressions;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct IfNode : public StatementNode {
	unique_ptr<ExpressionNode> condition;
	unique_ptr<StatementNode> command;
	unique_ptr<StatementNode> else_command;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct WhileNode : public StatementNode {
	bool is_do_while;
	unique_ptr<ExpressionNode> condition;
	unique_ptr<StatementNode> command;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct ForNode : public StatementNode {
	unique_ptr<StatementNode> initialization;
	unique_ptr<ExpressionNode> condition;
	unique_ptr<StatementNode> step;
	unique_ptr<StatementNode> command;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct CaseNode : public StatementNode {
	unique_ptr<ExpressionNode> condition;
	vector<unique_ptr<StatementNode>> commands;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct SwitchNode : public StatementNode {
	unique_ptr<ExpressionNode> expression_for_switch;
	vector<unique_ptr<CaseNode>> cases;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct BlockNode : public StatementNode {
	vector<unique_ptr<StatementNode>> commands;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct FunctionCallNode : public ExpressionNode {
	string name;
	vector<unique_ptr<ExpressionNode>> argument_expressions;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct FunctionCallAsStatementNode : public StatementNode {
	unique_ptr<FunctionCallNode> function_call;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct CoutNode : public StatementNode {
	vector<unique_ptr<ExpressionNode>> printing_expressions;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct IncDecNode : public StatementNode {
	bool is_inc;
	unique_ptr<VariableNode> variable;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct BreakNode : public StatementNode {
	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct ContinueNode : public StatementNode {
	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct ReturnNode : public StatementNode {
	unique_ptr<ExpressionNode> returning_expression;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct VariableDefinitionPart {
	string name;
	unique_ptr<ExpressionNode> value;
	vector<unique_ptr<ExpressionNode>> arrays_sizes;
};

struct VariableDeclarationNode : public StatementNode {
	string variable_type;
	vector<VariableDefinitionPart> declarations;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct AssignmentNode : public StatementNode {
	unique_ptr<VariableNode> variable;
	string operation;
	unique_ptr<ExpressionNode> value;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};

struct Program : Node {
	vector<unique_ptr<FunctionDefinitionNode>> functions;
	string parameter_name;
	long long parameter_min;
	long long parameter_max;
	long long parameter_step;

	void visit(Visitor& v) override {
		v.handle(*this);
	}
};
