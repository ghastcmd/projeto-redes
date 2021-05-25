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
        sock.send("simple message");
        sock.send("Another simple message");
        sock.send(">>>>>>>>>>>>>> #!3213jidsa j09213421562dêewqewqe/SAD");
        sock.send(">ksksksks                         000 1");
        sock.send("               |          000 1");
        sock.send("close");
    }

    sock.send("Thing to write");
    sock.send("close");
}

std::vector<std::shared_ptr<conn::sock>> client_sockets;

void client_handle(size_t index)
{
    using namespace conn;
    const auto &csock = *client_sockets[index];
    
    constexpr size_t packet_size = 25;
    char msg[packet_size] {0};
    csock.send("Established connection...\n");
    while (1)
    {
        int lenght = csock.recv(msg, packet_size);
        if (lenght <= 0) break;
        msg[--lenght] = '\0';
        
        if (!strncmp(msg, "close", sizeof("close")))
        {
            break;
        }

        std::cout << msg << '\n';
    }
}

void simple_func(size_t index)
{
    std::cout << client_sockets[index]->get_fd() << ' ' << index << '\n';
    client_sockets[index]->send("This is some message\n");
}

void server_func()
{
    conn::server server_instance(2222);
    puts("Just started the server function");
    
    constexpr int no_connections = 1;

    server_instance.bind();
    server_instance.listen(no_connections);

    std::vector<std::thread> threads;

    for (int i = 0; i < no_connections; i++)
    {
        auto sock_fd = server_instance.accept();

        client_sockets.emplace_back(std::make_shared<conn::sock>(sock_fd));
        threads.emplace_back(std::thread(client_handle, i));
    }

    for (auto &val: threads)
    {
        val.join();
    }
}

#if !defined(TEST)

int main(int argc, char *argv[])
{
    if (argc > 1 && !strcmp(argv[1], "client"))
    {
        client_func();
    }
    else
    {
        server_func();
    }

    return 0;
}

#endif