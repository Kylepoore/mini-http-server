#
# Kyle Poore
# March 3, 2014

CC = gcc -O3
BUILDDIR = ../build
BINDIR = ../bin
MODDIR = modules
MODULES = GET.c POST.c
FRAMEWORK = dispatch.c server.c process_request.c request.c helpers.c
MODULE_OBJS = $(MODULES:%.c=$(BUILDDIR)/$(MODDIR)/%.o)
FRAMEWORK_OBJS = $(FRAMEWORK:%.c=$(BUILDDIR)/%.o)
EXEC = server

all: directories ${EXEC}

directories:
	mkdir -p ${BUILDDIR}/${MODDIR} ${BINDIR}

${EXEC}: ${FRAMEWORK_OBJS} ${MODULE_OBJS}
	${CC} ${FRAMEWORK_OBJS} ${MODULE_OBJS} -o ${BINDIR}/${EXEC}

${BUILDDIR}/server.o: ${FRAMEWORK} ${BUILDDIR}/dispatch.o \
	                    ${BUILDDIR}/process_request.o \
	                    ${BUILDDIR}/request.o \
	                    ${BUILDDIR}/helpers.o
	${CC} -c server.c -o ${BUILDDIR}/server.o

${BUILDDIR}/dispatch.o: dispatch.c dispatch.h ${MODULE_OBJS} 
	${CC} -c dispatch.c -o ${BUILDDIR}/dispatch.o

${BUILDDIR}/process_request.o: process_request.c process_request.h
	${CC} -c process_request.c -o ${BUILDDIR}/process_request.o

${BUILDDIR}/request.o: request.c request.h 
	${CC} -c request.c -o ${BUILDDIR}/request.o

${BUILDDIR}/helpers.o: helpers.c helpers.h
	${CC} -c helpers.c -o ${BUILDDIR}/helpers.o

${BUILDDIR}/${MODDIR}/%.o: ${MODDIR}/%.c ${MODDIR}/%.h
	${CC} -c $< -o $@

submit:
	svn commit -m "submitted for grade"

clean:
	-rm -rf ${BUILDDIR} ${BINDIR}
