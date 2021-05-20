#include "pch.hpp"
#include "socket.hpp"
#include "poker.hpp"

void server_func()
{
    puts("Just started the server function");

    conn::server serverInstance(2222);

    serverInstance.bind();
    for (int i = 0; i < 5; i++)
    {
        serverInstance.listen(1);
        auto sock = serverInstance.accept();

        char msg[23];
        int lenght = serverInstance.recv(sock, msg, sizeof(msg));
        msg[lenght] = '\0';

        puts(msg);
    }
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