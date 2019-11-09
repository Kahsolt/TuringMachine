all: 
	cd src \
	  && g++ -Wall Exception.cpp Tokenizer.cpp Parser.cpp TuringMachine.cpp main.cpp -o ../turing.exe

binary:
	cd src \
	  && g++ -Wall -c Exception.cpp -o Exception.o \
	  && g++ -Wall -c Tokenizer.cpp -o Tokenizer.o \
	  && g++ -Wall -c Parser.cpp -o Parser.o \
	  && g++ -Wall -c TuringMachine.cpp -o TuringMachine.o \
	  && g++ -Wall Exception.o Tokenizer.o Parser.o TuringMachine.o main.cpp -o turing.exe

install:
	cp src/turing.exe .

run:
	./turing.exe ${CASE}

clean:
	cd src && rm -f *.o *.exe
