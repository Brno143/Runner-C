# Nome do executável final
TARGET = RunnerC

# Diretórios
SRCDIR = src
OBJDIR = obj
INCDIR = include

# Compilador e Flags
CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Encontra todos os arquivos .c no diretório src
SRC = $(wildcard $(SRCDIR)/*.c)

# Cria a lista de arquivos objeto (.o) no diretório obj
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

# -----------------------------------------------------------------------------
# REGRA PRINCIPAL: Faz tudo (compila)
# -----------------------------------------------------------------------------
all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Linkagem: Cria o executável a partir dos objetos
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compilação: Regra para criar arquivos objeto (.o)
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCDIR)

# -----------------------------------------------------------------------------
# AÇÕES AUXILIARES
# -----------------------------------------------------------------------------

# Executa o jogo
run: all
	./$(TARGET)

# Limpa todos os arquivos gerados (objetos e executável)
clean:
	rm -rf $(OBJDIR) $(TARGET)
	
.PHONY: all clean run