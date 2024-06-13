# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++17

# Source files
M_SRCS = src/Maneger/Connection.cpp src/Maneger/Listener.cpp src/Maneger/HiderManeger.cpp src/Maneger/main.cpp src/Maneger/SoftwareManeger.cpp src/IncludeCPP/encoding.cpp
H_SRCS = src/Hider/Hider.cpp src/Hider/HiddenFileHandler.cpp src/IncludeCPP/encoding.cpp

# Object files
M_OBJS = $(M_SRCS:.cpp=.o)
H_OBJS = $(H_SRCS:.cpp=.o)

# Executables names
M_EXEC = maneger
H_EXEC = hider

# Rule to compile source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

all: maneger hider

# Build target
maneger: $(M_EXEC)

$(M_EXEC): $(M_OBJS)
	$(CC) $(CFLAGS) $(M_OBJS) -o $@

hider: $(H_EXEC)

$(H_EXEC): $(H_OBJS)
	$(CC) $(CFLAGS) $(H_OBJS) -o $@

# Clean rule
clean:
	rm -f $(M_OBJS) $(H_OBJS) $(M_EXEC) $(H_EXEC) 
