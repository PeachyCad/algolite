﻿#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Nodes.h"
#include "TranslatingVisitor.h"


#define SUCCESS_COLORIZED_COUT "\033[1m\033[32m"
#define ERROR_COLORIZED_COUT "\033[1m\033[31m"
#define STOP_COLORIZED_COUT "\033[0m"

#define MAGIC_ENUM_RANGE_MAX 300
#include "magic_enum.hpp"

using namespace std;
string text;
string current_id;
string string_literal;
int idx, line, pos;
int prev_symbol_index, prev_symbol_line, prev_symbol_pos;
bool is_real_number;
bool has_main = false;
bool in_non_analysis_block = false;
double number_value;

string readSource(string file_name) {
	ifstream f(file_name);
	f.seekg(0, ios::end);
	size_t size = f.tellg();
	string s(size, ' ');
	f.seekg(0);
	f.read(&s[0], size);
	return s;
}

enum TokenType {
	LParen = '(',
	RParen = ')',
	LBrace = '{',
	RBrace = '}',
	LBracket = '[',
	RBracket = ']',
	Comma = ',',
	Plus = '+',
	Minus = '-',
	Div = '/',
	Mod = '%',
	Mult = '*',
	Equals = '=',
	Less = '<',
	Greater = '>',
	Colon = ':',
	Pragma = '#',
	Semicolon = ';',
	NewLine = '\n',
	EndOfFile = '\0',
	If = 256,
	Else,
	For,
	While,
	Do,
	Switch,
	Default,
	Case,
	AnalysisDisable,
	Cout,
	LeftLeft,
	RightRight,
	And,
	Or,
	LessEq,
	GreaterEq,
	PlusEq,
	MinusEq,
	MultEq,
	DivEq,
	ModEq,
	NotEq,
	EqualsEq,
	Inc,
	Dec,
	Break,
	Continue,
	Return,
	Void,
	Int,
	Long,
	Double,
	Bool,
	Ident,
	Number,
	LogicalValue,
	StringLiteral,
	Parameter
};

TokenType symbol;
string id();

bool digit();
bool letter();
void nextSymbol();
void skipSpaces();
void readNumber();

unique_ptr<ExpressionNode> atom();
unique_ptr<FunctionCallNode> functionCall();
unique_ptr<StatementNode> command();

void error(string text_error) {
	cout << ERROR_COLORIZED_COUT << "Line " << prev_symbol_line << ", symbol " << prev_symbol_pos << ": ";
	cout << text_error << STOP_COLORIZED_COUT << "\n";
	cout << "Was captured: " << magic_enum::enum_name(symbol) << "\n";
	exit(1);
}

void savePrevSymbolData() {
	prev_symbol_index = idx;
	prev_symbol_pos = pos;
	prev_symbol_line = line;
}

void backupSymbolData(int backup_index, int backup_pos, int backup_line) {
	idx = backup_index;
	pos = backup_pos;
	line = backup_line;
	nextSymbol();
}

