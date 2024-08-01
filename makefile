all:
	g++ lexer.cpp -o lexer

clean:
	rm lexer
	rm out
	rm out.s