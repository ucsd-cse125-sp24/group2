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

    int data_len;
    string input;
    char buf[4096];
    while (1) {
        memset(buf, 0, 4096);
        cout << "> ";
        cin >> input;  // get user input
        data_len = input.length();
        int sent_bytes = send(sock, &input[0], data_len, 0);
        if (sent_bytes < 0) {
            perror("send failed");
        }

        int read_bytes = recv(sock, &buf[0], 4096, 0);
        if (read_bytes == 0) {  // Connection was closed
            printf("Connection was closed\n");
            return;
        } else if (read_bytes < 0) {  // error
#ifdef _WIN32
            closesocket(sock);
#elif defined __APPLE__
            close(sock);
#endif
            perror("recv failed");
            return;
        } else {
            printf("Received %d bytes from server\n", read_bytes);
            printf("%.*s\n", read_bytes, &buf[0]);
        }
    }
}