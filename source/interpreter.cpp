#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>

using std::vector;
using std::string;
using std::stack;
using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::pair;

map<string, int> varmap;
map<string, int> labels;

enum LEXEM_TYPE {
	NUMBER, OP, VAR
};

enum OPERATOR {
	PRINT,
	IF, THEN,
	ELSE, ENDIF,
	WHILE, ENDWHILE,
	GOTO, ASSIGN, COLON,
	LBRACKET, RBRACKET,
	OR,
	AND,
	BITOR,
	XOR,
	BITAND,
	EQ, NEQ,
	LEQ, SHL,
	GEQ, SHR,
	LT, GT,
	/*LEQ, LT,
	GEQ, GT,
	SHL, SHR,*/
	PLUS, MINUS,
	MULT, DIV, MOD
};

int PRIORITY[] = {
	-1,
	-1, -1,
	-1, -1,
	-1, -1,
	-1, 0, -1,
	-1, -1,
	1,
	2,
	3,
	4,
	5,
	6, 6,
	7, 8,
	7, 8,
	7, 7,
	9, 9,
	10, 10, 10
};

string OPERTEXT[] = {
	"print",
	"if", "then",
	"else", "endif",
	"while", "endwhile",
	"goto", ":=", ":",
	"(", ")",
	"or",
	"and",
	"|",
	"^",
	"&",
	"==", "!=",
	"<=", "<<",
	">=", ">>",
	"<", ">",
	"+", "-",
	"*", "/", "%"
};


class Lexem {
	LEXEM_TYPE lexem_type;
public:
	Lexem() {}
	LEXEM_TYPE getLexemType();
	void setLexemType(LEXEM_TYPE type);
	friend vector<Lexem *> parseLexem(string codeline);
};

class Number: public Lexem {
	int value;
public:
	Number();
	Number(int someValue);
	int getValue();
};

class Variable;

class Operator: public Lexem {
	OPERATOR opertype;
public:
	Operator();
	Operator(OPERATOR status);
	int evaluateNum(int a, int b);
	int evaluateVar(Variable *var, int b);
	OPERATOR getType();
	int getPriority();
	int evaluate(Lexem *left, Lexem *right);
};

class Variable: public Lexem {
	string name;
	//int value;
public:
	Variable(const string & name);
	int getValue();
	string getName();
	void setValue(int value);
	bool inLabelTable();
};

class Goto: public Operator {
	int row;
public:
	enum { UNDEFINED = -INT32_MAX };
	Goto(OPERATOR optype) : Operator(optype) {
		row = UNDEFINED;
	}
	void setRow(int row) {
		this->row = row;
	}
	int getRow() {
		return row;
	}
};




LEXEM_TYPE Lexem::getLexemType() {
	return lexem_type;
}


void Lexem::setLexemType(LEXEM_TYPE type) {
	lexem_type = type;
}

/////////////////////// class Variable methods ///////////////////////


int Variable::getValue() {
	return varmap[name];
}

void Variable::setValue(int value) {
	varmap[name] = value;
}

string Variable::getName() {
	return name;
}

Variable::Variable(const string & name) {
	this->name = name;
	varmap.insert(pair<string, int>(name, 0));
	Lexem::setLexemType(VAR);
}

bool Variable::inLabelTable() {
	return labels.find(name) != labels.end();
}


////////////////////// class Number methods ////////////////////////////



Number::Number() : value(0) {
}

Number::Number(int someValue) {
	value = someValue;
	Lexem::setLexemType(NUMBER);
}

int Number::getValue() {
	return value;
}


////////////////////// class Operator methods ///////////////////////////



Operator::Operator(OPERATOR status) {
	opertype = status;
	Lexem::setLexemType(OP);
}

OPERATOR Operator::getType() {
	return opertype;
}

int Operator::getPriority() {
	return PRIORITY[opertype];
}


int Operator::evaluateNum(int a, int b) {
	switch(this->opertype) {
		case PLUS:
			return a + b;
		case MINUS:
			return a - b;
		case MULT:
			return a * b;
		case ASSIGN:
			return b;
		case OR:
			return a or b;
		case AND:
			return a and b;
		case BITOR:
			return a | b;
		case XOR:
			return a ^ b;
		case BITAND:
			return a & b;
		case EQ:
			return a == b;
		case NEQ:
			return a != b;
		case LEQ:
			return a <= b;
		case LT:
			return a < b;
		case GEQ:
			return a >= b;
		case GT:
			return a > b;
		case SHL:
			return a << b;
		case SHR:
			return a >> b;
		case DIV:
			return (b == 0) ? 0 : (a / b);
		case MOD:
			return a % b;

	}

}

