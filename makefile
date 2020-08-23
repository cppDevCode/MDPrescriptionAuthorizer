# change application name here (executable output name)
TARGET=ADME2
 
# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall
 
PTHREAD=-pthread
 
CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe
 
GTKLIB=`pkg-config --cflags --libs gtk+-3.0 gthread-2.0`
 
# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic
 
OBJS=    main.o nuevoafiliado.o opciones.o
 
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)

nuevoafiliado.o: src/nuevoafiliado.c
	$(CC) -c $(CCFLAGS) src/nuevoafiliado.c $(GTKLIB) -o nuevoafiliado.o

opciones.o: src/opciones.c
	$(CC) -c $(CCFLAGS) src/opciones.c $(GTKLIB) -o opciones.o

main.o: src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(GTKLIB) -o main.o
clean:
	rm -f *.o $(TARGET)