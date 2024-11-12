all:
	g++ -g parser.cpp main.cpp lexer.cpp write.cpp -o lexer

clean:
	rm lexer
	rm out
	rm out.s