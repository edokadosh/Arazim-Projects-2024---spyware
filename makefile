# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra

# Source files
SRCS = client.cpp HiddenFileHandler.cpp Hider.cpp HiderManeger.cpp main.cpp Message.pb.cc SoftwareManeger.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = cyber

# Build target
all: $(EXEC)

$(EXEC): $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# Rule to compile source files
%.o: %.cpp
    $(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
    rm -f $(OBJS) $(EXEC)