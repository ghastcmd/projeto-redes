#include "pch.hpp"
#include "socket.hpp"

namespace conn {

int basic_socket::get_error() const
{
#if defined(Windows)
    return WSAGetLastError();
#elif defined(Linux)
    return errno;
#endif
}

void basic_socket::print_error(const char* msg) const
{
    fprintf(stderr, ">>> %s\n", msg);
}

void basic_socket::print_errorg(const char* msg) const
{
    auto error_code = get_error();
    fprintf(stderr, ">>> %s, error code: %i\n", msg, error_code);
    if (m_print_error_str) puts(strerror(error_code)), puts("");
}

void basic_socket::send(const char* msg) const
{
    ::send(m_socket, msg, strlen(msg), 0);
}

basic_socket::~basic_socket()
{
#if defined(Windows)
    closesocket(m_socket);
    WSACleanup();
#elif defined(Linux)    
    m_print_error_str = true;
    ::close(m_socket);
#endif
}

void basic_socket::wsa_startup() const
{
#if defined(Windows)
    WSADATA wsa;
    int wsa_ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (wsa_ret != 0)
        print_error("WSA Startup failed");
#endif
}

basic_socket::basic_socket()
{
    wsa_startup();
}

void sock::send(const char* msg) const
{
    s_send(m_sock_fd, msg, strlen(msg), 0);
}

int sock::recv(char* buffer, size_t packet_size) const
{
    return s_recv(m_sock_fd, buffer, packet_size, 0);
}

sock::sock(socket_t fd)
    : m_sock_fd(fd)
{

}

sock::~sock()
{
#if defined(Windows)
    closesocket(m_sock_fd);
#elif defined(Linux)
    ::close(m_sock_fd);
#endif
}

client::client(const char *ip, unsigned int port)
{
    // Starting the socket with ip protocol and tcp connection
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket <= 0)
        print_errorg("Invalid socket returned");

    m_consocket.sin_addr.s_addr = inet_addr(ip);
    if (m_consocket.sin_addr.s_addr == INADDR_NONE)
        print_error("Invalid ip address, none");
    else if (m_consocket.sin_addr.s_addr == INADDR_ANY)
        print_error("Invalid ip address, any");

    m_consocket.sin_family = AF_INET;
    m_consocket.sin_port = htons(port);
}

sock client::connect() const
{
    int ret = ::connect(m_socket, (struct sockaddr*)&m_consocket, sizeof(m_consocket));
    if (ret != 0) print_errorg("Could not connect to server with socket");
    return ret != -1;
}

int client::recv(char *buffer, int lenght)
{
    return ::recv(m_socket, buffer, lenght, 0);
}

server::server(unsigned int port)
    : m_bount(false)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);

    m_consocket.sin_family = AF_INET;
    m_consocket.sin_port = htons(port);
    m_consocket.sin_addr.s_addr = htonl(INADDR_ANY);
}

void server::bind()
{
    int ret = ::bind(m_socket, (struct sockaddr*)&m_consocket, sizeof(m_consocket));
    if (ret != 0) print_errorg("Could not bind to the socket");
    m_bount = true;
}

void server::listen(const size_t max) const
{
    if (!m_bount) print_error("Need to bind first");
    if (!(max <= SOMAXCONN)) print_error("The max count number not supported");
    int ret = ::listen(m_socket, max);
    if (ret != 0) print_errorg("The server could not listen... ");
}

sock server::accept() const
{
    socketlen_t csize = sizeof(m_consocket);
    auto ret = ::accept(m_socket, (struct sockaddr*)&m_consocket, &csize);
    if (ret <= 0) print_errorg("Could not accept the incoming connection");
    return ret;
}

int server::recv(socket_t fd, char *buffer, int buffer_len) const
{
    auto ret = ::recv(fd, buffer, buffer_len, 0);
    if (ret == -1) print_errorg("Could not get message from connection");
    return ret;
}

}