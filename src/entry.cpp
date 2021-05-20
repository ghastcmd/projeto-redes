#include "pch.hpp"
#include "socket.hpp"
#include "poker.hpp"

void dump_str(const char* str)
{
    while (char c = *str++)
        printf("%02x ", c);
    putchar('\n');
}

void client_func()
{
    conn::client sclient("127.0.0.1", 2222);

    puts("Just initialized the client function");

    auto sock = sclient.connect();
    if (sock)
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
    constexpr size_t packet_size = 25;
    msg.reserve(packet_size);
    auto sock = serverInstance.accept();
    while (1)
    {
        int lenght = serverInstance.recv(sock, &msg[0], packet_size);
        if (lenght == 0) break;
        msg[--lenght] = '\0';
        // serverInstance.send(msg.c_str());

        if (!strncmp(msg.c_str(), "close", sizeof("close")))
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