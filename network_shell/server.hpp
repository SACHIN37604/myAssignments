#ifndef _SERVER_HPP_
#define _SERVER_HPP_

class Server {
public:
  	Server();
	Server(int, char * );
	
	/**
	 * @brief creates server socket
	 * 
	 */ 
	void CreateSocket();

	/**
	 * @brief  Binds the socket with port number and server ip
	 * 
	 */ 
	void BindSocket();

	/**
	 * @brief listens for connections on port 
	 * 
	 */ 
	void ListenOnPort();

	/**
	 * @brief Maintains communication with server
	 *
	 */  
	void Communicate();

	/**
	 *@brief Accept the client request 
	 *
	 */  
	void AcceptRequest();

	/**
	 * @brief Executes the terminal command
	 * 
	 * @param cmd command to execute
	 * 
	 * @return result of executed command
	 */ 
	std::string ExecuteCommand(const char* cmd);

	/**
	 * @brief Creates child thread for running command
	 * 
	 * @param cmd command to execute 
	 * 
	 * @return result of executed command 
	 */ 
	std::string SpawnChild(const char* cmd);

	static void SigQuitHandler(int);

	~Server();

private:
	
	// server socket descriptor
  	int server_socket_handler;
	
	int port ;
	
	// descriptor for accepted socket returned by accept function
	int connection;

	char * server_ip;
};

#endif