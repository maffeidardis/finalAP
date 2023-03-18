#pragma once
#include "sys/types.h"

#define MY_PORT 7777
#define HOST_NAME_MAX 64

int serverSetup (short port, void* (func)(void* acc_s));
int clientConnection (short port, int argc, char* argv[]);
int sendStat (int client_socket, struct stat st);
int readStat (int accept_socket, off_t &filesize, time_t &mtime, char* filename);
int sendFile (int client_socket, char* filename, off_t filesize);
int readFile (int accept_socket, char* filename, off_t filesize);