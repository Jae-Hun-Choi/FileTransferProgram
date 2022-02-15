CC = g++

S_SRCS = src/server/*.cpp
C_SRCS = src/client/*.cpp							 

INC_DIRS = -I include

CFLAGS = -o
TFLAGS = -lpthread

all : server client

server : $(S_SRCS)
	$(CC) $(CFLAGS) $@ $< $(INC_DIRS) $(TFLAGS)

client : $(C_SRCS)
	$(CC) $(CFLAGS) $@ $< $(INC_DIRS)


clean : 
	rm ./server ./client ./recvfile.jpg
