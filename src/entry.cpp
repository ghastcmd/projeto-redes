#include "pch.hpp"
#include "socket.hpp"
#include "poker.hpp"

#if !defined(CLIENT)

int main(int argc, char *argv[])
{
    puts("Just started the server side...");
    int num_players = 2;
    if (argc == 2)
    {
        num_players = atoi(argv[1]);
    }
    srand(time(nullptr));
    Poker jogo(num_players);
    jogo.newGame();
    jogo.newGame();
    jogo.newGame();
}

#else

extern void client_fn();

int main()
{
    puts("Just started the client side...");
    client_fn();
}

#endif