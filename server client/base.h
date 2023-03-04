#pragma once
#include "sys/types.h"

#define MY_PORT 7777

int serverSetup (short port, void* (func)(void* acc_s));
int clientConnection (short port, int argc, char* argv[]);
void printDate(time_t t);
int sendStat (int client_socket, struct stat st);
int readStat (int accept_socket, off_t &filesize, time_t &mtime);
int sendFile (int client_socket, char* filename, off_t filesize);
int readFile (int accept_socket, char* filename, off_t filesize);