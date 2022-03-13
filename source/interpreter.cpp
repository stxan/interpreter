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
	PLUS, MINUS,
	MULTIPLY, ASSIGN
};

int PRIORITY[] = {
	-1, -1,
	1, 1,
	2, 0
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
	//int value;
public:
	Variable(const string & name);
	int getValue();
	void setValue(int value);
};

map<string, Variable *> varmap;

LEXEM_TYPE Lexem::getLexemType() {
	return lexem_type;
}


void Lexem::setLexemType(LEXEM_TYPE type) {
	lexem_type = type;
}

/////////////////////// class Variable methods ///////////////////////


Variable::Variable(const string & name) {
	this->name = name;
	varmap.insert(pair<string, Variable *>(name, this));
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

char OPERATOR_STR[] = {
	'(', ')', '+', '-', '*'
};

Operator *check_operator(string codeline, int i) {

	for (int j = 0; j < sizeof(OPERATOR_STR); j++) {
		if (codeline[i] == OPERATOR_STR[j]) {
			return new Operator(OPERATOR(j));
			//infix[infix.size() - 1]->lexem_type = OP; // вместо этого запихать в конструктор эти строки
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
	//	cout << number << endl;
		return new Number(number);
	}
	return nullptr;
}


/*for (int i = 0; i < codeline.size(); i++) {
	Operator *ptr_o = check_operator(codeline, &i);
	if (ptr_o) {
		infix.push_back(ptr_o);
	}
	Number *ptr_n = check_number(codeline, &i);
	if (ptr_n) {
		infix.push_back(ptr_n);
	}
}*/

vector<Lexem *> parseLexem(string codeline) {
	vector<Lexem *> infix;
	for (int i = 0; i < codeline.size(); i++) {
		Operator *ptr_o = check_operator(codeline, i);
		if (ptr_o) {
			infix.push_back(ptr_o);
		}
		Number *ptr_n = check_number(codeline, &i);
		if (ptr_n) {
			infix.push_back(ptr_n);
		}
	}
	return infix;
}

/*vector<Lexem *> parseLexem(string codeline) {
	vector<Lexem *> infix;
	for (int i = 0; i < codeline.size(); i++) {
		if (codeline[i] == ' ' || codeline[i] == '\t')
			continue;
		if (codeline[i] == '+') {
			infix.push_back(new Operator(PLUS));
			continue;
		}
		if (codeline[i] == '-') {
			infix.push_back(new Operator(MINUS));
			continue;
		}
		if (codeline[i] == '*') {
			infix.push_back(new Operator(MULTIPLY));
			continue;
		}
		if (codeline[i] == '(') {
			infix.push_back(new Operator(LBRACKET));
			continue;
		}
		if (codeline[i] == ')') {
			infix.push_back(new Operator(RBRACKET));
			continue;
		}
		if (codeline[i] == '=') {
			infix.push_back(new Operator(ASSIGN));
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
			continue;
		}
		if (isLetter(codeline[i])) {
			string tmp;
			int j = 0;
			while (isLetter(codeline[i])) {
				tmp.push_back(codeline[i++]);
			}
			i--;
			infix.push_back(new Variable(tmp));
		}
	}
	return infix;
}*/

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
		//if ()
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
		//cout << infix.size();// << ' ' << varmap.size();
		//cout << static_cast<Operator *>(infix[1])->getType();
		postfix = buildPostfix(infix);
		//cout << postfix.size();// << ' ' << varmap.size();
		value = evaluatePostfix(postfix);
		cout << value << endl;
	}
	return 0;
}