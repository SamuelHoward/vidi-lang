main: main.o lexer.o parser.o interpreter.o
	g++ interpreter.o parser.o lexer.o main.o -o vidic

main.o: main.cc lexer.h parser.h interpreter.h
	g++ -c main.cc

lexer.o: lexer.cc lexer.h
	g++ -c lexer.cc

parser.o: parser.cc parser.h
	g++ -c parser.cc

interpreter.o: interpreter.cc interpreter.h
	g++ -c interpreter.cc

clean:
	rm *.o *~ Examples/*~ Doc/*~
