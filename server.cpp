#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
    // Port number is required
    if (argc != 2) {
        std::cerr << "Error: Port number is required!" << std::endl;
        exit(-1);
    }
    // grab the port number
    int port = std::atoi(argv[1]);

    // A buffer to send and receive messages with
    char buffer[1500];

    // Set up a socket and connection tools
    sockaddr_in server_address{};
    bzero((char *)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    // Open a stream oriented socket with internet address and keep track of the
    // socket descriptor
    int server_sd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sd < 0) {
        std::cerr << "Error starting the server socket!" << std::endl;
        exit(-1);
    }

    // Bind the socket to its local address
    int bind_status = bind(
        server_sd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_status < 0) {
        std::cerr << "Error binding the socket to the local address"
                  << std::endl;
        exit(-1);
    }

    std::cout << "Waiting for a client to connect..." << std::endl;

    // Listen for up to 5 requests at a time
    listen(server_sd, 5);

    // New address to connect with the client
    sockaddr_in new_socket_address{};
    socklen_t new_socket_address_size = sizeof(new_socket_address);

    // Accept and create a new socket descriptor to handle the new connection
    // with client
    int new_sd = accept(
        server_sd, (sockaddr *)&new_socket_address, &new_socket_address_size);
    if (new_sd < 0) {
        std::cerr << "Error accepting a request from a client!" << std::endl;
        exit(-1);
    }

    std::cout << "A client has successfully connected!" << std::endl;

    while (true) {
        memset(&buffer, 0, sizeof(buffer));  // Clear the buffer

        // Receive a message from a client
        if (recv(new_sd, (char *)&buffer, sizeof(buffer), 0) < 0) {
            std::cerr << "Error receiving a message from a client!"
                      << std::endl;
            break;
        }

        auto value = std::atoi(buffer);
        value++;
        auto value_str = std::to_string(value);
        memcpy(&buffer, &value_str[0], value_str.length());

        // Send the message to client
        if (send(new_sd, (char *)&buffer, strlen(buffer), 0) < 0) {
            std::cerr << "Error sending a message to a client!" << std::endl;
            break;
        }
    }

    // Close the socket descriptors
    close(new_sd);
    close(server_sd);

    return 0;
}