void nextSymbol() {
	is_real_number = false;
	skipSpaces();
	savePrevSymbolData();

	if (idx == text.length()) {
		symbol = EndOfFile;
		return;
	}
	switch (text[idx]) {
	case '(': case ')':
	case '{': case '}':
	case '[': case ']':
	case ',': case ';':
	case ':': case '#':
		symbol = (TokenType)text[idx];
		idx++;
		pos++;
		break;

	case '\n':
		line++;
		symbol = NewLine;
		idx++;
		pos = 1;
		break;

	case '&':
		if (text[idx + 1] == '&') {
			symbol = And;
			idx += 2;
			pos++;
			break;
		}
		else error("Unknown symbol");

	case '|':
		if (text[idx + 1] == '|') {
			symbol = Or;
			idx += 2;
			pos++;
			break;
		}
		else error("Unknown symbol");

	case '<': case '>':
	case '-': case '+':
	case '!': case '=':
	case '/': case '%':
	case '*':
		if (text[idx + 1] != '=') {
			if (text[idx] == '+' && text[idx + 1] == '+') {
				symbol = Inc;
				idx += 2;
				pos += 2;
				break;
			}
			else if (text[idx] == '-' && text[idx + 1] == '-') {
				symbol = Dec;
				idx += 2;
				pos += 2;
				break;
			}
			else if (text[idx] == '<' && text[idx + 1] == '<') {
				symbol = LeftLeft;
				idx += 2;
				pos += 2;
				break;
			}
			else if (text[idx] == '>' && text[idx + 1] == '>') {
				symbol = RightRight;
				idx += 2;
				pos += 2;
				break;
			}
			else {
				symbol = (TokenType)text[idx];
				idx++;
				pos++;
			}
		}
		else {
			switch (text[idx]) {
			case '<': symbol = LessEq; break;
			case '+': symbol = PlusEq; break;
			case '-': symbol = MinusEq; break;
			case '*': symbol = MultEq; break;
			case '/': symbol = DivEq; break;
			case '%': symbol = ModEq; break;
			case '>': symbol = GreaterEq; break;
			case '!': symbol = NotEq; break;
			case '=': symbol = EqualsEq; break;
			default: break;
			}
			idx += 2;
			pos += 2;
		}
		break;

	case '"':
		symbol = StringLiteral;
		string_literal = "\"";
		idx++;
		pos++;
		while (text[idx] != '"') {
			string_literal += text[idx];
			if (idx == text.length())
				error("Unknown symbol");
			else {
				idx++;
				pos++;
			}
		}
		string_literal += "\"";
		idx++;
		pos++;
		break;

	default:

		if (digit()) {
			readNumber();
		}

		else if (letter()) {
			current_id = id();
			if (current_id == "ANALYSIS_DISABLE") symbol = AnalysisDisable;
			else if (current_id == "for") symbol = For;
			else if (current_id == "while") symbol = While;
			else if (current_id == "do") symbol = Do;
			else if (current_id == "switch") symbol = Switch;
			else if (current_id == "default") symbol = Default;
			else if (current_id == "case") symbol = Case;
			else if (current_id == "if") symbol = If;
			else if (current_id == "else") symbol = Else;
			else if (current_id == "break") symbol = Break;
			else if (current_id == "continue") symbol = Continue;
			else if (current_id == "return") symbol = Return;
			else if (current_id == "void") symbol = Void;
			else if (current_id == "int") symbol = Int;
			else if (current_id == "long") symbol = Long;
			else if (current_id == "double") symbol = Double;
			else if (current_id == "bool") symbol = Bool;
			else if (current_id == "true" || current_id == "false") symbol = LogicalValue;
			else if (current_id == "cout") symbol = Cout;
			else if (current_id == "parameter") symbol = Parameter;
			else {
				symbol = Ident;
			}
		}
		else error("Unknown symbol");
	}
}

void initText() {
	text = readSource("SourceAlgolite.txt");
	idx = 0;
	line = 1;
	pos = 1;
	nextSymbol();
}

void accept(TokenType expected) {
	if (symbol != expected) {
		string str = string(1, (char)expected);
		switch (expected) {
			case NewLine: str = "new line"; break;
			case EndOfFile: str = "end of file"; break;
			case Parameter: str = "parameter"; break;
			case If: str = "if"; break;
			case Else: str = "else"; break;
			case For: str = "for"; break;
			case While: str = "while"; break;
			case Do: str = "do"; break;
			case Pragma: str = "#"; break;
			case AnalysisDisable: str = "ANALYSIS_DISABLE"; break;
			case Switch: str = "switch"; break;
			case Case: str = "case"; break;
			case Cout: str = "cout"; break;
			case LeftLeft: str = "<<"; break;
			case RightRight: str = ">>"; break;
			case And: str = "&&"; break;
			case Or: str = "||"; break;
			case LessEq: str = "<="; break;
			case GreaterEq: str = ">="; break;
			case PlusEq: str = "+="; break;
			case MinusEq: str = "-="; break;
			case MultEq: str = "*="; break;
			case DivEq: str = "/="; break;
			case ModEq: str = "%="; break;
			case NotEq: str = "!="; break;
			case EqualsEq: str = "=="; break;
			case Inc: str = "++"; break;
			case Dec: str = "--"; break;
			case Break: str = "break"; break;
			case Continue: str = "continue"; break;
			case Return: str = "return"; break;
			case Void: str = "void"; break;
			case Int: str = "int"; break;
			case Long: str = "long "; break;
			case Double: str = "double"; break;
			case Bool: str = "bool"; break;
			case Ident: str = "identifier"; break;
			case Number: str = "number"; break;
		}
		error("Was expected: " + str);
	}
	nextSymbol();
}

