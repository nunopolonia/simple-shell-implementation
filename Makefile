CC=gcc
CFLAGS=-c -Wall
SOURCES=cmds.c sosh.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sosh

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	
rebuild: clean all
