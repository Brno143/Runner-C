TARGET = meu_jogo
SRC = main.c
CFLAGS = -Wall -std=c99
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt
CC = gcc
all: $(TARGET)
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)
clean:
	rm -f $(TARGET)