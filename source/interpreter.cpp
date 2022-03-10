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

enum LEXEM_TYPE {
	NUMBER, OP, VAR
};

enum OPERATOR {
	LBRACKET, RBRACKET,
	ASSIGN,
	PLUS, MINUS,
	MULTIPLY
};

int PRIORITY[] = {
	-1, -1,
	0,
	1, 1,
	2
};


class Lexem {
	LEXEM_TYPE lexem_type;
public:
	Lexem() {}
	LEXEM_TYPE getLexemType();
	friend vector<Lexem *> parseLexem(string codeline);
};

class Number: public Lexem {
	int value;
public:
	Number();
	Number(int someValue);
	int getValue();
};

class Operator: public Lexem {
	OPERATOR opertype;
public:
	Operator();
	Operator(OPERATOR status);
	OPERATOR getType();
	int getPriority();
	int evaluate(const Number & left, const Number & right);
};

class Variable: public Lexem {
	string name;
	int value;
public:
	Variable(const string & name);
	int getValue();
	void setValue(int value);
};

//map<Variable *, int> vars;

LEXEM_TYPE Lexem::getLexemType() {
	return lexem_type;
}


/////////////////////// class Variable methods ///////////////////////


Variable::Variable(const string & name) {
	this->name = name;
}

int Variable::getValue() {
	return value;
}

void Variable::setValue(int value) {
	this->value = value;
}


////////////////////// class Number methods ////////////////////////////



Number::Number() : value(0) {
}

Number::Number(int someValue) {
	value = someValue;
}

int Number::getValue() {
	return value;
}


////////////////////// class Operator methods ///////////////////////////



Operator::Operator(OPERATOR status) {
	opertype = status;
}

OPERATOR Operator::getType() {
	return opertype;
}

int Operator::getPriority() {
	return PRIORITY[opertype];
}

int Operator::evaluate(const Number & left, const Number & right) {
	switch(opertype) {
	case PLUS:
		return static_cast<Number>(left).getValue() + static_cast<Number>(right).getValue();
	case MINUS:
		return static_cast<Number>(left).getValue() - static_cast<Number>(right).getValue();
	case MULTIPLY:
		return static_cast<Number>(left).getValue() * static_cast<Number>(right).getValue();
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


vector<Lexem *> parseLexem(string codeline) {
	vector<Lexem *> infix;
	for (int i = 0; i < codeline.size(); i++) {
		if (codeline[i] == ' ' || codeline[i] == '\t')
			continue;
		if (codeline[i] == '+') {
			infix.push_back(new Operator(PLUS));
			infix[infix.size() - 1]->lexem_type = OP;
			continue;
		}
		if (codeline[i] == '-') {
			infix.push_back(new Operator(MINUS));
			infix[infix.size() - 1]->lexem_type = OP;
			continue;
		}
		if (codeline[i] == '*') {
			infix.push_back(new Operator(MULTIPLY));
			infix[infix.size() - 1]->lexem_type = OP;
			continue;
		}
		if (codeline[i] == '(') {
			infix.push_back(new Operator(LBRACKET));
			infix[infix.size() - 1]->lexem_type = OP;
			continue;
		}
		if (codeline[i] == ')') {
			infix.push_back(new Operator(RBRACKET));
			infix[infix.size() - 1]->lexem_type = OP;
			continue;
		}
		if (codeline[i] == '=') {
			infix.push_back(new Operator(ASSIGN));
			infix[infix.size() - 1]->lexem_type = OP;
			continue;
		}
		if (isDigit(codeline[i])) {
			int number = 0;
			while (isDigit(codeline[i])) {
				number = number * 10 + codeline[i] - '0';
				i++;
			}
			i--;
			infix.push_back(new Number(number));
			infix[infix.size() - 1]->lexem_type = NUMBER;
			continue;
		}
		if (isLetter(codeline[i])) {
			string tmp;
			tmp[0] = codeline[i];
			infix.push_back(new Variable(tmp));
			infix[infix.size() - 1]->lexem_type = VAR;
		}
	}
	return infix;
}

bool isLeftAssociated(OPERATOR opType) {
	if (opType == ASSIGN)
		return true;
	return false;
}

vector<Lexem *> buildPostfix(vector<Lexem *> infix) {
	vector<Lexem *> postfix;
	stack <Lexem *> stack;
	int j = 0;
	for (int i = 0; i < infix.size(); i++) {
		if ((infix[i]->getLexemType()) == NUMBER || (infix[i]->getLexemType()) == VAR) {
			postfix.push_back(infix[i]);
		}

		else if((infix[i]->getLexemType()) == OP) {
			OPERATOR operType = static_cast<Operator *>(infix[i])->getType();
			if (operType == LBRACKET) {
				stack.push(infix[i]);
				continue;
			}
			if (operType == RBRACKET) {
				Lexem *tmp = stack.top();
				while (static_cast<Operator *>(tmp)->getType() != LBRACKET) {
					postfix.push_back(tmp);
					stack.pop();
					tmp = stack.top();
				}
				stack.pop();
				continue;
			}
			
			//if we see binary operand

			if(stack.size() != 0) {
				Lexem *tmp = stack.top();
				if (isLeftAssociated(operType)) {
					while (static_cast<Operator *>(tmp)->getPriority() >= 
									static_cast<Operator *>(infix[i])->getPriority()) {
						postfix.push_back(tmp);
						stack.pop();
						tmp = stack.top();
					}
				}
				else {
					while (static_cast<Operator *>(tmp)->getPriority() > 
									static_cast<Operator *>(infix[i])->getPriority()) {
						postfix.push_back(tmp);
						stack.pop();
						tmp = stack.top();
					}
				}
			}
			stack.push(infix[i]);
		}
	}
	while (stack.size() > 0) {
		postfix.push_back(stack.top());
		stack.pop();
	}
	return postfix;
}

int evaluatePostfix(vector<Lexem *> postfix) {
	stack <int> stack;
	for (int i = 0; i < postfix.size(); i++) {
		if (postfix[i]->getLexemType() == NUMBER) {
			stack.push(static_cast<Number *>(postfix[i])->getValue());
			continue;
		}
		if (postfix[i]->getLexemType() == OP) {
			int value2 = stack.top();
			stack.pop();
			int value1 = stack.top();
			stack.pop();
			stack.push(static_cast<Operator *>(postfix[i])->evaluate(Number(value1), Number(value2)));
		}
	}
	int ans = stack.top();
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
		cout << infix.size();
		postfix = buildPostfix(infix);
		//value = evaluatePostfix(postfix);
		//cout << value << endl;
	}
	return 0;
}