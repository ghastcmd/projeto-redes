#include "pch.hpp"
#include "socket.hpp"
#include "poker.hpp"

void server_func()
{
    puts("Just started the server function");

    conn::server serverInstance(2222);
    
    serverInstance.bind();
    serverInstance.listen(1);
    for (int i = 0; i < 1; i++)
    {
        auto sock = serverInstance.accept();

        char msg[23];
        int lenght = serverInstance.recv(sock, msg, sizeof(msg));
        msg[lenght] = '\0';

        puts(msg);
        puts("Just put the message into terminal");
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
        sclient.send("Another simple message");
        sclient.send(">>>>>>>>>>>>>> #!3213jidsa j09213421562dêewqewqe/SAD");
        sclient.send(">ksksksks                         000 1");
        sclient.send("               |          000 1");
    }

    serverThread.join();

    return 0;
}

#endif