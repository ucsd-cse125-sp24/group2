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
        perror("WSAStartup failed");
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
    while (1) {
        struct sockaddr_in client_sin;
        int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (struct sockaddr *)&client_sin, (socklen_t *)&addr_len);
        if (client_sock < 0) {
            perror("accept failed");
            return -1;
        }
        printf("Client (%s) connected on port %d\n", inet_ntoa(client_sin.sin_addr), client_sin.sin_port);

        pthread_t thread;
        int res = pthread_create(&thread, NULL, receive, &client_sock);
    }

    return 0;
}

void *Server::receive(void *params) {
    int *client_sock = (int *)params;
    char buffer[4096];
    int expected_data_len = sizeof(buffer);

    while (1) {
        int read_bytes = recv(*client_sock, &buffer, expected_data_len, 0);
        if (read_bytes == 0) {  // Connection was closed
            return NULL;
        } else if (read_bytes < 0) {  // error
#ifdef _WIN32
            closesocket(*client_sock);
#elif defined __APPLE__
            close(*client_sock);
#endif
            perror("recv failed");
            return NULL;
        } else {
            printf("Received %d bytes from client\n", read_bytes);
            printf("%.*s\n", read_bytes, buffer);
            int sent_bytes = send(*client_sock, &buffer, read_bytes, 0);
            if (sent_bytes < 0) {
                perror("send failed");
                return NULL;
            }
        }

        memset(buffer, 0, 4096);
    }

    return NULL;
}