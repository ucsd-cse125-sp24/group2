#include "client.hpp"
#include <glm/glm.hpp>
#include <iomanip>
union FloatUnion {
    uint32_t i;
    float f;
} num;

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

    if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("connection failed");
    }

    pthread_t thread;
    pthread_create(&thread, NULL, receive, &sock);

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
    }
}

void* Client::receive(void* params) {
    int sock = *((int*)params);
    uint8_t buf[4096];
    while (1) {
        int read_bytes = recv(sock, buf, 4096, 0);
        if (read_bytes == 0) {  // Connection was closed
            printf("Connection was closed\n");
            return NULL;
        } else if (read_bytes < 0) {  // error
#ifdef _WIN32
            closesocket(sock);
#elif defined __APPLE__
            close(sock);
#endif
            perror("recv failed");
            return NULL;
        } else {
            printf("Received %d bytes from server\n", read_bytes);
            uint32_t tmp;
            memcpy(&tmp, buf, 4);
            num.i = ntohl(tmp);
            float x = num.f;

            memcpy(&tmp, buf + 4, 4);
            num.i = ntohl(tmp);
            float y = num.f;

            memcpy(&tmp, buf + 8, 4);
            num.i = ntohl(tmp);
            float z = num.f;

            printf("(%g, %g, %g)\n", x, y, z);
        }

        memset(buf, 0, 4096);
    }

    return NULL;
}