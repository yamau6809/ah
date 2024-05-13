# Makefile for ah

TARGET = ah
DESTINATION = ${HOME}/bin

all: ${TARGET}

${TARGET}: ah.c
	gcc -o $@ $<

clean:
	rm -f *~ *.out ah *.o

install: ${TARGET}
	install ${TARGET} ${DESTINATION}
