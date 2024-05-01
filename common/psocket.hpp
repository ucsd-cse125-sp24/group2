/***************************************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Alejandro Ramírez Muñoz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
***************************************************************************************************/
#pragma once
#ifndef PSOCKET_H
#define PSOCKET_H

#include <iostream>

using namespace std;

#ifdef _WIN32
/**
 *   To compile in MinGW use -lws2_32 linker option
 *   To compile with MSVC++ use  #pragma comment(lib,"Ws2_32.lib")
 **/
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

typedef SOCKET sp_type;

#else
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

typedef int sp_type;
#endif

class Socket {
private:
    sp_type sockfd;

public:
    Socket();
    ~Socket();
    string getLastErrorMessage();
    bool socket(int domain, int type, int protocol);
    bool setNonBlock();
    bool nonBlockNoError();
    void close();
    bool setsockopt(int level, int optname, const void* optval,
                    socklen_t optlen);
#ifdef _WIN32
    int recv(char* buf, int len, int flags);
    int send(const char* buf, int len, int flags);
    int recvfrom(char* buf, int len, int flags, struct sockaddr* src_addr,
                 socklen_t* addrlen);
#else
    ssize_t recv(void* buf, size_t len, int flags);
    ssize_t send(const void* buf, size_t len, int flags);
    ssize_t recvfrom(void* buf, size_t len, int flags,
                     struct sockaddr* src_addr, socklen_t* addrlen);
#endif
    /* Same implementation */
    bool connect(const char* node, const char* service,
                 const struct addrinfo* hints);
    bool connect(const struct sockaddr* addr, socklen_t addrlen);
    bool listen(int backlog);
    bool bind(const struct sockaddr* addr, socklen_t addrlen);
    Socket* accept(struct sockaddr* addr, socklen_t* addrlen);
    sp_type getFD();
};

#ifdef __WIN32
int inet_pton(int af, const char* src, void* dst);
const char* inet_ntop(int af, const void* src, char* dst, socklen_t size);
#endif // __WIN32

#ifdef _WIN32
inline int inet_pton(int af, const char* src, void* dst) {
    struct sockaddr_storage ss;
    int size = sizeof(ss);
    char src_copy[INET6_ADDRSTRLEN + 1];

    ZeroMemory(&ss, sizeof(ss));
    /* stupid non-const API */
    strncpy(src_copy, src, INET6_ADDRSTRLEN + 1);
    src_copy[INET6_ADDRSTRLEN] = 0;

    if (WSAStringToAddress(src_copy, af, nullptr, (struct sockaddr*)&ss,
                           &size) == 0) {
        switch (af) {
        case AF_INET:
            *(struct in_addr*)dst = ((struct sockaddr_in*)&ss)->sin_addr;
            return 1;
        case AF_INET6:
            *(struct in6_addr*)dst = ((struct sockaddr_in6*)&ss)->sin6_addr;
            return 1;
        }
    }
    return 0;
}

inline const char* inet_ntop(int af, const void* src, char* dst,
                             socklen_t size) {
    struct sockaddr_storage ss;
    unsigned long s = size;

    ZeroMemory(&ss, sizeof(ss));
    ss.ss_family = af;

    switch (af) {
    case AF_INET:
        ((struct sockaddr_in*)&ss)->sin_addr = *(struct in_addr*)src;
        break;
    case AF_INET6:
        ((struct sockaddr_in6*)&ss)->sin6_addr = *(struct in6_addr*)src;
        break;
    default:
        return NULL;
    }
    /* cannot direclty use &size because of strict aliasing rules */
    return (WSAAddressToString((struct sockaddr*)&ss, sizeof(ss), nullptr, dst,
                               &s) == 0)
               ? dst
               : NULL;
}

