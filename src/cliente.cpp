#include "pch.hpp"
#include "socket.hpp"

void dump_str(const char* str)
{
    char c;
    while ((c = *str++))
    {
        printf("%02x ", c);
    }
    putchar('\n');
}

void client_fn() {
    conn::client sclient("127.0.0.1", 2222);
    auto sock = sclient.connect();
    constexpr size_t packet_size = 2550;
    char msg[packet_size] {0};
    int lenght = sock.recv(msg, packet_size);
    msg[--lenght] = '\0';
    std::cout << msg << '\n';
    std::string name, s;
    std::cin >> name;
    sock.send(name.c_str());
    while(true){
        lenght = sock.recv(msg, packet_size);
        if (lenght <= 0) 
            break;
        msg[--lenght] = '\0';
        if (!strncmp(msg + lenght - sizeof("close") + 1, "close", sizeof("close")))
            break;
                
        if (strncmp(msg + lenght - sizeof("recv") + 1, "recv", sizeof("recv")))
            std::cout << msg << "\n";
        else{
            int i;
            do{
                std::cin >> s;
                i = stoi(s);
            }while(i < 0 || i > 2);
            sock.send(std::to_string(i).c_str());
            if(i == 2){
                int lenght = sock.recv(msg, packet_size);
                if (lenght <= 0) 
                    break;
                msg[--lenght] = '\0';
                do{
                    std::cout << msg;
                    std::cin >> s;
                    sock.send(s.c_str());
                    lenght = sock.recv(msg, packet_size);
                    if (lenght <= 0) 
                    break;
                    msg[--lenght] = '\0';
                }while(strncmp(msg + lenght - sizeof("ok") + 1, "ok", sizeof("ok")));
            }
        }
    }
}
