# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++17

# Source files
PROTO_SRCS = Message.proto
SRCS = src/ManegerAndHider/Connection.cpp src/ManegerAndHider/Listener.cpp src/ManegerAndHider/HiddenFileHandler.cpp src/ManegerAndHider/HiderManeger.cpp src/ManegerAndHider/main.cpp src/ManegerAndHider/SoftwareManeger.cpp src/ManegerAndHider/command.cpp src/ManegerAndHider/responce.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = maneger

# Rule to compile source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Build target
all: $(EXEC)

$(EXEC): $(OBJS) $(PROTO_OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(PROTO_OBJS) -o $@ -lprotobuf

# Clean rule
clean:
	rm -f $(OBJS) $(PROTO_OBJS) $(EXEC)
