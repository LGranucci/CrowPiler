all:
	g++ -g compiler.cpp  compiler.h -o lexer

clean:
	rm lexer
	rm out
	rm out.s