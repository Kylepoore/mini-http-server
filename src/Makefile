#
# Kyle Poore
# March 3, 2014

CC = gcc -O3
LOCALHOST = localhost
FILENAME = test.txt
BUILDDIR = ../build
BINDIR = ../bin
EXEC = server

all: directories ${EXEC}

directories:
	mkdir -p ${BUILDDIR} ${BINDIR}

${EXEC}: server.c ${BUILDDIR}/server.o
	${CC} ${BUILDDIR}/server.o -o ${BINDIR}/${EXEC}

${BUILDDIR}/server.o: server.c
	${CC} -c server.c -o ${BUILDDIR}/server.o

test: test-file
	@echo you will have to do these from different terminals.
	@echo to test the server, run: make test-server
	@echo to test the client in write mode, in a different terminal run: make test-client-write
	@echo to test the client in read mode, in a different terminal run: make test-client-read

submit:
	svn commit -m "submitted for grade"

clean:
	-rm -rf ${BUILDDIR} ${BINDIR}
