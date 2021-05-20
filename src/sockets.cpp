#include "pch.hpp"
#include "socket.hpp"

namespace conn {
decltype(&::connect) s_connect = ::connect;
decltype(&::send)    s_send    = ::send;
decltype(&::recv)    s_recv    = ::recv;
decltype(&::bind)    s_bind    = ::bind;
decltype(&::listen)  s_listen  = ::listen;
decltype(&::accept)  s_accept  = ::accept;



int basic_socket::get_error() const
{
#if defined(Windows)
    return WSAGetLastError();
#elif defined(Linux)
#endif
}

void basic_socket::send(const char* msg) const
{
    s_send(m_socket, msg, strlen(msg), 0);
}

client::client(const char *ip, unsigned int port)
{
#if defined(Windows)
    // Windows specific initialization of initial socket data
    WSADATA wsa;
    int wsa_ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (wsa_ret != 0) throw "WSA Startup failed", get_error();
#endif

    // Starting the socket with ip protocol and tcp connection
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == INVALID_SOCKET) throw "Invalid socket returned ", get_error();

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

client::~client()
{
#if defined(Windows)
    int sockResult = closesocket(m_socket);
    WSACleanup();
#elif defined(Linux)
    close(m_socket);
#endif
}

bool client::connect() const
{
    int ret = s_connect(m_socket, (struct sockaddr*)&m_consocket, sizeof(m_consocket));
    return ret != -1;
}

int client::recv(char *buffer, int lenght)
{
    return s_recv(m_socket, buffer, lenght, 0);
}

server::server(unsigned int port)
    : m_bount(false)
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

void server::bind()
{
    int ret = s_bind(m_socket, (struct sockaddr*)&m_consocket, sizeof(decltype(m_consocket)));
    if (ret != 0) throw "Could not bind to the socket", get_error();
    m_bount = true;
}

void server::listen(const size_t max) const
{
    if (!m_bount) throw "Need to bind first";
    if (!(max <= SOMAXCONN)) throw "Too big of count";
    int ret = s_listen(m_socket, max);
    if (ret != 0) throw "The server could not listen... ", get_error();
}

socket_t server::accept() const
{
    int addr_len = sizeof(m_consocket);
    return s_accept(m_socket, (struct sockaddr*)&m_consocket, &addr_len);
}

int server::recv(socket_t fd, char *buffer, int buffer_len) const
{
    return s_recv(fd, buffer, buffer_len, 0);
}

}