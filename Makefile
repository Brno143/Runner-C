TARGET = RunnerC

SRCDIR = src
OBJDIR = obj
INCDIR = include

CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC = $(wildcard $(SRCDIR)/*.c)

OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))


all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCDIR)


run: all
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)
	
.PHONY: all clean run