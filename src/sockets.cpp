#include "pch.hpp"
#include "socket.hpp"

namespace conn
{

client::client(const char *ip, unsigned int port)
{
#if defined(Windows)
    // windows specific initialization of initial socket data
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    // starting the socket with ip protocol and tcp connection
    m_socket = socket(AF_INET, SOCK_STREAM, 0);

    m_consocket.sin_addr.s_addr = inet_addr(ip);
    m_consocket.sin_family = AF_INET;
    m_consocket.sin_port = htons(port);
}

bool client::connect() const
{
    int ret = ::connect(m_socket, (struct sockaddr*)&m_consocket, sizeof(m_consocket));
    return ret != -1;
}

void client::send(const char *msg)
{
    ::send(m_socket, msg, strlen(msg), 0);
}

int client::recv(char *buffer, int lenght)
{
    return ::recv(m_socket, buffer, lenght, 0);
}

client::~client()
{
#if defined(Windows)
    closesocket(m_socket);
    WSACleanup();
#elif defined(Linux)
    close(m_socket);
#endif
}

server::server(unsigned int port)
{
#if defined(Windows)
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    m_socket = socket(AF_INET, SOCK_STREAM, 0);

    m_consocket.sin_family = AF_INET;
    m_consocket.sin_port = htons(port);
    m_consocket.sin_addr.s_addr = htonl(INADDR_ANY);
}

server::~server()
{
#if defined(Windows)
    closesocket(m_socket);
    WSACleanup();
#elif defined(Linux)    
    close(m_socket);
#endif
}

void server::bind() const
{
    ::bind(m_socket, (struct sockaddr*)&m_consocket, sizeof(decltype(m_consocket)));
}

void server::listen(const size_t max) const
{
    if (!max <= SOMAXCONN)
    {
        throw "Too big of ammount";
    }
    ::listen(m_socket, max);
}

void server::send(const char *msg) const
{
    ::send(m_socket, msg, strlen(msg), 0);
}

int server::recv(char *buffer, int lenght) const
{
    return ::recv(m_socket, buffer, lenght, 0);
}

}