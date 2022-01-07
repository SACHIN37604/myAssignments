#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <stdlib.h> 
#include <sys/wait.h>
#include <signal.h>

#include "server.hpp"

std::atomic<int> child_pid;
std::atomic<int> children;

std::string Server::ExecuteCommand (const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}



void Server::SigQuitHandler(int signal_number) {
    kill(children, SIGTERM);
    while ((child_pid = wait(nullptr)) > 0);
    _exit(103);
}

std::string Server::SpawnChild(const char * cmd) {
    int fd[2], i = 0;
	pipe(fd);
	pid_t pid = fork();
  
	if (pid > 0) {
      	wait(NULL);
      	close(0);
      	close(fd[1]); 
      	dup(fd[0]); 
		int MAX = 500;
		char buff[MAX]; 
		int n = read(fd[0], buff, sizeof(buff));
		std::string result = "";
		for ( i = 0;i < n; i++)
			result += buff[i];
		return result; 
  } 
  else if (pid == 0) {
		std::string result = ExecuteCommand(cmd);
		int n = result.size();
		char buff[n];
		for(int i = 0; i < n; i++){
			buff[i] = result[i];
		}
      	close(fd[0]); 
  		close(1);    
  		dup(fd[1]);  
      	write(1, buff , sizeof(buff));
  } 
  else {
      perror("error\n");
  }
  exit(EXIT_SUCCESS);
}

Server::Server(){}

Server::Server(int port , char * server_ip) {
	this->port = port;
	this->server_ip = server_ip;
}

void Server::CreateSocket() {
	server_socket_handler = socket(AF_INET , SOCK_STREAM , 0); 
	if (server_socket_handler < 0) {	
		std::cout << "Socket creation has failed.";
		exit(EXIT_FAILURE);
	}
}

void Server::BindSocket() {
	struct sockaddr_in server_addr;
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	
	bind(server_socket_handler, (struct sockaddr*) & server_addr, sizeof(server_addr));
}

void Server::ListenOnPort() {

	int listen_status = listen(server_socket_handler , 5);
	
	if (listen_status < 0) {	
		std::cout << "Listner has failed\n" ;
		exit(EXIT_FAILURE);
	}
	
	std::cout << "...Waiting for connections \n" ;
}
 
void Server::Communicate(){
	int MAX = 9000;
	char buff[MAX];
	while(true){
		
		int msg_size;
		if ((msg_size  = recv(connection, buff, MAX, 0)) > 0) {
			if(buff[0] == 'q' && buff[1] == 'u' && buff[2] == 'i' && buff[3] == 't'){
				std::cout << "\nConnection ended..!\n" ;
				break;
			}
			char input[MAX];
			std::string s;
			signal(SIGQUIT, Server::SigQuitHandler);
			s = SpawnChild(buff);
			while ((child_pid = wait(nullptr)) > 0)
				std::cout << "child " << child_pid << " terminated\n" ;

			int n = s.size();
			for (int i = 0; i < n; i++) {
				input[i] = s[i];
			}
			input[n] = '\0';
			send(connection , input , strlen(input)+1 , 0);
		}
	}
}

void Server::AcceptRequest(){

	int client_socket_handler;
	
	struct sockaddr_in client_addr;

	socklen_t len = sizeof(client_addr);
	
	if ((connection = accept(server_socket_handler, (struct sockaddr*) & client_addr , &len)) < 0) {
		std::cout << "Server didn't accept the request. \n\n";
		exit(EXIT_FAILURE);
	} else {
		std::cout << "Server accepted the request. \n\n";
	}
}

Server :: ~Server(){
	close(server_socket_handler);
}

int main() {
	Server *server = new Server(9000 , (char *)"127.0.0.1");
	server->CreateSocket();
	server->BindSocket();
	server->ListenOnPort();
	server->AcceptRequest();
	server->Communicate();
	return 0;
}