void skipSpaces() {
	while (text[idx] == ' ' || text[idx] == '\t' || text[idx] == '\r' || text[idx] == '\n') {
		if (text[idx] == '\n') {
			line++;
			pos = 1;
		}
		else
			pos++;
		idx++;
	}
}

bool digit() {
	if (text[idx] >= '0' && text[idx] <= '9') return true;
	return false;
}

void readNumber() {
	symbol = Number;
	number_value = 0;

	while (digit()) {
		number_value = number_value * 10 + text[idx] - '0';
		idx++;
		pos++;
	}
	if (text[idx] == '.') {
		is_real_number = true;
		idx++;
		pos++;
		if (!digit()) {
			idx--;
			pos--;
			return;
		}

		double weight = 0.1;
		while (digit()) {
			number_value += weight * (text[idx] - '0');
			weight /= 10;
			idx++;
			pos++;
		}
	}
}

bool letter() {
	if (text[idx] == '_') return true;
	if (text[idx] >= 'a' && text[idx] <= 'z') return true;
	if (text[idx] >= 'A' && text[idx] <= 'Z') return true;
	return false;
}

string id() {
	int startIndex = idx;
	if (!letter()) error("Isn't a letter");
	idx++;
	pos++;

	while (letter() || digit()) {
		idx++;
		pos++;
	}
	return text.substr(startIndex, idx - startIndex);
}

double acceptNumber() {
	double result = 1;
	if (symbol == Minus) {
		nextSymbol();
		result = -1;
	}
	result *= number_value;
	accept(Number);
	return result;
}

string dataType() {
	string result_type = current_id;
	switch (symbol) {
		case Int:
		case Bool:
		case Double:
			nextSymbol();
			break;
		case Long:
			nextSymbol();
			if (symbol == Long) {
				result_type += " long";
				nextSymbol();
			}
			break;
		default:
			error("Data type was expected");
	}
	return result_type;
}

unique_ptr<ExpressionNode> factor() {
	if (symbol == Minus) {
		nextSymbol();
		unique_ptr<UnaryOperationNode> result_factor = make_unique<UnaryOperationNode>();
		result_factor->child = atom();
		result_factor->operation = '-';
		return result_factor;
	}
	else
		return atom();
}

unique_ptr<ExpressionNode> term() {
	unique_ptr<ExpressionNode> temp_factor = factor();

	if (symbol != Mult && symbol != Div && symbol != Mod)
		return temp_factor;

	else {
		unique_ptr<BinaryOperationNode> result_term = make_unique<BinaryOperationNode>();
		result_term->children.push_back(move(temp_factor));

		switch (symbol) {
		case Mult:
			result_term->operation = "*";
			break;
		case Div:
			result_term->operation = "/";
			break;
		case Mod:
			result_term->operation = "%";
			break;
		}

		while (true) {
			if (symbol != Mult && symbol != Div && symbol != Mod)
				break;

			string current_operation;

			switch (symbol) {
			case Mult:
				current_operation = "*";
				break;
			case Div:
				current_operation = "/";
				break;
			case Mod:
				current_operation = "%";
				break;
			}

			if (current_operation != result_term->operation) {
				unique_ptr<BinaryOperationNode> new_term = make_unique<BinaryOperationNode>();
				new_term->operation = current_operation;
				new_term->children.push_back(move(result_term));
				result_term = move(new_term);
			}

			nextSymbol();
			result_term->children.push_back(factor());
		}
		return result_term;
	}
}

unique_ptr<ExpressionNode> sum() {
	unique_ptr<ExpressionNode> temp_term = term();

	if (symbol != Plus && symbol != Minus)
		return temp_term;

	else {
		unique_ptr<BinaryOperationNode> result_sum = make_unique<BinaryOperationNode>();
		result_sum->children.push_back(move(temp_term));

		switch (symbol) {
		case Plus:
			result_sum->operation = "+";
			break;
		case Minus:
			result_sum->operation = "-";
			break;
		}

		while (true) {
			if (symbol != Plus && symbol != Minus)
				break;

			string current_operation;

			switch (symbol) {
			case Plus:
				current_operation = "+";
				break;
			case Minus:
				current_operation = "-";
				break;
			}

			if (current_operation != result_sum->operation) {
				unique_ptr<BinaryOperationNode> new_sum = make_unique<BinaryOperationNode>();
				new_sum->operation = current_operation;
				new_sum->children.push_back(move(result_sum));
				result_sum = move(new_sum);
			}

			nextSymbol();
			result_sum->children.push_back(term());
		}
		return result_sum;
	}
}

