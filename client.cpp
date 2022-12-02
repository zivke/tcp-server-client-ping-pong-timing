#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

int main(int argc, char* argv[])
{
    // IP address and port number are required (in that order)
    if (argc < 3) {
        std::cerr
            << "Error: IP address and port number are required (in that order)!"
            << std::endl;
        exit(-1);
    }

    std::string server_ip(argv[1]);
    int port = std::atoi(argv[2]);
    uint32_t ping_pongs = 1000;

    if (argc == 4) {
        ping_pongs = std::atoi(argv[3]);
    }

    std::cout << "Number of ping-pongs: " << ping_pongs << std::endl;

    // Create a message buffer
    char buffer[1500];

    // Set up a socket and connection tools
    struct hostent* host = gethostbyname(server_ip.c_str());
    sockaddr_in send_socket_address{};
    bzero((char*)&send_socket_address, sizeof(send_socket_address));
    send_socket_address.sin_family = AF_INET;
    send_socket_address.sin_addr.s_addr =
        inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    send_socket_address.sin_port = htons(port);

    int client_sd = socket(AF_INET, SOCK_STREAM, 0);

    // Try to connect
    int status = connect(client_sd,
                         (sockaddr*)&send_socket_address,
                         sizeof(send_socket_address));
    if (status < 0) {
        std::cerr << "Error connecting to a socket!" << std::endl;
        exit(-1);
    }

    std::cout << "Connected to the server!" << std::endl;

    auto stopwatch_begin = std::chrono::high_resolution_clock::now();

    int value = 0;
    std::string value_str = std::to_string(value);
    memcpy(&buffer, &value_str[0], value_str.length());
    send(client_sd, (char*)&buffer, strlen(buffer), 0);

    while (true) {
        recv(client_sd, (char*)&buffer, sizeof(buffer), 0);

        value = std::atoi(buffer);
        if (value >= ping_pongs) {
            auto stopwatch_end = std::chrono::high_resolution_clock::now();
            auto microseconds =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    stopwatch_end - stopwatch_begin)
                    .count();
            std::cout << "Elapsed time: " << std::fixed << std::setprecision(3)
                      << microseconds / 1000.0 << " ms" << std::endl;
            break;
        }

        value++;
        auto value_str_tmp = std::to_string(value);

        memcpy(&buffer, &value_str_tmp[0], value_str_tmp.length());

        send(client_sd, (char*)&buffer, strlen(buffer), 0);
    }

    close(client_sd);

    return 0;
}
