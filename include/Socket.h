// system call
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// library
#include <cstring>
#include <thread>
#include <vector>

// macro
#define BUF_SIZE 1024

class Socket 
{
	protected:
		int id;
	public:
		Socket()
		{
			this->id = socket(AF_INET, SOCK_STREAM,0);
		}
		~Socket()
		{
			close(this->id);
		}
};

class ServerSocket : Socket
{
	private:
		int client_sock;
	public:
		ServerSocket()
		{
		}
		bool sock_bind(char* port)
		{
			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
			addr.sin_port = htons(atoi(port));
			int flag = bind(this->id, (struct sockaddr*) &(addr), sizeof(addr));
			if(flag == -1)
				return false;
			return true;
		}
		
		bool sock_listen(int conn_num)
		{
			int flag = listen(this->id, conn_num);
			if(flag == -1)
				return false;
			return true;
		}
		
		bool sock_accept()
		{
			struct sockaddr client_addr;
			socklen_t client_addr_size = sizeof(client_addr);
			this->client_sock  = accept(this->id, (struct sockaddr*)&client_addr, (socklen_t*) &client_addr_size);
			if(this->client_sock == -1)
				return false;
			return true;
		}
		
		void sock_send(const char* filepath)
		{
			std::FILE* fp = std::fopen(filepath,"rb");
			std::vector<char> buf(BUF_SIZE);
			
			int len = 1;
			while(len > 0)
			{
				len = std::fread(&buf[0],1,buf.size(),fp);
				if(len == 0)
				{	
					break;
				}
					send(this->client_sock,&buf[0],len,0);
			}
			fclose(fp);
		}
		bool sock_sendfile(const char *filepath)
		{
			while(true)
			{
				if(!(this->sock_accept()))
					return false;
				this->sock_send(filepath);
				close(this->client_sock);
			}
		}
		~ServerSocket()
		{
			close(this->client_sock);
		}

};

class ClientSocket : Socket 
{
	public :
		ClientSocket() 
		{
		}
		bool sock_connect(char* ip, char* port)
		{
			struct sockaddr_in addr;
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr(ip);
			addr.sin_port = htons(atoi(port));
			int flag = connect(id, (struct sockaddr*)&addr,sizeof(addr));
			if(flag == -1) 
				return false;
			return true;
		}
		void sock_recv(const char* filepath)
		{
			std::FILE* fp = std::fopen(filepath, "wb");
			std::vector<char> buf(BUF_SIZE);
		
			int len = 1;
			while(len > 0)
			{
				len = recv(id, &buf[0], BUF_SIZE, 0);
				
				if(len == 0)
					break;
				fwrite(&buf[0], 1, len, fp);
			}
			fclose(fp);
		}
		void sock_recvfile(const char* filepath)
		{
			this->sock_recv(filepath);
		}
		~ClientSocket()
		{
		}
};









