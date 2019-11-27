# OS specifics
ifeq ($(shell uname -o),Msys)
	EXECUTABLE = turing.exe
else
	EXECUTABLE = turing
endif

.PHONY : all binary install test_all test run clean tar

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

test_all:
	@for case in $(shell ls programs); do \
		[ $$case = "syntax.tm" ] && continue; \
		echo "./$(EXECUTABLE) programs/$$case -b -W"; \
		./$(EXECUTABLE) programs/$$case -b -W; \
	done

test:
	./$(EXECUTABLE) ${CASE} -v -W

run:
	./$(EXECUTABLE) ${CASE} -s -w

tar:
	python tar.py

clean:
	cd src && rm -f *.o $(EXECUTABLE)
	rm -f $(EXECUTABLE)
	rm -f *-*.zip
