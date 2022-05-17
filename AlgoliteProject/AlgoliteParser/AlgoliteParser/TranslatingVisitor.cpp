#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Nodes.h"
#include "TranslatingVisitor.h"

#define ERROR_COLORIZED_COUT "\033[1m\033[31m"
#define STOP_COLORIZED_COUT "\033[0m"

bool in_for_translating = false;
bool writing_mode = true;
bool analysis_mode = true;
stringstream analysing_program;

int _expression_counter;

void TranslatingVisitor::handle(ArgumentNode& n) {
	if (writing_mode) {
		analysing_program << n.type << " " << n.name;

		if (n.is_array) {
			analysing_program << "[]";
		}
	}

	for (const unique_ptr<ExpressionNode>& expression : n.arrays_sizes) {
		if (writing_mode)
			analysing_program << "[ ";
		expression->visit(*this);
		if (writing_mode)
			analysing_program << " ]";
	}
}

void TranslatingVisitor::handle(FunctionDefinitionNode& n) {
	if (writing_mode) {
		analysing_program << n.type << " _" << n.name;

		analysing_program << "(";
	}

	int arguments_size = n.arguments.size();

	if (n.name == "main" && arguments_size != 0) {
		cout << ERROR_COLORIZED_COUT << "Функция main не должна иметь аргументов" << STOP_COLORIZED_COUT << "\n";
		exit(1);
	}

	for (int i = 0; i < arguments_size; i++) {
		n.arguments[i]->visit(*this);
		if (i < arguments_size - 1 && writing_mode)
			analysing_program << ", ";
	}
	if (writing_mode)
		analysing_program << ")";

	if (n.hasDefinition) {
		if (writing_mode)
			analysing_program << " {" << "\n";
		for (const unique_ptr<StatementNode>& command : n.commands) {
			if (writing_mode)
				analysing_program << "    ";
			command->visit(*this);
			if (writing_mode)
				analysing_program << "\n";
		}
		if (writing_mode)
			analysing_program << "}";
	}
	else if (writing_mode)
		analysing_program << ";" << "\n";
}

void TranslatingVisitor::handle(ParenExpressionNode& n) {
	if (writing_mode)
		analysing_program << "(";
	n.child->visit(*this);
	if (writing_mode)
		analysing_program << ")";
}

void TranslatingVisitor::handle(BinaryOperationNode& n) {
	int children_size = n.children.size();

	_expression_counter += children_size - 1;

	for (int i = 0; i < children_size; i++) {
		n.children[i]->visit(*this);
		if (i < children_size - 1 && writing_mode)
			analysing_program << " " << n.operation << " ";
	}
}

void TranslatingVisitor::handle(UnaryOperationNode& n) {
	if (writing_mode)
		analysing_program << " " << n.operation << " ";
	_expression_counter++;
	n.child->visit(*this);
}


void TranslatingVisitor::handle(IntegerLiteralNode& n) {
	if (writing_mode)
		analysing_program << n.value;
}

void TranslatingVisitor::handle(DoubleLiteralNode& n) {
	if (writing_mode)
		analysing_program << n.value;
}

void TranslatingVisitor::handle(BooleanLiteralNode& n) {
	if (writing_mode) {
		if (n.value)
			analysing_program << "true";
		else
			analysing_program << "false";
	}
}

void TranslatingVisitor::handle(StringLiteralNode& n) {
	if (writing_mode)
		analysing_program << n.value;
}

void TranslatingVisitor::handle(VariableNode& n) {
	if (writing_mode)
		analysing_program << n.name;
	for (const unique_ptr<ExpressionNode>& expression : n.index_expressions) {
		if (writing_mode)
			analysing_program << "[ ";
		expression->visit(*this);
		if (writing_mode)
			analysing_program << " ]";
	}
}

