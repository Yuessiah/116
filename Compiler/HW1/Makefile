CC=gcc
FNAME=compiler_hw1
EXE=myscanner
TEST=operators.go

ifeq ($(OS),Windows_NT)
    OSTYPE = windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OSTYPE = linux
    endif
endif

compile: scanner
	@${CC} lex.yy.c -o ${EXE}

scanner: 
	@lex ${FNAME}.l

clean:
	@rm ${EXE} lex.*
