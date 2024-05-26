# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++17

# Source files
PROTO_SRCS = Message.proto
SRCS = src/ManegerAndHider/client.cpp src/ManegerAndHider/HiddenFileHandler.cpp src/ManegerAndHider/Hider.cpp src/ManegerAndHider/HiderManeger.cpp src/ManegerAndHider/main.cpp src/ManegerAndHider/SoftwareManeger.cpp

# Object files
PROTO_OBJS = $(PROTO_SRCS:.proto=.pb.o)
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = cyber

#PROTOBUF_LIB = -lprotobuf

# Rule to compile Protocol Buffers source files into object files
%.pb.o: %.proto
	protoc --cpp_out=. $<
	$(CC) $(CFLAGS) -c $(<:.proto=.pb.cc) -o $@

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