void TranslatingVisitor::handle(IfNode& n) {
	_expression_counter = 0;
	writing_mode = false;

	n.condition->visit(*this);

	writing_mode = true;

	analysing_program << "if " << "(";
	if (analysis_mode)
		analysing_program << "_counter += " << _expression_counter << ", ";
	n.condition->visit(*this);
	analysing_program << ") ";

	if (dynamic_cast<BlockNode*>(n.command.get()) != nullptr) {
		n.command->visit(*this);
	}
	else {
		analysing_program << "{";
		n.command->visit(*this);
		analysing_program << "    }";
	}

	if (n.else_command != nullptr) {
		analysing_program << "\n    " << "else ";

		if (dynamic_cast<BlockNode*>(n.command.get()) != nullptr) {
			n.else_command->visit(*this);
		}
		else {
			analysing_program << "{";
			n.else_command->visit(*this);
			analysing_program << "    }";
		}
	}
}

void TranslatingVisitor::handle(WhileNode& n) {
	int condition_counter;
	_expression_counter = 0;
	writing_mode = false;

	n.condition->visit(*this);

	writing_mode = true;

	condition_counter = _expression_counter;

	if (n.is_do_while) {
		analysing_program << "do ";

		if (dynamic_cast<BlockNode*>(n.command.get()) != nullptr) {
			n.command->visit(*this);
		}
		else {
			analysing_program << "{";
			n.command->visit(*this);
			analysing_program << "    }";
		}

		analysing_program << " while" << "(";
		if (analysis_mode)
			analysing_program << "_counter += " << condition_counter << ", ";
		n.condition->visit(*this);
		analysing_program << ");";
	}

	else {
		analysing_program << "while" << "(";
		if (analysis_mode)
			analysing_program << "_counter += " << condition_counter << ", ";
		n.condition->visit(*this);
		analysing_program << ") ";

		if (dynamic_cast<BlockNode*>(n.command.get()) != nullptr) {
			n.command->visit(*this);
		}
		else {
			analysing_program << "{";
			n.command->visit(*this);
			analysing_program << "    }";
		}
	}
}

void TranslatingVisitor::handle(ForNode& n) {
	if (dynamic_cast<VariableDeclarationNode*>(n.initialization.get()) != nullptr) {
		_expression_counter = 0;
		writing_mode = false;

		n.initialization->visit(*this);

		writing_mode = true;

		if (analysis_mode)
			analysing_program << "_counter += " << _expression_counter << ";\n";
	}

	analysing_program << "for " << "(";
	in_for_translating = true;

	if (n.initialization != nullptr)
		n.initialization->visit(*this);
	if (writing_mode)
		analysing_program << "; ";

	if (n.condition != nullptr) {
		_expression_counter = 0;
		writing_mode = false;

		n.condition->visit(*this);

		writing_mode = true;

		if (analysis_mode)
			analysing_program << "_counter += " << _expression_counter << ", ";
		n.condition->visit(*this);

	}
	analysing_program << "; ";

	if (n.step != nullptr)
		n.step->visit(*this);

	analysing_program << ") ";
	in_for_translating = false;


	if (dynamic_cast<BlockNode*>(n.command.get()) != nullptr) {
		n.command->visit(*this);
	}
	else {
		analysing_program << "{";
		n.command->visit(*this);
		analysing_program << "    }";
	}
}

void TranslatingVisitor::handle(DefaultNode& n) {
	if (writing_mode)
		analysing_program << "default: " << "\n";

	for (const unique_ptr<StatementNode>& command : n.commands) {
		if (writing_mode)
			analysing_program << "    ";
		command->visit(*this);
		if (writing_mode)
			analysing_program << "\n";
	}
}

void TranslatingVisitor::handle(CaseNode& n) {
	if (writing_mode)
		analysing_program << "case ";
	n.condition->visit(*this);
	if (writing_mode)
		analysing_program << ":" << "\n";

	for (const unique_ptr<StatementNode>& command : n.commands) {
		if (writing_mode)
			analysing_program << "    ";
		command->visit(*this);
		if (writing_mode)
			analysing_program << "\n";
	}
}

void TranslatingVisitor::handle(SwitchNode& n) {
	_expression_counter = 0;
	writing_mode = false;

	n.expression_for_switch->visit(*this);

	writing_mode = true;

	if (analysis_mode)
		analysing_program << "_counter += " << _expression_counter << ";\n";

	analysing_program << "switch" << "(";
	n.expression_for_switch->visit(*this);
	analysing_program << ") ";

	analysing_program << "{ " << "\n";

	for (const unique_ptr<CaseNode>& case_command : n.cases) {
		analysing_program << "    ";
		case_command->visit(*this);
	}

	if (n.default_case != nullptr) {
		analysing_program << "    ";
		n.default_case->visit(*this);
	}

	analysing_program << " }";
}

