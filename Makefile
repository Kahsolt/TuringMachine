EXECUTABLE = turing.exe

all: 
	cd src \
	  && g++ -Wall Exception.cpp Tokenizer.cpp Parser.cpp StringPool.cpp TuringMachine.cpp main.cpp -o ../$(EXECUTABLE)

binary:
	cd src \
	  && g++ -Wall -c Exception.cpp -o Exception.o \
	  && g++ -Wall -c Tokenizer.cpp -o Tokenizer.o \
	  && g++ -Wall -c Parser.cpp -o Parser.o \
	  && g++ -Wall -c StringPool.cpp -o StringPool.o \
	  && g++ -Wall -c TuringMachine.cpp -o TuringMachine.o \
	  && g++ -Wall Exception.o Tokenizer.o Parser.o StringPool.o TuringMachine.o main.cpp -o $(EXECUTABLE)

install:
	cp src/$(EXECUTABLE) .

test:
	./$(EXECUTABLE) ${CASE} -v -W

run:
	./$(EXECUTABLE) ${CASE} -s -w

clean:
	cd src && rm -f *.o *.exe
	rm -f *-*.zip

tar:
	python tar.py