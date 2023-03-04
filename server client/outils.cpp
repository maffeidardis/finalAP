#include "base.h"
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>

int serverSetup (short port, void* (func)(void* acc_s)) {

	int server_socket = 1, bind_return = 1, listen_return = 1, accept_socket = 1;

	pthread_t tid;

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_port = htons(port);
	inet_aton("0.0.0.0", &service.sin_addr);

	server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Step 1: creating the server socket (returns 0 if good)
															   // AF_INET = IPv4 (address family)
															   // SOCK_STREM = TCP socket (socket type)
															   // IPPROTO_TCP = TCP protocol (protocol to be used)

	if (server_socket == -1) {
		std::cout << "Error creating server socket." << std::endl;
		return 0;
	}
	else {
		std::cout << "Server socket created." << std::endl;
	}

	bind_return = bind(server_socket, (struct sockaddr*)&service, sizeof(service)); // Step 2: binding the server (binding IP and Port)
																				    // server_socket = unbound socket
																				    // service = address to assign to the socket from the sockaddr struct
																				    // sizeof(service) = socket structure length in bytes

	if (bind_return == -1) {
		std::cout << "Error binding server socket." << std::endl;
		return 0;
	}
	else {
		std::cout << "Server binded." << std::endl;
	}

	listen_return = listen(server_socket, 10); // Step 3: listen to client requests
											   // server_socket = binded server
											   // 10 = number of max. connected clients (backlog)

	if (listen_return == -1) {
		std::cout << "Error listening requests: " << listen_return << std::endl;
		return 0;
	}
	else {
		std::cout << "Listening..." << std::endl;
	}

	while (true) {
		accept_socket = accept(server_socket, NULL, NULL); 	// Step 4: accepting connection
															// server_socket = listening socket
															// first NULL = optional structure containing the client address info
															// second NULL = size of this structure (if included)
	
		if (accept_socket == -1) {
			std::cout << "Error accepting request: " << accept_socket << std::endl;
			return 0;
		}
		else {
			std::cout << "Request accepted. Socket duplicated. Listening..." << std::endl;
		}

		int* acc_s = new int;
		*acc_s = accept_socket;
		pthread_create(&tid, NULL, func, (void*)acc_s);

	}

	return 0;
}

int clientConnection (short port, int argc, char* argv[]) {

    int client_socket, ret;
	
	struct sockaddr_in service;
	struct hostent * server;
	
	// Help to ./client execution
	if (argc == 1) {
		std::cerr << "Usage: " << argv[0] 
			  << " [ IP address / Server Name ]" << std::endl;
		return -1;
	}

	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Step 1: creating the server socket (returns 0 if good)
								   							   // AF_INET = IPv4 (address family)
								   							   // SOCK_STREM = TCP socket (socket type)
								   							   // IPPROTO_TCP = TCP protocol (protocol to be used)

	if (client_socket == -1) {
		std::cerr << "Error creating client socket." << std::endl;
		return -1;
	}
	else {
		std::cout << "Client socket created." << std::endl;
	}

	service.sin_family = AF_INET;
	service.sin_port = htons(port);
	server = gethostbyname(argv[1]);

	if (server == NULL) {
		std::cerr << "gethostbyname() failed" << std::endl;
		close(client_socket);
		return -1;
	}

	else {
		memcpy(&service.sin_addr, server->h_addr, 4);
	}

	ret = connect(client_socket, (struct sockaddr*)&service, sizeof(service)); // Step 2: connecting to server
										   									   // client_socket = descriptor of client socket
										   									   // &service = structure containing the server IP address
										   									   // sizeof(service) = its corresponding size

	if (ret == -1) {
		std::cerr << "Cannot connect to remote server." << std::endl;
		close(client_socket);
		return -1;
	}

    return client_socket;
}

void printDate(time_t t) {
	
	struct tm formatter;
	localtime_r(&t, &formatter);
	std::cout << formatter.tm_mday << "/" << formatter.tm_mon+1 << "/" << formatter.tm_year+1900 << " ";
	std::cout << formatter.tm_hour << ":" << formatter.tm_min << ":" << formatter.tm_sec << std::endl;

}

int sendStat (int client_socket, struct stat st) {

	// Send file size
	write(client_socket, &(st.st_size), sizeof(off_t));

	// Send last modification time
	time_t t = st.st_mtime;
	write(client_socket, &t, sizeof(time_t));

	return 0;

}

int readStat (int accept_socket, off_t &filesize, time_t &mtime) {

	// Read file size
	off_t fz;
	read(accept_socket, &fz, sizeof(off_t));
	filesize = fz;

	// Read last modification time
	time_t t = 0;
	read(accept_socket, &t, sizeof(time_t));
	mtime = t;

	return 0;

}

int sendFile (int client_socket, char* filename, off_t filesize) {

	FILE* fp = fopen(filename, "r");
	
	if (fp != NULL) {
		char* img = new char[filesize];

		off_t fsw = fread(img, 1, filesize, fp);

		if (filesize == fsw) {
			write(client_socket, img, filesize);
			delete []img;
			fclose(fp);
			return 0;
		}
		else {
			std::cerr << "Error reading file." << std::endl;
			return -1;
		}
	}

	else {
		std::cerr << "Error opening file." << std::endl;
		return -1;
	}

}

int readFile (int accept_socket, char* filename, off_t filesize) {

	FILE* fp = fopen(filename, "w");

	if (fp != NULL) {
		
		char* img = new char[filesize];
		read(accept_socket, img, filesize);

		off_t fsw = fwrite(img, 1, filesize, fp);
		if (filesize == fsw) {
			delete []img;
			fclose(fp);
			return 0;
		}
		else {
			std::cerr << "Error reading file." << std::endl;
			return -1;
		}
	}

	else {
		std::cerr << "Error opening file." << std::endl;
		return -1;
	}

}