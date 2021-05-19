#pragma once
#include "pch.hpp"

using namespace std;

constexpr int numCards = 52;

class card{
    private:
        int numero, suit;
    public:
        string carta;
        card(int n);

        bool operator<(const card& c2);
};

typedef struct listCards{
    card *c;
    listCards *next;
}listCards;

class Deck{
    private:
        card *cards[numCards];
        bool chosed[numCards];
    public:
        Deck();

        void reshuffle();

        card* pick();

        ~Deck();
};

class Hand{
    private:
        int tam;
        bool ord;
        listCards *hand;
    public:
        Hand(bool ordenado = false);

        void revN(int n);

        void pick(Deck *d, int n);

        void add(card *c);

        void discard(int j);

        void discarAll();

        Hand* operator+(const Hand& h2);

        void printHand();

        ~Hand();
};

class Poker{
    private:
        Deck *deck;
        Hand *jogadores;
        Hand mesa;
    public:
        Poker();

        void newGame(int n);

        ~Poker();
};
