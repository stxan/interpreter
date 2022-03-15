all:
	g++ source/interpreter.cpp -o bin/interp -fsanitize=leak,address
