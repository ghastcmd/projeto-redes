#pragma once
#include "pch.hpp"
#include "socket.hpp"

using namespace std;

const int numCards = 52;
const string suitsChar[4] = {"\u2660", "\u2661", "\u2662", "\u2663"};
const string numeros[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

enum rank_category {
  STRAIGHT_FLUSH = 1,
  FOUR_OF_A_KIND,
  FULL_HOUSE,
  FLUSH,
  STRAIGHT,
  THREE_OF_A_KIND,
  TWO_PAIR,
  ONE_PAIR,
  HIGH_CARD,
};

enum jogadas {CALL, FOLD, RAISE};

class card{
  private:
  public:
    card(int n);
    int suit_hash;
    int binaryId;
    int n;
    int b, s;
    string carta;
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
    listCards *hand, *last;
    void add(card *c);
  public:
    Hand();

    void pick(Deck *d, int n = 1);

    void discarAll();

    int evaluate();

    Hand *operator+(const Hand& h2);

    string printHand();

    ~Hand();
};

class Jogador{
  private:
    int dinheiro;
    Hand hand;
    int rank;
  public:
    int porta;
    const size_t packet_size = 50;
    string name;
    int pot;
    const char* rankDesc;
    const char* dscRankDesc;
    Jogador(int p);

    string getName();

    void deal(Deck *deck);

    string printHand();

    void checkHand(Hand *mesa);

    jogadas acao();

    void add(int pot);

    void clearHand();

    int raise(int *oldBet);

    int bet(int b);

    bool isBankrupt();

    bool operator>(const Jogador& j);

    bool operator==(const Jogador& j);
};

typedef struct Jogadores{
  Jogador* jog;
  Jogadores *next, *prev;
  bool inGame;
}Jogadores;

class Poker{
  private:
    Deck *deck;
    Hand mesa;
    int nJog;
    Jogadores *mesaJog, *smallBlind, *turn;
    int portas[7] = {82, 83, 84, 85, 86, 87, 88};

    void cast(string s, bool first);

    bool rodada(int blind, int *totPot, bool first = false);

    void checkWinner(int totPot);

    Jogador* addPlayer();

    void dealer();

    Jogadores *nextJog(Jogadores *turn);
  public:
    Poker(int n);

    void newGame();

    ~Poker();
};
