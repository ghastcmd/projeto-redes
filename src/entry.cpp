#include "pch.hpp"
#include "socket.hpp"
#include "poker.hpp"

void client_func()
{
    conn::client sclient("127.0.0.1", 2222);

    puts("Just initialized the client function");

    if (sclient.connect())
    {
        puts("Sending 6 different messages to server");
        sclient.send("simple message");
        sclient.send("Another simple message");
        sclient.send(">>>>>>>>>>>>>> #!3213jidsa j09213421562dêewqewqe/SAD");
        sclient.send(">ksksksks                         000 1");
        sclient.send("               |          000 1");
        sclient.send("close");
    }

    sclient.send("Thing to write");
    sclient.send("close");
}

void server_func()
{
    conn::server serverInstance(2222);
    puts("Just started the server function");
    
    serverInstance.bind();
    serverInstance.listen(1);
    std::string msg;
    msg.reserve(24);
    auto sock = serverInstance.accept();
    while (1)
    {
        int lenght = serverInstance.recv(sock, &msg[0], 23);
        msg[lenght] = '\0';
        serverInstance.send(msg.c_str());

        if (msg == "close")
        {
            break;
        }
        puts(msg.c_str());
    }
}

#if !defined(TEST)

int main(int argc, char *argv[])
{
    if (argc > 1 && !strcmp(argv[1], "server"))
    {
        server_func();
    }
    else
    {
        client_func();
    }

    return 0;
}

#endif