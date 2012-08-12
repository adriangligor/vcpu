SRC=./src
OUT=./out

# you can use gcc as well
CC=clang
CFLAGS=-I$(SRC)

LIBS=-lm

_DEPS = cpu.h parser.h
DEPS = $(patsubst %,$(SRC)/%,$(_DEPS))

_OBJ = shell.o cpu.o parser.o
OBJ = $(patsubst %,$(OUT)/%,$(_OBJ))


$(OUT)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT)/shell: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: $(OUT)/shell


.PHONY: clean

clean:
	rm -rf $(OUT)/*
