CC=gcc
CFLAGS=-Wall
LDFLAGS=-lpthread
SOURCES=history.c aux.c cmds.c
OBJECTS=$(SOURCES:.c=.o)

all: clean sosh freq

sosh: $(OBJECTS) sosh.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) sosh.c -o sosh

freq: $(OBJECTS) freq.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) freq.c -o freq

.c.o:
	$(CC) -c $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) *.o sosh freq

rebuild: clean all

