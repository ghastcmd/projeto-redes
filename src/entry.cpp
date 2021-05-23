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

void client_handle(conn::socket_t sock_fd)
{
    constexpr size_t packet_size = 25;
    conn::sock sock {sock_fd};
    char msg[packet_size] {0};
    snprintf(msg, packet_size, ">> %lu\n", (unsigned long)sock.get_fd());
    sock.send(msg);
    // std::string msg;
    // msg.reserve(packet_size);
    sock.send("Entered the client handle func\n");
    sock.send("This is another line i'm writing\n");
    while (1)
    {
        sock.send("Inside the while loop\n");
        int lenght = sock.recv(msg, packet_size);
        printf("lenght: %i\n", lenght);
        if (lenght == -1)
        {
            conn::basic_socket::print_errorg("Socket connection failed");
        }
        if (lenght <= 0) break;
        msg[--lenght] = '\0';
        sock.send(msg);

        if (!strncmp(msg, "close", sizeof("close")))
        {
            break;
        }

        puts("before putting message to screen");
        puts(msg);
    }
}

std::vector<std::thread> thread_client_pool;

void server_func()
{
    conn::server server_instance(2222);
    puts("Just started the server function");
    
    server_instance.bind();
    server_instance.listen(SOMAXCONN);

    char msg[20] {0};
    for (int i = 0 ; i < 1; ++i)
    {
        conn::sock sock = server_instance.accept();
        std::cout << sock.get_fd() << '\n';
        sock.send("The connection was accepted...\n");
        thread_client_pool.emplace_back(client_handle, sock.get_fd());
        sock.recv(msg, 20);
    }

    for (auto &thread: thread_client_pool)
    {
        thread.join();
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