inline Socket::Socket() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);
    sockfd = INVALID_SOCKET;
}
inline Socket::~Socket() {
    close();
    WSACleanup();
}
inline string Socket::getLastErrorMessage() {
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0)
        return ""; // No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, nullptr);

    string message(messageBuffer, size);
    LocalFree(messageBuffer);

    return message;
}
inline bool Socket::socket(int domain, int type, int protocol) {
    sockfd = ::socket(domain, type, protocol);
    if (sockfd == INVALID_SOCKET) {
        close();
        return false;
    }
    return true;
}
inline bool Socket::setNonBlock() {
    u_long iMode = 1;
    if (ioctlsocket(sockfd, FIONBIO, &iMode) == NO_ERROR) {
        return true;
    }
    return false;
}
inline bool Socket::nonBlockNoError() {
    return WSAGetLastError() == WSAEWOULDBLOCK;
}
inline void Socket::close() { ::closesocket(sockfd); }
inline int Socket::recv(char* buf, int len, int flags) {
    return ::recv(sockfd, buf, len, flags);
}
inline int Socket::send(const char* buf, int len, int flags) {
    return ::send(sockfd, buf, len, flags);
}
inline bool Socket::setsockopt(int level, int optname, const void* optval,
                               socklen_t optlen) {
    return ::setsockopt(sockfd, level, optname, (char*)optval, optlen) >= 0;
}
inline int Socket::recvfrom(char* buf, int len, int flags,
                            struct sockaddr* src_addr, socklen_t* addrlen) {
    return ::recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}
#else
inline Socket::Socket() {}
inline Socket::~Socket() { ::close(sockfd); }
inline string Socket::getLastErrorMessage() { return strerror(errno); }
inline bool Socket::socket(int domain, int type, int protocol) {
    sockfd = ::socket(domain, type, protocol);
    if (sockfd < 0) {
        ::close(sockfd);
        return false;
    }
    return true;
}
inline bool Socket::setNonBlock() {
    int flags = fcntl(sockfd, F_GETFL);
    flags = flags | O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flags) < 0) {
        return false;
    }
    return true;
}
inline bool Socket::nonBlockNoError() {
    return errno == EAGAIN || errno == EWOULDBLOCK;
}
inline void Socket::close() { ::close(sockfd); }
inline ssize_t Socket::recv(void* buf, size_t len, int flags) {
    return ::recv(sockfd, buf, len, flags);
}
inline ssize_t Socket::send(const void* buf, size_t len, int flags) {
    return ::send(sockfd, buf, len, flags);
}
inline bool Socket::setsockopt(int level, int optname, const void* optval,
                               socklen_t optlen) {
    return ::setsockopt(sockfd, level, optname, optval, optlen) >= 0;
}
inline ssize_t Socket::recvfrom(void* buf, size_t len, int flags,
                                struct sockaddr* src_addr, socklen_t* addrlen) {
    return ::recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
}
#endif
inline bool Socket::bind(const struct sockaddr* addr, socklen_t addrlen) {
    return ::bind(sockfd, addr, addrlen) >= 0;
}
inline bool Socket::listen(int backlog) {
    return ::listen(sockfd, backlog) >= 0;
}
inline bool Socket::connect(const char* node, const char* service,
                            const struct addrinfo* hints) {
    struct addrinfo *res, *rp;

    if (getaddrinfo(node, service, hints, &res) != 0) {
        freeaddrinfo(res);
        return false;
    }

    for (rp = res; rp != nullptr; rp = rp->ai_next) {
        if (socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) {
            if (connect(rp->ai_addr, rp->ai_addrlen)) {
                break;
            }
        }
        close();
    }

    if (rp == nullptr) {
        freeaddrinfo(res);
        return false;
    }
    freeaddrinfo(res);
    return true;
}

inline bool Socket::connect(const struct sockaddr* addr, socklen_t addrlen) {
    if (::connect(sockfd, addr, addrlen) != 0) {
        close();
        return false;
    }
    return true;
}

inline Socket* Socket::accept(struct sockaddr* addr, socklen_t* addrlen) {
    Socket* accepted = new Socket();
    accepted->sockfd = ::accept(sockfd, addr, addrlen);
    return accepted;
}

inline sp_type Socket::getFD() { return sockfd; }
#endif // PSOCKET_H