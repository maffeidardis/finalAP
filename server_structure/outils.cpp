#include "base.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <time.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

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

		if (pthread_create(&tid, NULL, func, (void*)acc_s) != 0) {
			std::cerr << "pthread_create() error" << std::endl;
		}

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

char* formatDate(time_t t) {
	
	struct tm formatter;
	localtime_r(&t, &formatter);

	std::string day = std::to_string(formatter.tm_mday);
	std::string month = std::to_string(formatter.tm_mon+1);
	std::string year = std::to_string(formatter.tm_year+1900);
	std::string hour = std::to_string(formatter.tm_hour);
	std::string minute = std::to_string(formatter.tm_min);
	std::string second = std::to_string(formatter.tm_sec);

	std::string date_str = day + "/" + month + "/" + year + " " + hour + ":" + minute + ":" + second;
	
	const int length = date_str.length();
	char* date_char = new char[length + 1];
	strcpy(date_char, date_str.c_str());

	return date_char;

}

int sendStat (int client_socket, struct stat st) {

	// Send client hostname
    char hostname[HOST_NAME_MAX];
    gethostname(hostname, HOST_NAME_MAX);
    if (write(client_socket, &(hostname), HOST_NAME_MAX) == -1) {
        std::cerr << "Error writing hostname (sendStat)" << std::endl;
    }

	// Send file size
	if (write(client_socket, &(st.st_size), sizeof(off_t)) == -1) {
		std::cerr << "Error writing size error (sendStat)" << std::endl;
	}

	// Send last modification time
	time_t t = st.st_mtime;
	if (write(client_socket, &t, sizeof(time_t)) == -1) {
		std::cerr << "Error writing time (sendStat)" << std::endl;
	}

	return 0;

}

int readStat (int accept_socket, off_t &filesize, time_t &mtime, char* filename) {

	// Reading hostname
    char hostname[HOST_NAME_MAX];
    if (read(accept_socket, &(hostname), HOST_NAME_MAX) == -1) {
        std::cerr << "Error reading hostname (readStat)" << std::endl;
    }

	// Read file size
	off_t fz;
	if (read(accept_socket, &fz, sizeof(off_t)) == -1) {
		std::cerr << "Error reading size (readStat)" << std::endl;
	}
	filesize = fz;

	// Read last modification time
	time_t t = 0;
	if (read(accept_socket, &t, sizeof(time_t)) == -1) {
		std::cerr << "Error reading time (readTime)" << std::endl;
	}
	mtime = t;
	char* date = formatDate(mtime);

	// Saving stats on a .txt file
	std::ofstream data(filename);
	data << hostname << "\n" << date;

	return 0;

}

int sendFile (int client_socket, char* filename, off_t filesize) {

	FILE* fp = fopen(filename, "r");
	
	if (fp != NULL) {
		char* img = new char[filesize];

		off_t fsw = fread(img, 1, filesize, fp);

		if (filesize == fsw) {
			if (write(client_socket, img, filesize) == -1) {
				std::cerr << "sendFile write img error" << std::endl;
			}
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
		if (read(accept_socket, img, filesize) == -1) {
			std::cerr << "Error reading image (readFile)" << std::endl;
		}

		off_t fsw = fwrite(img, 1, filesize, fp);
		if (filesize == fsw) {
			delete []img;
			fclose(fp);
			return 0;
		}
		else {
			std::cerr << "Error reading file (readFile)" << std::endl;
			return -1;
		}
	}

	else {
		std::cerr << "Error opening file (readFile)" << std::endl;
		return -1;
	}

}