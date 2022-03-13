#include <iostream>
#include <vector>
#include <string>
#include <stack>
<<<<<<< HEAD
=======
#include <map>
>>>>>>> dev-main

using std::vector;
using std::string;
using std::stack;
using std::cin;
using std::cout;
using std::endl;
<<<<<<< HEAD


enum LEXEM_TYPE {
	NUMBER, OP
=======
using std::map;
using std::pair;


enum LEXEM_TYPE {
	NUMBER, OP, VAR
>>>>>>> dev-main
};

enum OPERATOR {
	LBRACKET, RBRACKET,
	PLUS, MINUS,
<<<<<<< HEAD
	MULTIPLY
=======
	MULTIPLY, ASSIGN
>>>>>>> dev-main
};

int PRIORITY[] = {
	-1, -1,
<<<<<<< HEAD
	0, 0,
	1
=======
	1, 1,
	2, 0
>>>>>>> dev-main
};


class Lexem {
	LEXEM_TYPE lexem_type;
public:
	Lexem() {}
	LEXEM_TYPE getLexemType();
<<<<<<< HEAD
=======
	void setLexemType(LEXEM_TYPE type);
>>>>>>> dev-main
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

<<<<<<< HEAD
=======
class Variable: public Lexem {
	string name;
	//int value;
public:
	Variable(const string & name);
	int getValue();
	void setValue(int value);
};

map<string, Variable *> varmap;
>>>>>>> dev-main

LEXEM_TYPE Lexem::getLexemType() {
	return lexem_type;
}


<<<<<<< HEAD
=======
void Lexem::setLexemType(LEXEM_TYPE type) {
	lexem_type = type;
}

/////////////////////// class Variable methods ///////////////////////


Variable::Variable(const string & name) {
	this->name = name;
	varmap.insert(pair<string, Variable *>(name, this));
	Lexem::setLexemType(VAR);
}


>>>>>>> dev-main
////////////////////// class Number methods ////////////////////////////



Number::Number() : value(0) {
}

Number::Number(int someValue) {
	value = someValue;
<<<<<<< HEAD
=======
	Lexem::setLexemType(NUMBER);
>>>>>>> dev-main
}

int Number::getValue() {
	return value;
}


////////////////////// class Operator methods ///////////////////////////



Operator::Operator(OPERATOR status) {
	opertype = status;
<<<<<<< HEAD
=======
	Lexem::setLexemType(OP);
>>>>>>> dev-main
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



<<<<<<< HEAD
=======


>>>>>>> dev-main
bool isDigit(char ch) {
	if (ch >= '0' && ch <= '9') 
		return true;
	return false;
}

<<<<<<< HEAD
=======
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
>>>>>>> dev-main

vector<Lexem *> parseLexem(string codeline) {
	vector<Lexem *> infix;
	for (int i = 0; i < codeline.size(); i++) {
<<<<<<< HEAD
=======
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
>>>>>>> dev-main
		if (codeline[i] == ' ' || codeline[i] == '\t')
			continue;
		if (codeline[i] == '+') {
			infix.push_back(new Operator(PLUS));
<<<<<<< HEAD
			infix[infix.size() - 1]->lexem_type = OP;
=======
>>>>>>> dev-main
			continue;
		}
		if (codeline[i] == '-') {
			infix.push_back(new Operator(MINUS));
<<<<<<< HEAD
			infix[infix.size() - 1]->lexem_type = OP;
=======
>>>>>>> dev-main
			continue;
		}
		if (codeline[i] == '*') {
			infix.push_back(new Operator(MULTIPLY));
<<<<<<< HEAD
			infix[infix.size() - 1]->lexem_type = OP;
=======
>>>>>>> dev-main
			continue;
		}
		if (codeline[i] == '(') {
			infix.push_back(new Operator(LBRACKET));
<<<<<<< HEAD
			infix[infix.size() - 1]->lexem_type = OP;
=======
>>>>>>> dev-main
			continue;
		}
		if (codeline[i] == ')') {
			infix.push_back(new Operator(RBRACKET));
<<<<<<< HEAD
			infix[infix.size() - 1]->lexem_type = OP;
=======
			continue;
		}
		if (codeline[i] == '=') {
			infix.push_back(new Operator(ASSIGN));
>>>>>>> dev-main
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
<<<<<<< HEAD
			infix[infix.size() - 1]->lexem_type = NUMBER;
			continue;
		}
	}
	return infix;
=======
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
>>>>>>> dev-main
}

vector<Lexem *> buildPostfix(vector<Lexem *> infix) {
	vector<Lexem *> postfix;
	stack <Lexem *> stack;
	int j = 0;
	for (int i = 0; i < infix.size(); i++) {
<<<<<<< HEAD
		if ((infix[i]->getLexemType()) == NUMBER) {
			postfix.push_back(infix[i]);
		}
=======
		if ((infix[i]->getLexemType()) == NUMBER || (infix[i]->getLexemType()) == VAR) {
			postfix.push_back(infix[i]);
		}

>>>>>>> dev-main
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
<<<<<<< HEAD
				while (static_cast<Operator *>(tmp)->getPriority() > 
									static_cast<Operator *>(infix[i])->getPriority()) {
					postfix.push_back(tmp);
					stack.pop();
					tmp = stack.top();
=======
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
>>>>>>> dev-main
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
<<<<<<< HEAD
=======
		//if ()
>>>>>>> dev-main
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
<<<<<<< HEAD
		postfix = buildPostfix(infix);
=======
		//cout << infix.size();// << ' ' << varmap.size();
		//cout << static_cast<Operator *>(infix[1])->getType();
		postfix = buildPostfix(infix);
		//cout << postfix.size();// << ' ' << varmap.size();
>>>>>>> dev-main
		value = evaluatePostfix(postfix);
		cout << value << endl;
	}
	return 0;
}