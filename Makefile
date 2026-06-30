# Nome do programa final
TARGET = simulador

# Compilador e flags de compilação (-Wall e -Wextra ajudam a achar erros bobos, -g permite usar o gdb para debugar)
CC = gcc
CFLAGS = -Wall -Wextra -g

# Lista de arquivos fonte (.c) baseada na modularização
SRCS = main.c pcb.c filas.c escalonador.c

# Converte a lista de arquivos .c em .o (arquivos objeto)
OBJS = $(SRCS:.c=.o)

# Regra principal: executada quando você digita apenas 'make'
all: $(TARGET)

# Linkagem: junta todos os .o para criar o executável final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilação: ensina o make a transformar qualquer .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Comando para limpar a sujeira da compilação (arquivos .o e o executável)
clean:
	rm -f $(OBJS) $(TARGET)

# Comando para compilar e rodar o simulador automaticamente
run: $(TARGET)
	./$(TARGET)