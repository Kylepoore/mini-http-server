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
	${CC} ${BUILDDIR}/*.o -o ${BINDIR}/${EXEC}

${BUILDDIR}/server.o: server.c ${BUILDDIR}/dispatch.o dispatch.c
	${CC} -c server.c -o ${BUILDDIR}/server.o

${BUILDDIR}/dispatch.o: dispatch.c dispatch.h ${BUILDDIR}/GET.o GET.c ${BUILDDIR}/POST.o POST.c
	${CC} -c dispatch.c -o ${BUILDDIR}/dispatch.o

${BUILDDIR}/GET.o: GET.c
	${CC} -c GET.c -o ${BUILDDIR}/GET.o

${BUILDDIR}/POST.o: POST.c
	${CC} -c POST.c -o ${BUILDDIR}/POST.o

test:
	@echo you will have to do these from different terminals.
	@echo to test the server, run: make test-server

submit:
	svn commit -m "submitted for grade"

clean:
	-rm -rf ${BUILDDIR} ${BINDIR}
