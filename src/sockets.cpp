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

void client::connc() const
{
    connect(m_socket, (struct sockaddr*)&m_consocket, sizeof(struct sockaddr_in));
}

void client::sendc(const char *msg)
{
    send(m_socket, msg, strlen(msg), 0);
}

int client::recc(char *buffer, int lenght)
{
    return recv(m_socket, buffer, lenght, 0);
}

client::~client()
{
    closesocket(m_socket);
    WSACleanup();
}

server::server(unsigned int port)
{
    printf("%i\n", port);
}

}