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
		int fd;
	public:
		Socket()
		{
			this->fd = socket(AF_INET, SOCK_STREAM,0);
		}
		~Socket()
		{
		}
};

class ServerSocket : Socket
{
	private:
		int client_sock_fd;
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
			int flag = bind(this->fd, (struct sockaddr*) &(addr), sizeof(addr));
			if(flag == -1)
				return false;
			return true;
		}
		
		bool sock_listen(int conn_num)
		{
			int flag = listen(this->fd, conn_num);
			if(flag == -1)
				return false;
			return true;
		}
		
		bool sock_accept()
		{
			struct sockaddr client_addr;
			socklen_t client_addr_size = sizeof(client_addr);
			this->client_sock_fd  = accept(this->fd, (struct sockaddr*)&client_addr, (socklen_t*) &client_addr_size);
			if(this->client_sock_fd == -1)
				return false;
			return true;
		}
		
		void sock_send(const char* filepath)
		{
			std::FILE* fp = std::fopen(filepath,"rb");
			std::vector<char> buf(BUF_SIZE);
			usleep(1000*1000*5);
			int len = 1;
			while(len > 0)
			{
				len = std::fread(&buf[0],1,buf.size(),fp);
				if(len == 0)
				{	
					break;
				}
					send(this->client_sock_fd,&buf[0],len,0);
			}
			fclose(fp);
		}	

		void serv_sock_close()
		{
			close(this->fd);
		}
		void accept_sock_close()
		{
			close(this->client_sock_fd);
		}
		~ServerSocket()
		{
			this->serv_sock_close();
			this->accept_sock_close();
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
			int flag = connect(this->fd, (struct sockaddr*)&addr,sizeof(addr));
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
				len = recv(this->fd, &buf[0], BUF_SIZE, 0);
				
				if(len == 0)
					break;
				fwrite(&buf[0], 1, len, fp);
			}
			fclose(fp);
		}
		void client_sock_close()
		{
			close(this->fd);
		}
		~ClientSocket()
		{
			this->client_sock_close();
		}
};









