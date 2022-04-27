#include <interpreter.h>

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

	// for (int i = 0; i < postfixLines.size(); i++) {
	// 	print(postfixLines[i]);
	// 	cout << endl;
	// }


	 int row = 0;
	 while (0 <= row && row < postfixLines.size()) {
	 	if (postfixLines[row].size() != 0) {
	 		row = evaluatePostfix(postfixLines[row], row);
	 	}
	 	else 
	 		row++;
	}

	
	for (int i = 0; i < postfixLines.size(); i++) {
		for (int j = 0; j < postfixLines[i].size(); i++) {
			if (postfixLines[i][j]->getLexemType() == OP) {
				if (static_cast<Operator *>(postfixLines[i][j])->getType() == DEREF) {
					delete postfixLines[i][j];
				}
			}
		}
	}
	for (int i = 0; i < infixLines.size(); i++) {
		remove(infixLines[i]);
	}


	return 0;
}