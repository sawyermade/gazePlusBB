CC=g++
CFLAGS=-g -Wall -std=c++11 `pkg-config --cflags opencv`
INC=
LIB=`pkg-config --libs opencv`
BIN=
SRC=main.cpp
OBJ=$(SRC:.cpp=.o)
EXEC=gazebb
LDFLAGS=


all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CLFAGS) $(INC) -o $@ $^ $(LIB)
	
.cpp.o:
	$(CC) $(CFLAGS) $(INC) -c -o $@ $^

clean:
	rm -f $(OBJ) $(EXEC)