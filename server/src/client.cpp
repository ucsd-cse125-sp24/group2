#include "client.hpp"

void Client::init() {
    pthread_t thread;
    int res = pthread_create(&thread, NULL, receive, this);
}

void* Client::receive(void* params) {
    Client* client = (Client*) (params);
    char buffer[4096];
    int expected_data_len = sizeof(buffer);

    while (1) {
        int read_bytes = recv(client->sockfd, (char*) &buffer, expected_data_len, 0);
        if (read_bytes == 0) {  // Connection was closed
            return NULL;
        } else if (read_bytes < 0) {  // error
#ifdef _WIN32
            closesocket(client->sockfd);
#elif defined __APPLE__
            close(*client_sock);
#endif
            perror("recv failed");
            return NULL;
        } else {
            printf("Received %d bytes from client %d\n", read_bytes, client->id);
            printf("%.*s\n", read_bytes, buffer);

            int sent_bytes = send(client->sockfd, (const char*) &buffer, read_bytes, 0);
            if (sent_bytes < 0) {
                perror("send failed");
                return NULL;
            }
        }

        memset(buffer, 0, 4096);
    }

    return 0;
}


void Client::handle_packet(void *packet) {
    std::string *cmd = new std::string((char *)packet);
    if (cmd->compare("forward")) {
    } else if (cmd->compare("back")) {
    } else if (cmd->compare("left")) {
    } else if (cmd->compare("right")) {
    }

    delete cmd;
}