unique_ptr<ExpressionNode> andExpression() {
	unique_ptr<ExpressionNode> temp_sum = sum();

	if (symbol == Less || symbol == Greater || symbol == LessEq || symbol == GreaterEq || symbol == NotEq || symbol == EqualsEq) {

		string current_operation;

		switch (symbol) {
		case Less:
			current_operation = "<";
			break;
		case Greater:
			current_operation = ">";
			break;
		case LessEq:
			current_operation = "<=";
			break;
		case GreaterEq:
			current_operation = ">=";
			break;
		case NotEq:
			current_operation = "!=";
			break;
		case EqualsEq:
			current_operation = "==";
			break;
		}

		unique_ptr<BinaryOperationNode> result_operation = make_unique<BinaryOperationNode>();
		result_operation->children.push_back(move(temp_sum));
		result_operation->operation = current_operation;
		nextSymbol();
		result_operation->children.push_back(sum());
		return result_operation;
	}
	else
		return temp_sum;
}

unique_ptr<ExpressionNode> orExpression() {
	unique_ptr<ExpressionNode> temp_expression = andExpression();

	if (symbol != And)
		return temp_expression;

	else {
		unique_ptr<BinaryOperationNode> result_operation = make_unique<BinaryOperationNode>();
		result_operation->children.push_back(move(temp_expression));
		result_operation->operation = "&&";

		while (true) {
			if (symbol != And)
				break;
			nextSymbol();
			result_operation->children.push_back(andExpression());
		}
		return result_operation;
	}
}

unique_ptr<ExpressionNode> expression() {
	unique_ptr<ExpressionNode> temp_expression = orExpression();

	if (symbol != Or)
		return temp_expression;

	else {
		unique_ptr<BinaryOperationNode> result_operation = make_unique<BinaryOperationNode>();
		result_operation->children.push_back(move(temp_expression));
		result_operation->operation = "||";

		while (true) {
			if (symbol != Or)
				break;
			nextSymbol();
			result_operation->children.push_back(orExpression());
		}
		return result_operation;
	}
}

unique_ptr<VariableNode> variable() {
	unique_ptr<VariableNode> result_variable = make_unique<VariableNode>();
	result_variable->name = current_id;
	accept(Ident);

	while (symbol == LBracket) {
		nextSymbol();

		unique_ptr<ExpressionNode> current_expression = expression();
		accept(RBracket);
		result_variable->index_expressions.push_back(move(current_expression));
	}
	return result_variable;
}

unique_ptr<ExpressionNode> atom() {
	unique_ptr<ExpressionNode> result_expression;

	if (symbol != Number && symbol != LogicalValue) {
		if (symbol == Ident) {
			int ident_index = prev_symbol_index;
			int ident_pos = prev_symbol_pos;
			int ident_line = prev_symbol_line;
			nextSymbol();

			if (symbol == LParen) {
				backupSymbolData(ident_index, ident_pos, ident_line);
				result_expression = functionCall();
			}

			else {
				backupSymbolData(ident_index, ident_pos, ident_line);
				result_expression = variable();
			}
		}

		else if (symbol == LParen) {
			nextSymbol();
			unique_ptr<ParenExpressionNode> paren_expression = make_unique<ParenExpressionNode>();
			paren_expression->child = expression();
			accept(RParen);
			return paren_expression;
		}

		else
			error("Atom was expected");
	}

	else {
		switch (symbol) {
		case Number:
			if (is_real_number) {
				unique_ptr<DoubleLiteralNode> real_number_expression = make_unique<DoubleLiteralNode>();
				real_number_expression->value = number_value;
				nextSymbol();
				return real_number_expression;
			}
			else {
				unique_ptr<IntegerLiteralNode> integer_number_expression = make_unique<IntegerLiteralNode>();
				integer_number_expression->value = number_value;
				nextSymbol();
				return integer_number_expression;
			}
			break;

		case LogicalValue:
			unique_ptr<BooleanLiteralNode> boolean_expression = make_unique<BooleanLiteralNode>();
			if (current_id == "true")
				boolean_expression->value = true;
			else
				boolean_expression->value = false;
			nextSymbol();
			return boolean_expression;
			break;
		}
	}
	return result_expression;
}

