
TARGET = simulador


CC = gcc
CFLAGS = -Wall -Wextra -g -I.

# Lista de arquivos fonte 
SRCS = main.c \
       simulador/simulador.c \
       escalonador/escalonador.c \
       processo/processo.c \
       fila/fila.c \
       io/io.c


OBJS = $(SRCS:.c=.o)

#executada quando você digita apenas 'make'
all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)


run: $(TARGET)
	./$(TARGET) 5 3