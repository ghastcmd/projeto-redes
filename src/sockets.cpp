#include "pch.hpp"
#include "socket.hpp"

namespace conn {
std::function<decltype(::connect)> s_connect = ::connect;
std::function<decltype(::send)>    s_send    = ::send;
std::function<decltype(::recv)>    s_recv    = ::recv;
std::function<decltype(::bind)>    s_bind    = ::bind;
std::function<decltype(::listen)>  s_listen  = ::listen;

client::client(const char *ip, unsigned int port)
{
#if defined(Windows)
    // windows specific initialization of initial socket data
    WSADATA wsa;
    int wsa_ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (wsa_ret != 0) puts("WSA Startup failed") ;
#endif

    // starting the socket with ip protocol and tcp connection
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) std::cout << "Invalid socket returned " << WSAGetLastError() << '\n';

    // auto addr_val = inet_addr(ip);

#if defined(Windows)
    m_consocket.sin_addr.s_addr = inet_addr(ip);
#elif defined(Linux)
    m_consocket.sin_addr.s_addr = inet_addr(ip);
#endif

    if (m_consocket.sin_addr.s_addr == INADDR_NONE) puts("Invalid ip address, none");
    else if (m_consocket.sin_addr.s_addr == INADDR_ANY) puts("Invalid ip address, any");

    m_consocket.sin_family = AF_INET;
    m_consocket.sin_port = htons(port);
}

bool client::connect() const
{
    int ret = s_connect(m_socket, (struct sockaddr*)&m_consocket, sizeof(m_consocket));
    return ret != -1;
}

void client::send(const char *msg)
{
    s_send(m_socket, msg, strlen(msg), 0);
}

int client::recv(char *buffer, int lenght)
{
    return s_recv(m_socket, buffer, lenght, 0);
}

client::~client()
{
#if defined(Windows)
    int sockResult = closesocket(m_socket);
    if (sockResult == SOCKET_ERROR) std::cout << "Socket closed with an error " << WSAGetLastError() << '\n';
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
    s_bind(m_socket, (struct sockaddr*)&m_consocket, sizeof(decltype(m_consocket)));
}

void server::listen(const size_t max) const
{
    if (!(max <= SOMAXCONN))
    {
        throw "Too big of ammount";
    }
    s_listen(m_socket, max);
}

void server::send(const char *msg) const
{
    s_send(m_socket, msg, strlen(msg), 0);
}

int server::recv(char *buffer, int lenght) const
{
    return s_recv(m_socket, buffer, lenght, 0);
}

}