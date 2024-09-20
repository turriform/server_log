CC:= gcc

SRC:= $(wildcard ./src/*.c)

SRC+= $(wildcard ./src/hash_table/*.c)

TARGET:= ./bin/prog

INCLUDE:= -I./include -I./include/hash_table

FLAGS:= -std=c11  -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs -g 

#LIBS:= -lz

all:
	$(CC) $(SRC) $(FLAGS) $(INCLUDE) -o $(TARGET) $(LIBS) && $(TARGET)