// library
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>
#include <cstring>

#include "Socket.h"


// Function Declaration
void ErrorHandler(const char* message);

// Code
int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		std::cout << "Usage : " << argv[0] << " <port>" << std::endl;
		ErrorHandler("arguments error\n");
	}
	//std::unique_ptr<ServerSocket*> sock(new ServerSocket(argv[1],5));
	ServerSocket* sock = new ServerSocket();
	
	//bind()
	if(!(sock->sock_bind(argv[1])))
		ErrorHandler("bind() error\n");

	// listen
	if(!(sock->sock_listen(5)))
		ErrorHandler("listen() error\n");

	std::vector<std::thread> threads;

	for(uint8_t i = 0; i < 5; i++)
	{
		threads.push_back(std::thread([](ServerSocket* sock) {
			sock->sock_sendfile("./img/sendfile.jpg");
		}, sock));
	}

	for(auto &thread : threads)
	{
		if(thread.joinable() == true)
			thread.join();
	}
	return 0;
}

void ErrorHandler(const char* message)
{
	std::fputs(message,stderr);
	std::fputc('\n',stderr);
	exit(1);
}
