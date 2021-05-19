#include "pch.hpp"
#include "socket.hpp"
#include "poker.hpp"

#if !defined(TEST)

int main()
{
    conn::client sclient("127.0.0.1", 2222);

    if (!sclient.connect())
    {
        puts("couldn't connect to server");
    }
    else
    {
        sclient.send("simple message");
    }

    return 0;
}

#endif