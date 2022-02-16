//library
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstring>
#include "Socket.h"

// Function Declaration
void ErrorHandler(const char* message);

// Code
int main(int argc, char* argv[])
{
	if( argc!= 3)
	{
		std:: cout << "Usage : " << argv[0] << " <ip> <port>" << std::endl;
		ErrorHandler("arguments error\n");
	}
	
	ClientSocket* sock = new ClientSocket();

	//bind()
	if(!(sock->sock_connect(argv[1], argv[2])))
		ErrorHandler("connect() Error\n");

	sock->sock_recv("./recvfile.jpg");
	sock->client_sock_close();

	delete(sock);
}

void ErrorHandler(const char* message)
{
	std::fputs(message,stderr);
	std::fputc('\n',stderr);
	exit(1);
}













	
