TARGET = a.out
CC = gcc

CFLAGS = -pedantic -ansi -Wall -std=gnu99 -g -lm -lncurses

BUILD = build/

FILES = $(BUILD)main.o $(BUILD)log.o $(BUILD)screen.o\
	   	$(BUILD)game.o $(BUILD)list.o $(BUILD)animation.o

$(TARGET): $(FILES)
	$(CC) $^ -o $(TARGET) $(CFLAGS)

$(BUILD)%.o: %.c
	$(CC) $^ -c -o $@ $(CFLAGS)

