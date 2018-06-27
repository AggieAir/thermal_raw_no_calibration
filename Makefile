CC=g++
CFLAGS=-c -Wall -std=c++11 -g
LDFLAGS=-L./ -licisdk -lusb-1.0

#SOURCES=test.cpp
SOURCES=ICI.cpp  main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
		EXECUTABLE=ICI

.PHONY: all clean

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
			$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

.cpp.o:
			$(CC) $(CFLAGS) $< -o $@

clean:
			rm -rf *.o $(EXECUTABLE)
