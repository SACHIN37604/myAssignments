#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_


class Client {
public:
  
  	Client();
	
	Client(int port, char * server_ip);
	/**
	 * @brief creates a socket
	 * 
	 */ 
	void CreateSocket();
	
	/**
	 * @brief Connect to server using TCP
	 * 
	 */ 
	void ConnectToServer();
	
	/**
	 * @brief Maintains communication with server 
	 * 
	 */ 
	void Communicate();
	
	~Client();

private:
	// client socket descriptor
  	int client_socket;
    
	int port ;
	
	char * server_ip;
};

#endif