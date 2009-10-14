CC=g++
CFLAGS=-c -Wall
SOURCES=cmds.cpp sosh.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=app

all: clean $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