unique_ptr<CoutNode> coutPrinting() {
	unique_ptr<CoutNode> result_command = make_unique<CoutNode>();
	accept(Cout);
	do {
		accept(LeftLeft);
		if (symbol != StringLiteral)
			result_command->printing_expressions.push_back(expression());
		else {
			unique_ptr<StringLiteralNode> result_literal = make_unique<StringLiteralNode>();
			result_literal->value = string_literal;
			result_command->printing_expressions.push_back(move(result_literal));
			nextSymbol();
		}
	} while (symbol == LeftLeft);

	return result_command;
}

unique_ptr<VariableDeclarationNode> variableDeclaration() {
	unique_ptr<VariableDeclarationNode> result_command = make_unique<VariableDeclarationNode>();
	result_command->variable_type = dataType();

	while (true) {
		unique_ptr<VariableNode> temp_var = variable();
		VariableDefinitionPart temp_def;
		temp_def.name = temp_var->name;
		temp_def.arrays_sizes = move(temp_var->index_expressions);

		if (symbol == Equals) {
			nextSymbol();
			temp_def.value = expression();
		}
		else temp_def.value = nullptr;

		result_command->declarations.push_back(move(temp_def));

		if (symbol != Comma)
			break;
		else
			nextSymbol();
	}
	return result_command;
}

unique_ptr<IncDecNode> prefixIncrement() {
	unique_ptr<IncDecNode> result_command = make_unique<IncDecNode>();
	accept(Inc);
	result_command->is_inc = true;
	result_command->variable = variable();

	return result_command;
}

unique_ptr<IncDecNode> prefixDecrement() {
	unique_ptr<IncDecNode> result_command = make_unique<IncDecNode>();
	accept(Dec);
	result_command->variable = variable();

	return result_command;
}

unique_ptr<StatementNode> simpleCommand() {
	unique_ptr<StatementNode> result_command;

	int command_start_index = prev_symbol_index;
	int command_start_pos = prev_symbol_pos;
	int command_start_line = prev_symbol_line;

	switch (symbol) {
	case Cout:
		backupSymbolData(command_start_index, command_start_pos, command_start_line);
		result_command = coutPrinting();
		break;

	case Return: {
		unique_ptr<ReturnNode> temp_return = make_unique<ReturnNode>();
		nextSymbol();
		temp_return->returning_expression = expression();
		result_command = move(temp_return);
		break;
	}

	case Break: {
		result_command = make_unique<BreakNode>();
		nextSymbol();
		break;
	}

	case Continue: {
		result_command = make_unique<ContinueNode>();
		nextSymbol();
		break;
	}

	case Inc:
		backupSymbolData(command_start_index, command_start_pos, command_start_line);
		result_command = prefixIncrement();
		break;

	case Dec:
		backupSymbolData(command_start_index, command_start_pos, command_start_line);
		result_command = prefixDecrement();
		break;

	case Int: case Bool:
	case Double: case Long:
		backupSymbolData(command_start_index, command_start_pos, command_start_line);
		result_command = variableDeclaration();
		break;

	case Ident:
		nextSymbol();

		if (symbol == LParen) {
			backupSymbolData(command_start_index, command_start_pos, command_start_line);
			unique_ptr<FunctionCallAsStatementNode> temp_func_call = make_unique<FunctionCallAsStatementNode>();
			temp_func_call->function_call = functionCall();
			result_command = move(temp_func_call);
			break;
		}

		else {
			backupSymbolData(command_start_index, command_start_pos, command_start_line);
			unique_ptr<VariableNode> temp_var = variable();

			if (symbol == Inc || symbol == Dec) {
				unique_ptr<IncDecNode> temp_inc_dec = make_unique<IncDecNode>();
				temp_inc_dec->variable = move(temp_var);
				if (symbol == Inc)
					temp_inc_dec->is_inc = true;
				result_command = move(temp_inc_dec);
				nextSymbol();
				break;
			}

			else if (symbol == PlusEq || symbol == MinusEq || symbol == MultEq || symbol == DivEq || symbol == ModEq || symbol == Equals) {
				unique_ptr<AssignmentNode> temp_assignment = make_unique<AssignmentNode>();
				temp_assignment->variable = move(temp_var);

				switch (symbol) {
				case PlusEq:
					temp_assignment->operation = "+=";
					break;
				case MinusEq:
					temp_assignment->operation = "-=";
					break;
				case MultEq:
					temp_assignment->operation = "*=";
					break;
				case DivEq:
					temp_assignment->operation = "/=";
					break;
				case ModEq:
					temp_assignment->operation = "%=";
					break;
				case Equals:
					temp_assignment->operation = "=";
					break;
				}

				nextSymbol();
				temp_assignment->value = expression();
				result_command = move(temp_assignment);
				break;
			}

			else
				error("Assignment was expected");
		}

	default:
		error("Simple command was expected");
	}
	return result_command;
}

