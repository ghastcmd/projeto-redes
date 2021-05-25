#include "pch.hpp"
#include "socket.hpp"

namespace conn {

int basic_socket::get_error()
{
#if defined(Windows)
    return WSAGetLastError();
#elif defined(Linux)
    return errno;
#endif
}

void basic_socket::print_error(const char* msg)
{
    fprintf(stderr, ">>> %s\n", msg);
}

std::string basic_socket::get_error_string(int error_code)
{
    char * str = nullptr;
#if defined(Windows)
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
        NULL, error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&str, 0, NULL
    );
#elif defined(Linux)
    str = (strdup(strerror(error_code)));
#endif
    return {str};
}

void basic_socket::print_errorg(const char* msg)
{
    auto error_code = get_error();
    fprintf(stderr, ">>> %s, error code: %i\n", msg, error_code);
    std::cout << '\t' << get_error_string(error_code) << '\n';
}

void basic_socket::assertf(bool op, const char* msg)
{
    if (op) print_error(msg);
}

void basic_socket::assertg(bool op, const char* msg)
{
    if (op) print_errorg(msg);
}

basic_socket::~basic_socket()
{
#if defined(Windows)
    closesocket(m_socket);
    WSACleanup();
#elif defined(Linux)    
    ::close(m_socket);
#endif
}

void basic_socket::wsa_startup() const
{
#if defined(Windows)
    WSADATA wsa;
    int wsa_ret = WSAStartup(MAKEWORD(2, 2), &wsa);
    assertf(wsa_ret != 0, "WSA Startup Failed");
#endif
}

basic_socket::basic_socket()
{
    wsa_startup();
}

void sock::send(const char* msg) const
{
    ::send(m_sock_fd, msg, strlen(msg), 0);
}

int sock::recv(char* buffer, size_t packet_size) const
{
    return ::recv(m_sock_fd, (char*)buffer, (int)packet_size, 0);
}

int socket_num = 0;

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
    assertg(m_socket <= 0, "Invalid socket returned");

    m_consocket.sin_addr.s_addr = inet_addr(ip);
    assertf(m_consocket.sin_addr.s_addr == INADDR_NONE, "Invalid ip address, none");
    assertf(m_consocket.sin_addr.s_addr == INADDR_ANY, "Invalid ip address, any");

    m_consocket.sin_family = AF_INET;
    m_consocket.sin_port = htons(port);
}

sock client::connect() const
{
    auto ret = ::connect(m_socket, (struct sockaddr*)&m_consocket, sizeof(m_consocket));
    assertg(ret != 0, "Could not connect to server with socket");
    return sock{m_socket};
}

server::server(unsigned int port)
    : m_bount(false)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    assertg(m_socket <= 0, "Invalid socket returned");

    m_consocket.sin_family = AF_INET;
    m_consocket.sin_port = htons(port);
    m_consocket.sin_addr.s_addr = htonl(INADDR_ANY);
}

void server::bind()
{
    int ret = ::bind(m_socket, (struct sockaddr*)&m_consocket, sizeof(m_consocket));
    assertg(ret != 0, "Could not bind to the socket");
    m_bount = true;
}

void server::listen(const size_t max) const
{
    assertf(!m_bount, "Need to bind first");
    assertf(!(max <= SOMAXCONN), "The max count number not supported");
    int ret = ::listen(m_socket, max);
    assertg(ret != 0, "The server could not listen...");
}

socket_t server::accept() const
{
    socketlen_t csize = sizeof(m_consocket);
    auto ret = ::accept(m_socket, (struct sockaddr*)&m_consocket, &csize);
    assertf(ret <= 0, "Could not accept the incoming connection");
    return ret;
}

}