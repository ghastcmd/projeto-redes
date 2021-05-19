#include "pch.hpp"
#include "socket.hpp"
#include "poker.hpp"

void server_func()
{
    puts("just started the server function");

    conn::server sockServer(2222);
}

#if !defined(TEST)

int main()
{
    conn::client sclient("localhost", 2222);

    std::thread serverThread(server_func);

    if (!sclient.connect())
    {
        puts("couldn't connect to server");
    }
    else
    {
        sclient.send("simple message");
    }

    serverThread.join();

    return 0;
}

#endif