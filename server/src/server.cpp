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
        int i = 0;
        for (i = 0; i < MAX_CLIENTS; i++) {
            // Find first free slot
            if (clients.find(i) != clients.end()) continue;

            Client* client_info = new Client(i, client_sock, client_sin);
            clients[i] = client_info;

            printf("Client (%s:%d) was assigned id %d. Server capacity: %d / %d\n", inet_ntoa(client_sin.sin_addr), client_sin.sin_port, i, clients.size(), MAX_CLIENTS);
            client_info->init();

            break;
        }

        if (i == MAX_CLIENTS) {
            perror("Server is full");
        }
    }

    return 0;
}

void Server::handle_packet(void *packet) {
    std::string *cmd = new std::string((char *)packet);
    if (cmd->compare("forward")) {
    } else if (cmd->compare("back")) {
    } else if (cmd->compare("left")) {
    } else if (cmd->compare("right")) {
    }

    delete cmd;
}