void TranslatingVisitor::handle(NonAnalysisBlockNode& n) {
	analysis_mode = false;

	for (const unique_ptr<StatementNode>& command : n.commands) {
		if (writing_mode)
			analysing_program << "    ";
		command->visit(*this);
		if (writing_mode)
			analysing_program << "\n";
	}

	analysis_mode = true;
}

void TranslatingVisitor::handle(BlockNode& n) {
	if (writing_mode)
		analysing_program << "{" << "\n";

	for (const unique_ptr<StatementNode>& command : n.commands) {
		if (writing_mode)
			analysing_program << "    ";
		command->visit(*this);
		if (writing_mode)
			analysing_program << "\n";
	}

	if (writing_mode)
		analysing_program << "    }";
}

void TranslatingVisitor::handle(FunctionCallNode& n) {
	if (writing_mode)
		analysing_program << "_" << n.name << "(";

	int argument_expression_size = n.argument_expressions.size();

	for (int i = 0; i < argument_expression_size; i++) {
		n.argument_expressions[i]->visit(*this);
		if (i < argument_expression_size - 1 && writing_mode)
			analysing_program << ", ";
	}

	if (writing_mode)
		analysing_program << ")";
}

void TranslatingVisitor::handle(FunctionCallAsStatementNode& n) {
	_expression_counter = 0;

	n.function_call->visit(*this);

	if (writing_mode && analysis_mode)
		analysing_program << ", _counter += " << _expression_counter;

	if (!in_for_translating && writing_mode)
		analysing_program << ";";
}

void TranslatingVisitor::handle(CoutNode& n) {
	_expression_counter = 0;

	if (writing_mode)
		analysing_program << "cout";
	for (const unique_ptr<ExpressionNode>& expression : n.printing_expressions) {
		if (writing_mode)
			analysing_program << " << ";
		expression->visit(*this);
	}

	if (writing_mode && analysis_mode)
		analysing_program << ", _counter += " << _expression_counter;

	if (!in_for_translating && writing_mode)
		analysing_program << ";";
}

void TranslatingVisitor::handle(IncDecNode& n) {
	_expression_counter = 0;

	n.variable->visit(*this);

	if (writing_mode) {
		if (n.is_inc)
			analysing_program << "++";
		else
			analysing_program << "--";
	}

	_expression_counter++;

	if (writing_mode && analysis_mode)
		analysing_program << ", _counter += " << _expression_counter;

	if (!in_for_translating && writing_mode)
		analysing_program << ";";
}

void TranslatingVisitor::handle(BreakNode& n) {
	if (writing_mode)
		analysing_program << "break";
	if (!in_for_translating && writing_mode)
		analysing_program << ";";
}

void TranslatingVisitor::handle(ContinueNode& n) {
	if (writing_mode)
		analysing_program << "continue";
	if (!in_for_translating && writing_mode)
		analysing_program << ";";
}

void TranslatingVisitor::handle(ReturnNode& n) {
	_expression_counter = 0;
	writing_mode = false;

	n.returning_expression->visit(*this);

	writing_mode = true;

	if (!in_for_translating && writing_mode && analysis_mode) {
		analysing_program << "_counter += " << _expression_counter << ";\n";
	}

	if (writing_mode)
		analysing_program << "return ";
	n.returning_expression->visit(*this);
	if (!in_for_translating && writing_mode)
		analysing_program << ";";
}

