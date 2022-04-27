#ifndef INTERP_HPP
#define INTERP_HPP

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
	NUMBER, OP, VAR, ARR
};

enum OPERATOR {
	PRINT,
	IF, THEN,
	ELSE, ENDIF,
	WHILE, ENDWHILE,
	GOTO, ASSIGN, COLON,
	ARRAY, DEREF,
	LBRACKET, RBRACKET,
	LQBRACKET, RQBRACKET,
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



class Lexem {
	LEXEM_TYPE lexem_type;
public:
	Lexem() {}
	virtual ~Lexem() {}
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
class ArrayElem;

class Operator: public Lexem {
	OPERATOR opertype;
public:
	Operator();
	Operator(OPERATOR status);
	int evaluateNum(int a, int b);
	int evaluateVar(Variable *var, int b);
	int evaluateArr(ArrayElem *array, int val);
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


class ArrayElem: public Lexem {
	string name;
	int index;
public:
	void initArray(string name, int capacity);
	string getName();
	int getIndex();
	int getValue();
	void setValue(int val);
	ArrayElem(Variable *array, int capacity);
	// ArrayElem(OPERATOR optype) : Operator(optype) {}
};

class Dereference: public Operator {
public:
	Dereference(OPERATOR optype): Operator(optype) {}
	ArrayElem *getValue(Variable *array, int index) {
		return new ArrayElem(array, index);
	}

};

bool isDigit(char ch);

bool isLetter(char ch);

void initLabels(vector<Lexem *> &infix, int row);
void initJumps(vector< vector <Lexem *> > infixes);
Operator *check_operator(string codeline, int *i);
Number *check_number(string codeline, int *i);
Variable *check_var(string codeline, int *i);
vector<Lexem *> parseLexem(string codeline);
bool isRightAssociated(OPERATOR opType);
void binaryOperBuildPostfix(stack<Lexem *> & stack, vector<Lexem *> & infix,
							 vector<Lexem *> & postfix, OPERATOR operType, int i);
void OperBuildPosfix(stack<Lexem *> & stack, vector<Lexem *> & infix,
						 vector<Lexem *> & postfix, int i);
vector<Lexem *> buildPostfix(vector<Lexem *> infix);
int evaluatePostfix(vector<Lexem *> postfix, int row);
void print(vector<Lexem *> vec);
void remove(vector<Lexem *> infix);

#endif