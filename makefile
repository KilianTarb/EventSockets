CC = g++

INCLUDEFLAGS = -Iinclude
OUTFLAGS = -o build/
FLAGS = -std=c++17

all: 
	${CC} -c lib/EventSocket.cpp ${INCLUDEFLAGS} ${FLAGS} ${OUTFLAGS}EventSocket.o
	${CC} -c main.cpp ${INCLUDEFLAGS} ${FLAGS} ${OUTFLAGS}main.o
	${CC} build/*  ${SSLFLAGS} ${FLAGS} ${OUTFLAGS}event_socket

clean:
	rm build/*