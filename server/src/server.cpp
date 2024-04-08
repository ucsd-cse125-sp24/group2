#include "server.hpp"

Server::Server() {
}

Server::~Server() {
#ifdef _WIN32
    WSACleanup();
#endif
}

int Server::init() {
#ifdef _WIN32
    int res;
    res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (res != 0) {
        perror("WSAStartup failed\n");
    }
#endif

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(sock, MAX_CLIENTS) < 0) {
        perror("listen failed");
        return 1;
    }

    printf("Now listening...\n");

    struct sockaddr_in client_sin;
    int addr_len = sizeof(client_sin);
    int client_sock = accept(sock, (struct sockaddr *)&client_sin, (socklen_t *)&addr_len);
    if (client_sock < 0) {
        perror("accept failed");
        return -1;
    }

    char buffer[4096];
    int expected_data_len = sizeof(buffer);

    int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
    if (read_bytes == 0) {
        // connection is closed
    } else if (read_bytes < 0) {  // error
        perror("recv failed");
    } else {
        printf("Received %d bytes from client\n", read_bytes);
        printf("%.*s\n", read_bytes, buffer);
    }

#ifdef _WIN32
    closesocket(client_sock);
#elif defined __APPLE__
    close(client_sock);
#endif

    return 0;
}