void TranslatingVisitor::handle(VariableDeclarationNode& n) {
	_expression_counter = 0;

	if (writing_mode)
		analysing_program << n.variable_type << " ";

	int declarations_size = n.declarations.size();

	for (int i = 0; i < declarations_size; i++) {
		if (writing_mode)
			analysing_program << n.declarations[i].name;

		for (const unique_ptr<ExpressionNode>& expression : n.declarations[i].arrays_sizes) {
			if (writing_mode)
				analysing_program << "[ ";
			expression->visit(*this);
			if (writing_mode)
				analysing_program << " ]";
		}

		if (n.declarations[i].value != nullptr) {
			_expression_counter++;
			if (writing_mode)
				analysing_program << " = ";
			n.declarations[i].value->visit(*this);
		}

		if (i < declarations_size - 1 && writing_mode)
			analysing_program << ", ";
	}

	if (!in_for_translating && writing_mode) {
		analysing_program << ";\n";
		if (analysis_mode)
			analysing_program << "    _counter += " << _expression_counter << ";";
	}

}

void TranslatingVisitor::handle(AssignmentNode& n) {
	_expression_counter = 0;

	n.variable->visit(*this);

	if (writing_mode)
		analysing_program << " " << n.operation << " ";

	n.value->visit(*this);

	if (n.operation == "=")
		_expression_counter++;
	else
		_expression_counter += 2;

	if (writing_mode && analysis_mode)
		analysing_program << ", _counter += " << _expression_counter;

	if (!in_for_translating && writing_mode)
		analysing_program << ";";
}

void TranslatingVisitor::handle(Program& n) {
	ifstream initialization_file("ProgramPartFiles/ProgramInitialization.txt");

	if (initialization_file && writing_mode) {
		analysing_program << initialization_file.rdbuf();
		initialization_file.close();
	}

	if (writing_mode)
		analysing_program << "__thread long long _counter;" << "\n";
		analysing_program << "__thread long long " << n.parameter_name << ";" << "\n";
		analysing_program << "__thread int global_thread_number;" << "\n";
		analysing_program << "vector<pair<long long, double>> func_vec(" << (n.parameter_max - n.parameter_min + n.parameter_step - 1) / n.parameter_step << ");\n\n";

	for (const unique_ptr<FunctionDefinitionNode>& func_definition : n.functions) {
		func_definition->visit(*this);
		if (writing_mode)
			analysing_program << "\n\n";
	}

	if (writing_mode) {
		analysing_program << "void fillComplexityFuncInThread(int thread_number) {" << "\n";
		analysing_program << "global_thread_number = thread_number;" << "\n";
		analysing_program << "int index = 0;" << "\n";
		analysing_program << "for (" << n.parameter_name << " = " << n.parameter_min << " + " << n.parameter_step << " * "  << "global_thread_number" << "; " << n.parameter_name << " < " << n.parameter_max << "; " << n.parameter_name << " += " << n.parameter_step << " * 4) {" << "\n";
		analysing_program << "_counter = 0; " << "\n";
		analysing_program << "_main(); " << "\n";
		analysing_program << "func_vec[global_thread_number + index] = make_pair(" << n.parameter_name << ", _counter); " << "\n";
		analysing_program << "index += 4;" << "\n";
		analysing_program << "}" << "\n";
		analysing_program << "}" << "\n\n";
	}

	if (writing_mode) {
		analysing_program << "void createComplexityFunc() {" << "\n";
		analysing_program << "thread t0(fillComplexityFuncInThread, 0);" << "\n";
		analysing_program << "thread t1(fillComplexityFuncInThread, 1);" << "\n";
		analysing_program << "thread t2(fillComplexityFuncInThread, 2);" << "\n";
		analysing_program << "thread t3(fillComplexityFuncInThread, 3);" << "\n";
		analysing_program << "t0.join();" << "\n";
		analysing_program << "t1.join();" << "\n";
		analysing_program << "t2.join();" << "\n";
		analysing_program << "t3.join();" << "\n";
		analysing_program << "}" << "\n\n";
	}

	ifstream complexity_class_file("ProgramPartFiles/ComplexityClassSearching.txt");
	if (complexity_class_file) {
		if (writing_mode)
			analysing_program << complexity_class_file.rdbuf();
		complexity_class_file.close();
	}

	ifstream main_func_file("ProgramPartFiles/MainFunction.txt");
	if (main_func_file) {
		if (writing_mode)
			analysing_program << main_func_file.rdbuf();
		main_func_file.close();
	}

	ofstream f("finalOutput.cpp", ios::out);
	f << analysing_program.str();
	f.close();
}
