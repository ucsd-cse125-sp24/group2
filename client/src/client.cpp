#include "client.hpp"

Client::Client() {
}

Client::~Client() {
#ifdef _WIN32
    WSACleanup();
#endif
}

void Client::init() {
#ifdef _WIN32
    int res;
    res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (res != 0) {
        perror("WSAStartup failed\n");
    }
#endif

    // AF_INET: IPv4 address family (also OK with PF_INET)
    // SOCK_STREAM: Streaming socket type
    // perror(): Prints out error msg
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket() failed");
        return;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_port = htons(25565);

    if (connect(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("connection failed");
    }

    char *data_addr = "hello, world";
    int data_len = 12;

    int sent_bytes = send(sock, data_addr, data_len, 0);
    if (sent_bytes < 0) {
        perror("send failed");
    }

    /*
    #ifdef _WIN32
        closesocket(sock);
    #else
        close(sock);
    #endif
    */

    while (1) {
        #ifdef _WIN32
        Sleep(1000);
        #else
        sleep(1);
        #endif

        printf("Client still alive\n");
    }
}