CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lpthread
SOURCES=history.c aux.c cmds.c sosh.c freq.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sosh freq

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	
rebuild: clean all
