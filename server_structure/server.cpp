#include "base.h"
#include <iostream>
#include <sstream>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
//If running on Linux, include this tag.
//#include <bits/stdc++.h>

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) {
    std::cout << " Server shutdown" << std::endl;

    struct stat sb;
    if (stat("../website/static/images", &sb) == 0) {
        if (system("rm -r ../website/static/images") != 0) {
            std::cerr << "Error removing static/images folder" << std::endl; // remove images folder if it exists
        }
    }

    // Terminate program
    exit(signum);
}

void* getImg (void* acc_s) {

    int accept_socket = *((int *)acc_s);

    // Folder name with client socket code
    char foldername[50] = "../website/static/images/socket";
    std::string str = std::to_string(accept_socket);
    char const *socket_str = str.c_str();
    std::strcat(foldername, socket_str); // foldername -> "socketX"

    // Creating folder
    char command_mkdir[50] = "mkdir ";
    std::strcat(command_mkdir, foldername); // command -> "mkdir socketX"

    struct stat sb;

    if (stat(foldername, &sb) != 0) {
        if (system(command_mkdir) != 0) {
            std::cerr << "Error creating client folder" << std::endl; // creates folder if doesn't exist
            close(accept_socket);
            return NULL;
        }
    }

    // Image filepath
    char img_path[50];
    std::strcpy(img_path, foldername);
    char img_name[12] = "/webcam.jpg";
    std::strcat(img_path, img_name); // img_path -> "socketX/webcam.jpg"

    // Data filepath
    char data_path[50];
    std::strcpy(data_path, foldername);
    char data_name[10] = "/data.txt";
    std::strcat(data_path, data_name); // data_path -> "socketX/data.txt"

    // Receiving data from client (stat and img)
    off_t filesize;
    time_t mtime;
    int flag = 1;
    
    while (flag != 3) {

        if (read(accept_socket, &flag, sizeof(int)) == -1) {
            std::cerr << "Error reading flag value" << std::endl;
            close(accept_socket);
            return NULL;
        }

        // flag -> 2: new image coming
        if (flag == 2) {
            if (readStat(accept_socket, filesize, mtime, data_path) == -1) {
            readStat(accept_socket, filesize, mtime, data_path);
                std::cerr << "Error reading data file" << std::endl;
                close(accept_socket);
                return NULL;
            }
            if (readFile(accept_socket, img_path, filesize) == -1) {
            readFile(accept_socket, img_path, filesize);
                std::cerr << "Error reading image file" << std::endl;
                close(accept_socket);
                return NULL;
            }
        }

        // flag -> 3: client disconnected
        if (flag == 3) {
            std::cout << "A client has disconnected" << std::endl;
        }

    }

    // Remove client folder
    char command_rm[50] = "rm -r ";
    std::strcat(command_rm, foldername); // command -> "rm -r socketX"
    
    if (system(command_rm) != 0) {
        std::cerr << "Error removing client folder" << std::endl; // creates folder if doesn't exist
    }

    close(accept_socket);

    return NULL;
}

int main (int argc, char** argv) {

    // Creating folder images
    struct stat sb;
    if (stat("../website/static/images", &sb) != 0) {
        if (system("mkdir ../website/static/images") != 0) {
            std::cerr << "Error creating static/images folder" << std::endl; // creates folder if doesn't exist
	
            return 1;
        }
    }

    signal(SIGINT, signal_callback_handler);
    serverSetup(MY_PORT, getImg);
    return 0;

}