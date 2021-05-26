#include "socket.cpp"

int main() {
    conn::client sclient("127.0.0.1", 2222);
    auto sock = sclient.connect();
    constexpr size_t packet_size = 255;
    char msg[packet_size] {0};
    int lenght = sock.recv(msg, packet_size);
    msg[--lenght] = '\0';
    std::cout << msg << '\n';
    std::string name, s;
    std::cin >> name;
    sock.send(name.c_str());
    while(true){
      int lenght = sock.recv(msg, packet_size);
      if (lenght <= 0) 
        break;
      msg[--lenght] = '\0';
      if (!strncmp(msg, "close", sizeof("close")))
        break;
      if (strncmp(msg, "recv", sizeof("recv")))
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
            int lenght = sock.recv(msg, packet_size);
            if (lenght <= 0) 
              break;
            msg[--lenght] = '\0';
          }while(strncmp(msg, "ok", sizeof("ok")));
        }
      }
    }
}