int Operator::evaluateVar(Variable *var, int b) {
	int a = var->getValue();
	if (this->opertype != ASSIGN) {
		return evaluateNum(a, b);
	}
	else {
		var->setValue(b);
		return b;
	}
}

int Operator::evaluate(Lexem *left, Lexem *right) {
	LEXEM_TYPE l = left->getLexemType(), r = right->getLexemType();
	if (l == NUMBER && r == NUMBER) {
		return evaluateNum(static_cast<Number *>(left)->getValue(), static_cast<Number *>(right)->getValue());	
	}
	if (l == NUMBER && r == VAR) {
		return evaluateNum(static_cast<Number *>(left)->getValue(), static_cast<Variable *>(right)->getValue());
	}
	if (l == VAR && r == NUMBER) {
		return evaluateVar(static_cast<Variable *>(left), static_cast<Number *>(right)->getValue());
	}
	if (l == VAR && r == VAR) {
		return evaluateVar(static_cast<Variable *>(left), static_cast<Variable *>(right)->getValue());
	}
}



//////////////////////End of classes methods //////////////////////////





bool isDigit(char ch) {
	if (ch >= '0' && ch <= '9') 
		return true;
	return false;
}

bool isLetter(char ch) {
	if (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
		return true;
	return false;
}



void initLabels(vector<Lexem *> &infix, int row) {
	for (int i = 1; i < infix.size(); i++) {
		if (infix[i - 1]->getLexemType() == VAR && infix[i]->getLexemType() == OP) {
			Variable *lexemvar = static_cast<Variable *>(infix[i - 1]);
			Operator *lexemop = static_cast<Operator *>(infix[i]);
			if (lexemop->getType() == COLON) {
				if (!lexemvar->inLabelTable()) {
					labels.insert(pair<string, int>(lexemvar->getName(), row));
				}
				else {
					labels[lexemvar->getName()] == row;
				}
				delete infix[i - 1];
				delete infix[i];
				infix[i - 1] = nullptr;
				infix[i] = nullptr;
				i++;
			}
		}
	}
}

void print(vector <Lexem *> vec);

void initJumps(vector< vector <Lexem *> > infixes) {
	stack<Goto *> stackIfElse;
	stack<Goto *> stackWhile;
	for (int row = 0; row < infixes.size(); row++) {
		for (int i = 0; i < infixes[row].size(); i++) {
			if (infixes[row][i] == nullptr)
				continue;
			if (infixes[row][i]->getLexemType() == OP) {
				Operator *lexemoper = static_cast<Operator *>(infixes[row][i]);

				if (lexemoper->getType() == IF) {
					stackIfElse.push(static_cast<Goto *>(lexemoper));
				}
				if (lexemoper->getType() == ELSE) {
					stackIfElse.top()->setRow(row + 1);
					stackIfElse.pop();
					stackIfElse.push(static_cast<Goto *>(lexemoper));
				}
				if (lexemoper->getType() == ENDIF) {
					stackIfElse.top()->setRow(row + 1);
					stackIfElse.pop();
				}
				if (lexemoper->getType() == WHILE) {
					Goto *lexemgoto = static_cast<Goto *>(lexemoper);
					lexemgoto->setRow(row);
					stackWhile.push(lexemgoto);
				}
				if (lexemoper->getType() == ENDWHILE) {
					Goto *lexemgoto = static_cast<Goto *>(lexemoper);
					lexemgoto->setRow(stackWhile.top()->getRow());
					stackWhile.top()->setRow(row + 1);
					stackWhile.pop();
				}
			}
		}
	}
}


Operator *check_operator(string codeline, int *i) {
	int n = sizeof(OPERTEXT) / sizeof(string);
	for (int op = 0; op < n; op++) {
		string subcodeline = 
			codeline.substr(*i, OPERTEXT[op].size());
		if (OPERTEXT[op] == subcodeline) {
			(*i) += subcodeline.size();
			if (op == GOTO || op == IF || op == ELSE || op == WHILE || op == ENDWHILE || op == ENDIF)
				return new Goto(OPERATOR(op));
			return new Operator(OPERATOR(op));
		}
	}
	return nullptr;
}

Number *check_number(string codeline, int *i) {
	if (isDigit(codeline[(*i)])) {
		int number = 0;
		while (isDigit(codeline[(*i)])) {
			number = number * 10 + codeline[(*i)] - '0';
			(*i)++;
		}
		return new Number(number);
	}
	return nullptr;
}

Variable *check_var(string codeline, int *i) {
	if (isLetter(codeline[(*i)])) {
		string tmp;
		while (isLetter(codeline[(*i)])) {
			tmp.push_back(codeline[(*i)]);
			(*i)++;
		}
		//(*i)--;
		return new Variable(tmp);
	}
	return nullptr;
}



vector<Lexem *> parseLexem(string codeline) {
	vector<Lexem *> infix;
	for (int i = 0; i < codeline.size();) {
		if (codeline[i] == ' ' || codeline[i] == '\t') {
			i++;
			continue;
		}
		Number *ptr_n = check_number(codeline, &i);
		if (ptr_n) {
			infix.push_back(ptr_n);
			continue;
		}
		
		Operator *ptr_o = check_operator(codeline, &i);
		if (ptr_o) {
			infix.push_back(ptr_o);
			continue;
		}

		Variable *ptr_v = check_var(codeline, &i);
		if (ptr_v) {
			infix.push_back(ptr_v);
			continue;
		}
		
		i++;
	}
	return infix;
}


bool isRightAssociated(OPERATOR opType) {
	if (opType == ASSIGN)
		return true;
	return false;
}

void binaryOperBuildPostfix(stack<Lexem *> & stack, vector<Lexem *> & infix,
							 vector<Lexem *> & postfix, OPERATOR operType, int i) {
	if(stack.size() != 0) {
		if (isRightAssociated(operType)) {
			while (stack.size() != 0 && static_cast<Operator *>(stack.top())->getPriority() > 
							static_cast<Operator *>(infix[i])->getPriority()) {
				postfix.push_back(stack.top());
				stack.pop();
			}
		}
		else {
			while (stack.size() != 0 && static_cast<Operator *>(stack.top())->getPriority() >= 
							static_cast<Operator *>(infix[i])->getPriority()) {

				postfix.push_back(stack.top());
				stack.pop();
			}
		}
	}
}

void OperBuildPosfix(stack<Lexem *> & stack, vector<Lexem *> & infix,
						 vector<Lexem *> & postfix, int i) {
	OPERATOR operType = static_cast<Operator *>(infix[i])->getType(	);
	if (operType == LBRACKET) {
		stack.push(infix[i]);
		return;
	}
	if (operType == RBRACKET) {
		Lexem *tmp = stack.top();
		while (static_cast<Operator *>(tmp)->getType() != LBRACKET) {
			postfix.push_back(tmp);
			stack.pop();
			tmp = stack.top();
		}
		stack.pop();
		return;
	}
	if (operType == PRINT) {
		stack.push(infix[i]);
		return;
	}
	if (operType == GOTO || operType == WHILE || operType == IF || operType == ELSE || operType == ENDWHILE) {
		if (operType == GOTO) {
			static_cast<Goto *>(infix[i])->setRow(labels[static_cast<Variable *>(infix[i + 1])->getName()]);
		}
		stack.push(infix[i]);
		return;
	}

	if (operType == ENDIF || operType == THEN) {
		return;
	}

	binaryOperBuildPostfix(stack, infix, postfix, operType, i);
	
	stack.push(infix[i]);
}


vector<Lexem *> buildPostfix(vector<Lexem *> infix) {
	vector<Lexem *> postfix;
	stack <Lexem *> stack;
	int j = 0;
	for (int i = 0; i < infix.size(); i++) {
		if (infix[i] == nullptr) {
			continue;
		}
		if ((infix[i]->getLexemType()) == NUMBER) {
			postfix.push_back(infix[i]);
			continue;
		}

		if ((infix[i]->getLexemType()) == VAR) {
			postfix.push_back(infix[i]);
			continue;
		}

		else if((infix[i]->getLexemType()) == OP) {
			OperBuildPosfix(stack, infix, postfix, i);
		}
	}
	if (stack.size() != 0) {
		while (stack.size() > 0) {
			postfix.push_back(stack.top());
			stack.pop();
		}
	}
	return postfix;
}

int evaluatePostfix(vector<Lexem *> postfix, int row) {
	stack <Lexem *> stack;
	vector <Number *> newArr;
	for (int i = 0; i < postfix.size(); i++) {
		if (postfix[i]->getLexemType() == NUMBER || postfix[i]->getLexemType() == VAR) {
			if (postfix[i]->getLexemType() == VAR) {
				if (static_cast<Variable *>(postfix[i])->inLabelTable()) {
					continue;
				}
			}
			stack.push(postfix[i]);
			continue;
		}
		if (postfix[i]->getLexemType() == OP) {
			Operator *lexemop = static_cast<Operator *>(postfix[i]);
			if (lexemop->getType() == GOTO || lexemop->getType() == ELSE || lexemop->getType() == ENDWHILE) {
				return static_cast<Goto *>(lexemop)->getRow();
			}
			if (lexemop->getType() == IF || lexemop->getType() == WHILE) {
				Goto *lexemgoto = static_cast<Goto *>(lexemop);
				int rvalue = static_cast<Number *>(stack.top())->getValue();
				stack.pop();
				if (!rvalue) {
					return lexemgoto->getRow();
				}
				else {
					return row + 1;
				}
			}
			if (lexemop->getType() == PRINT) {
				int variable_to_print = static_cast<Number *>(stack.top())->getValue();
				cout << static_cast<Variable *>(stack.top())->getValue() << endl;
				stack.pop();

				return row + 1;
			}
			Lexem *value2 = stack.top();
			stack.pop();
			Lexem *value1 = stack.top();
			stack.pop();
			stack.push(new Number(lexemop->evaluate(value1, value2)));
			newArr.push_back(static_cast<Number *>(stack.top()));
		}
	}
	for (int i = 0; i < newArr.size(); i++) {
		delete(newArr[i]);
	}
	int ans = static_cast<Number *>(stack.top())->getValue();
	// cout << ans << endl;
	stack.pop();
	return row + 1;
}

void print(vector<Lexem *> vec) {
	for (int i = 0; i < vec.size(); i++) {
		if (!vec[i]) {
			continue;
		}
		if (vec[i]->getLexemType() == NUMBER) {
			cout << "[" << static_cast<Number *>(vec[i])->getValue() << "]";
		}
		if (vec[i]->getLexemType() == VAR) {
			if (static_cast<Variable *>(vec[i])->inLabelTable()) {
				cout << "[" << static_cast<Variable *>(vec[i])->getName() << "(" << labels[static_cast<Variable *>(vec[i])->getName()] << ")] ";
				continue;
			}
			cout << "[" << static_cast<Variable *>(vec[i])->getName() << "]";
		}
		if (vec[i]->getLexemType() == OP) {
			if (static_cast<Operator *>(vec[i])->getType() == IF || static_cast<Operator *>(vec[i])->getType() == ELSE || static_cast<Operator *>(vec[i])->getType() == WHILE ||
						static_cast<Operator *>(vec[i])->getType() == ENDWHILE)
			{
				cout << "[" << OPERTEXT[static_cast<Operator *>(vec[i])->getType()] << "<" <<
						static_cast<Goto *>(vec[i])->getRow() << ">" << "]";
			}
			else
				cout << "[" << OPERTEXT[static_cast<Operator *>(vec[i])->getType()] << "]"; 
		}
		cout << " ";
	}	
} 

void remove(vector<Lexem *> infix) {
	for (int i = 0; i < infix.size(); i++) {
		if (infix[i] != nullptr) {
			delete(infix[i]);
		}
	}
}

int main() {
	string codeline;
	vector < vector<Lexem *> > infixLines, postfixLines;
	int value;
	while (std::getline(cin, codeline)) {
		infixLines.push_back(parseLexem(codeline));
	}

	for (int row = 0; row < infixLines.size(); row++) {
		initLabels(infixLines[row], row);
	}
	initJumps(infixLines);
	for (const auto &infix: infixLines) {
		postfixLines.push_back(buildPostfix(infix));
	}

	for (int i = 0; i < infixLines.size(); i++) {
		remove(infixLines[i]);
	}

	for (int i = 0; i < postfixLines.size(); i++) {
		print(postfixLines[i]);
		cout << endl;
	}

	 int row = 0;
	 while (0 <= row && row < postfixLines.size()) {
	 	if (postfixLines[row].size() != 0) {
	 		row = evaluatePostfix(postfixLines[row], row);
	 	}
	 	else 
	 		row++;
	}
	return 0;
}