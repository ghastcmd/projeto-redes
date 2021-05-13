#include "pch.hpp"
#include "socket.hpp"

namespace socket
{

client::client(const char *ip, unsigned int port)
{
    puts(ip);
    printf("%u\n", port);
}

}