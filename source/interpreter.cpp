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


enum LEXEM_TYPE {
	NUMBER, OP, VAR
};

enum OPERATOR {
	LBRACKET, RBRACKET,
	ASSIGN,
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
	-1, -1,
	0,
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
	"(", ")",
	"=",
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
	void setValue(int value);
};

map<string, int> varmap;

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

Variable::Variable(const string & name) {
	this->name = name;
	varmap.insert(pair<string, int>(name, 0));
	Lexem::setLexemType(VAR);
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
	if (ch >= 'a' && ch <= 'z')
		return true;
	return false;
}


Operator *check_operator(string codeline, int *i) {
	int n = sizeof(OPERTEXT) / sizeof(string);
	for (int op = 0; op < n; op++) {
		string subcodeline = 
			codeline.substr(*i, OPERTEXT[op].size());
		if (OPERTEXT[op] == subcodeline) {
			(*i) += subcodeline.size() - 1;
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
		(*i)--;
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
		(*i)--;
		return new Variable(tmp);
	}
	return nullptr;
}



vector<Lexem *> parseLexem(string codeline) {
	vector<Lexem *> infix;
	for (int i = 0; i < codeline.size(); i++) {
		if (codeline[i] == ' ' || codeline[i] == '\t') {
			continue;
		}
		Number *ptr_n = check_number(codeline, &i);
		if (ptr_n) {
			infix.push_back(ptr_n);
		}
		else {
			Variable *ptr_v = check_var(codeline, &i);
			if (ptr_v) {
				infix.push_back(ptr_v);
			}
			else {
				Operator *ptr_o = check_operator(codeline, &i);
				if (ptr_o) {
					infix.push_back(ptr_o);
				}
			}
		}
		
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

	binaryOperBuildPostfix(stack, infix, postfix, operType, i);
	
	stack.push(infix[i]);
}

vector<Lexem *> buildPostfix(vector<Lexem *> infix) {
	vector<Lexem *> postfix;
	stack <Lexem *> stack;
	int j = 0;
	for (int i = 0; i < infix.size(); i++) {
		if ((infix[i]->getLexemType()) == NUMBER || (infix[i]->getLexemType()) == VAR) {
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

int evaluatePostfix(vector<Lexem *> postfix) {
	stack <Lexem *> stack;
	for (int i = 0; i < postfix.size(); i++) {
		if (postfix[i]->getLexemType() == NUMBER || postfix[i]->getLexemType() == VAR) {
			stack.push(postfix[i]);
			continue;
		}
		if (postfix[i]->getLexemType() == OP) {
			Lexem *value2 = stack.top();
			stack.pop();
			Lexem *value1 = stack.top();
			stack.pop();
			stack.push(new Number(static_cast<Operator *>(postfix[i])->evaluate(value1, value2)));
		}
	}
	int ans = static_cast<Number *>(stack.top())->getValue();
	stack.pop();
	return ans;
}

int main() {
	string codeline;
	vector<Lexem *> infix;
	vector<Lexem *> postfix;
	int value;
	while (std::getline(cin, codeline)) {
		infix = parseLexem(codeline);
		//cout << infix.size() << endl;// << ' ' << varmap.size();
		//cout << static_cast<Operator *>(infix[1])->getType();
		//for (int i = 0; i < infix.size(); i++) {
		//	cout << "i: " << i << " op: " << infix[i]->getLexemType() << endl;
		//}
		postfix = buildPostfix(infix);
		//cout << postfix.size() << endl;
		//cout << postfix.size() << ' ' << varmap.size() << endl;
		//for (int i = 0; i < postfix.size(); i++) {
		//	cout << "i: " << i << " op: " << postfix[i]->getLexemType() << endl;
		//}
		value = evaluatePostfix(postfix);
		cout << value << endl;
	}
	return 0;
}