unique_ptr<IfNode> ifCommand() {
	unique_ptr<IfNode> result_command = make_unique<IfNode>();
	accept(If);
	accept(LParen);
	result_command->condition = expression();
	accept(RParen);
	result_command->command = command();
	if (symbol == Else) {
		nextSymbol();
		result_command->else_command = command();
	}
	else
		result_command->else_command = nullptr;

	return result_command;
}

unique_ptr<ExpressionNode> whileCondition() {
	accept(While);
	accept(LParen);
	unique_ptr<ExpressionNode> result_condition = expression();
	accept(RParen);

	return result_condition;
}

unique_ptr<WhileNode> whileCommand() {
	unique_ptr<WhileNode> result_command = make_unique<WhileNode>();
	result_command->condition = whileCondition();
	result_command->command = command();

	return result_command;
}

unique_ptr<WhileNode> doWhileCommand() {
	unique_ptr<WhileNode> result_command = make_unique<WhileNode>();
	result_command->is_do_while = true;
	accept(Do);
	result_command->command = command();
	result_command->condition = whileCondition();

	return result_command;
}

unique_ptr<ForNode> forCommand() {
	unique_ptr<ForNode> result_command = make_unique<ForNode>();
	accept(For);
	accept(LParen);
	if (symbol != Semicolon)
		result_command->initialization = simpleCommand();
	else
		result_command->initialization = nullptr;
	accept(Semicolon);
	if (symbol != Semicolon)
		result_command->condition = expression();
	else
		result_command->condition = nullptr;
	accept(Semicolon);
	if (symbol != RParen)
		result_command->step = simpleCommand();
	else
		result_command->step = nullptr;
	accept(RParen);
	result_command->command = command();

	return result_command;
}

unique_ptr<DefaultNode> defaultCaseCommand() {
	unique_ptr<DefaultNode> result_command = make_unique<DefaultNode>();
	accept(Default);
	accept(Colon);
	while (symbol != RBrace)
		result_command->commands.push_back(command());

	return result_command;
}

unique_ptr<CaseNode> caseCommand() {
	unique_ptr<CaseNode> result_command = make_unique<CaseNode>();
	accept(Case);
	result_command->condition = expression();
	accept(Colon);
	while (symbol != Case && symbol != RBrace && symbol != Default)
		result_command->commands.push_back(command());

	return result_command;
}

unique_ptr<SwitchNode> switchCommand() {
	unique_ptr<SwitchNode> result_command = make_unique<SwitchNode>();
	accept(Switch);
	accept(LParen);
	result_command->expression_for_switch = expression();
	accept(RParen);
	accept(LBrace);
	while (symbol != RBrace && symbol != Default)
		result_command->cases.push_back(caseCommand());

	if (symbol == Default) {
		result_command->default_case = defaultCaseCommand();
	}
	nextSymbol();

	return result_command;
}

unique_ptr<FunctionCallNode> functionCall() {
	unique_ptr<FunctionCallNode> result_call = make_unique<FunctionCallNode>();
	result_call->name = current_id;
	accept(Ident);
	accept(LParen);
	if (symbol != RParen) {
		while (true) {
			result_call->argument_expressions.push_back(expression());
			if (symbol != Comma)
				break;
			else
				nextSymbol();
		}
		accept(RParen);
	}
	else
		nextSymbol();

	return result_call;
}

unique_ptr<StatementNode> command() {
	unique_ptr<StatementNode> result_command;
	switch (symbol) {
	case If:
		result_command = ifCommand();
		break;

	case For:
		result_command = forCommand();
		break;

	case Switch:
		result_command = switchCommand();
		break;

	case While:
		result_command = whileCommand();
		break;

	case Pragma: {
		nextSymbol();
		accept(AnalysisDisable);
		accept(Pragma);
		accept(LBrace);

		unique_ptr<NonAnalysisBlockNode> temp_block = make_unique<NonAnalysisBlockNode>();
		while (symbol != RBrace)
			temp_block->commands.push_back(command());
		result_command = move(temp_block);
		nextSymbol();
		break;
	}

	case LBrace: {
		unique_ptr<BlockNode> temp_block = make_unique<BlockNode>();
		nextSymbol();
		while (symbol != RBrace)
			temp_block->commands.push_back(command());
		result_command = move(temp_block);
		nextSymbol();
		break;
	}

	default:
		if (symbol == Do)
			result_command = doWhileCommand();
		else
			result_command = simpleCommand();
		accept(Semicolon);
		break;
	}

	return result_command;
}

