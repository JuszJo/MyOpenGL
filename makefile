CC = g++
CFLAGS = -Wall -Wextra
LDFLAGS = -lglfw3 -lopengl32 -lgdi32 -lglew32
INC_DIRS = -I./include
LIB_DIRS = -L./

SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = myprogram

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(INC_DIRS) $(LIB_DIRS) -o $@ $(OBJECTS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)