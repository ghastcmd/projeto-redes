#pragma once

namespace socket
{

struct client
{
    client(const char *ip, unsigned int port);
};

struct server
{
    server(unsigned int port);
};

}