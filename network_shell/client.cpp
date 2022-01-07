#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include "client.hpp"

Client::Client() {}

Client::Client(int port, char* server_ip) {
	this->port = port;
	this->server_ip = server_ip;
}

void Client::CreateSocket() {
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket < 0) {
		std::cout << "Creation of client socket failed\n";
		exit(EXIT_FAILURE);
	}
}  

void Client::ConnectToServer() {

	struct sockaddr_in server_addr, client_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	server_addr.sin_port = htons(port);
	
	if (connect(client_socket,  (struct sockaddr*) & server_addr, sizeof(server_addr)) < 0) {
		std::cout << "Connection Error..!\n";
		exit(EXIT_FAILURE);
	} else { 
		std::cout << "Connection Established..!\n";
	}	
}


void Client::Communicate() {
	
	const int MAX = 500;
	
	while (true) {
		std::string s;
		char input[MAX];
		std::cout << "\ncmd>";
		std::getline(std::cin, s);
		int n = s.size();
		for (int i = 0; i < n; i++) {
			input[i] = s[i];
		}
		
		input[n] = '\0';
		
		send(client_socket, input, strlen(input)+1 , 0); 	
	
		char received_message[MAX];
	
		int msg_size = recv(client_socket, received_message, MAX, 0);
		
		if (msg_size < 0) {
			std::cout << "Packet recieve failed.\n";
			exit(EXIT_FAILURE);
		}
		else if (msg_size == 0) {
			std::cout << "Server is off.\n";
			exit(EXIT_FAILURE);
		}
		
		if (received_message[0] == 'q' && received_message[1] == 'u' && received_message[2] == 'i' && received_message[3] == 't') {
			std::cout << "\nConnection ended..!\n\n)";
			break;
		}

		std::cout << received_message << "\n";
	}

}

Client :: ~Client() {
	close(client_socket);
}

int main() {
	Client *client = new Client(9000 , (char *)"127.0.0.1"); 
	client->CreateSocket();
	client->ConnectToServer();
	client->Communicate();
	return 0;
}