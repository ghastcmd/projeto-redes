#include "pch.hpp"
#include "socket.hpp"
#include "poker.hpp"

void server_func()
{
    puts("just started the server function");

    conn::server serverInstance(2222);

    serverInstance.listen(1);
    // while (1)
    // {
        // char msg[64];
        // int lenght = serverInstance.recv(msg, sizeof(msg));

        // puts(msg);
    // }
}

#if !defined(TEST)

int main()
{
    std::thread serverThread(server_func);

    conn::client sclient("127.0.0.1", 2222);

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