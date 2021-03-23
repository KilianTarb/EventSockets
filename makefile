CC = g++

INCLUDEFLAGS = -Iinclude
GTESTINCLUDE = -lgtest -lgtest_main -lpthread
OUTFLAGS = -o build/
FLAGS = -std=c++17

all: 
	${CC} -c lib/EventSocket.cpp ${INCLUDEFLAGS} ${FLAGS} ${OUTFLAGS}EventSocket.o
	${CC} -c tests/EventSocketTests.cpp ${INCLUDEFLAGS} ${FLAGS} ${OUTFLAGS}EventSocketTests.o
	${CC} build/* ${GTESTINCLUDE} ${SSLFLAGS} ${FLAGS} ${OUTFLAGS}EventSocketTests

clean:
	rm build/*