vector<unique_ptr<ArgumentNode>> functionArguments() {
	vector<unique_ptr<ArgumentNode>> result_arguments;

	while (true) {
		unique_ptr<ArgumentNode> current_argument = make_unique<ArgumentNode>();
		current_argument->type = dataType();

		current_argument->name = current_id;
		accept(Ident);

		bool bracket_flag = false;

		while (symbol == LBracket) {
			nextSymbol();

			if (symbol == RBracket) {
				if (bracket_flag)
					error("[] must be listed once first in order");
				else {
					current_argument->is_array = true;
					nextSymbol();
				}
			}

			else {
				unique_ptr<ExpressionNode> current_expression = expression();
				accept(RBracket);
				current_argument->arrays_sizes.push_back(move(current_expression));
			}

			bracket_flag = true;
		}

		result_arguments.push_back(move(current_argument));

		if (symbol != Comma)
			break;
		else
			nextSymbol();
	}
	return result_arguments;
}

vector<unique_ptr<StatementNode>> functionDefinition() {
	vector<unique_ptr<StatementNode>> result_statements;

	accept(LBrace);
	while (symbol != RBrace)
		result_statements.push_back(command());

	nextSymbol();

	return result_statements;
}

unique_ptr<FunctionDefinitionNode> functionDeclaration() {
	unique_ptr<FunctionDefinitionNode> result_func_declaration = make_unique<FunctionDefinitionNode>();
	if (symbol != Void)
		result_func_declaration->type = dataType();
	else {
		result_func_declaration->type = "void";
		nextSymbol();
	}
	result_func_declaration->name = current_id;

	accept(Ident);
	accept(LParen);

	if (symbol != RParen) {
		result_func_declaration->arguments = functionArguments();
		accept(RParen);
	}
	else
		nextSymbol();

	if (symbol != Semicolon) {
		result_func_declaration->hasDefinition = true;
		result_func_declaration->commands = functionDefinition();
	}
	else {
		result_func_declaration->hasDefinition = false;
		nextSymbol();
	}

	if (result_func_declaration->name == "main" && result_func_declaration->hasDefinition)
		has_main = true;

	return result_func_declaration;
}

Program program() {
	Program result_program;

	accept(Parameter);
	if (symbol == Ident) {
		result_program.parameter_name = current_id;
		nextSymbol();
		accept(LParen);

		result_program.parameter_min = number_value;
		accept(Number);

		accept(Comma);

		result_program.parameter_max = number_value;
		accept(Number);

		accept(Comma);

		result_program.parameter_step = number_value;
		accept(Number);

		accept(RParen);
		accept(Semicolon);
	}
	else
		error("Complexity parameter name was expected");

	while (symbol != EndOfFile) {
		result_program.functions.push_back(functionDeclaration());
	}

	if (!has_main)
		error("There is no starting point for program (described main function)");

	return result_program;
}

int main() {
	initText();
	Program current_program = program();
	TranslatingVisitor visitor;
	current_program.visit(visitor);

	cout << SUCCESS_COLORIZED_COUT << "Transpilation was compiled successfully" << STOP_COLORIZED_COUT << "\n\n";

	/**
	* Example of how to automately run transpilation with uncrustify.
	*
	* On Windows:
	* system("uncrustify -c msvc.cfg -f finalOutput.cpp --no-backup -o finalOutput.cpp");
	* system("g++ -O3 -std=c++20 -o output_program.exe finalOutput.cpp pbPlots.cpp supportLib.cpp -lm -D _WIN32_WINNT=0x0A00");
	* system("start output_program.exe");
	*
	* On Unix:
	* system("uncrustify -c msvc.cfg -f finalOutput.cpp --no-backup -o finalOutput.cpp");
	* system("g++ -O3 -std=c++20 -o output_program.out finalOutput.cpp pbPlots.cpp supportLib.cpp");
	* system("./output_program.out");
	*/
}