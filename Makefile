CC=gcc
CFLAGS=-c -Wall
LIBS=-lreadline
SOURCES=aux.c cmds.c sosh.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sosh

all: clean $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	
rebuild: clean all
