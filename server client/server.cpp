#include "base.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>
#include <bits/stdc++.h>

void* getImg (void* acc_s) {

    int accept_socket = *((int *)acc_s);
    off_t filesize;
    time_t mtime;

    char hn[HOST_NAME_MAX] = "none", img_ext[5] = ".jpg";
    read(accept_socket, &(hn), HOST_NAME_MAX);
    std::strcat(hn, img_ext);
    
    int flag;
    
    while (true) {

        read(accept_socket, &flag, sizeof(int));

        if (flag == 0) {
            readStat(accept_socket, filesize, mtime);
            readFile(accept_socket, hn, filesize);
        }
    }

    return NULL;
}

int main (int argc, char** argv) {

    serverSetup(MY_PORT, getImg);
    return 0;

}