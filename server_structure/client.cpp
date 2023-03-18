#include "base.h"
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

int client_socket; // Global variable (needed by the ctrl+c process)

// Define the function to be called when ctrl-c (SIGINT) is sent to process
void signal_callback_handler(int signum) {
    std::cout << " Client shutdown" << std::endl;
    int flag = 3;
    if (write(client_socket, &flag, sizeof(int)) == -1) {
                std::cerr << "Error writing flag=1" << std::endl;
    }
    // Terminate program
    exit(signum);
}

int main (int argc, char* argv[]) {

    // ctrl+c event: client disconnection
    signal(SIGINT, signal_callback_handler);

    // Client connection
    client_socket = clientConnection(MY_PORT, argc, argv);

    if (client_socket == -1) {
        std::cerr << "Error clientConnect()" << std::endl;
        return -1;
    }

    int flag = 1; // flag -> {1: waiting refresh, 2: send image, 3: client disconnection}
    time_t start = time(NULL);
    time_t now = time(NULL);

    while (true) {

        while ((now - start < 1)) {
            flag = 1;
            if (write(client_socket, &flag, sizeof(int)) == -1) {
                std::cerr << "Error writing flag=1" << std::endl;
            }
            now = time(NULL);
        }

        flag = 2;
        if (write(client_socket, &flag, sizeof(int)) == -1) {
            std::cerr << "Error writing flag=0" << std::endl;
        }

        // Take pic
        if (system("python3 capture_image.py") != 0) {
            std::cerr << "Error capture_image.py" << std::endl;
        }

        // File metadata
        struct stat st;

        if (stat("webcam.jpg", &st) == -1) {
            std::cerr << "stat() error" << std::endl;
            return -1;
        }

        // Send image stats
        sendStat(client_socket, st);

        // Send image
        char filename[20] = "webcam.jpg";
        sendFile(client_socket, filename, st.st_size);

        // Refresh
        start = time(NULL);

    }

    return 0;

}