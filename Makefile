SRC=src
INC=include
BIN=bin
OBJ=objects

CC=gcc
CFLAGS=-I$(INC) -O0 -ggdb -Wall -pedantic

OBJECTS= \
$(OBJ)/enigma.o \
$(OBJ)/getopt.o \
$(OBJ)/keyboard.o \
$(OBJ)/main.o \
$(OBJ)/plugboard.o \
$(OBJ)/reflector.o \
$(OBJ)/rotors.o \
$(OBJ)/util.o

.PHONY: clean

$(BIN)/ligma: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJ)/*
	rm -f $(BIN)/*
