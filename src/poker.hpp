#pragma once
#include "pch.hpp"
#include "tables.h"

using namespace std;

constexpr int numCards = 52;

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

const char* rank_category_description[] = {
  "",
  "Straight Flush",
  "Four of a Kind",
  "Full House",
  "Flush",
  "Straight",
  "Three of a Kind",
  "Two Pair",
  "One Pair",
  "High Card",
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

    void printHand();

    ~Hand();
};

class Jogador{
  private:
    int dinheiro;
    Hand hand;
    int rank;
    string name;
  public:
    int pot;
    const char* rankDesc;
    const char* dscRankDesc;
    Jogador();

    string getName();

    void deal(Deck *deck);

    void printHand();

    void checkHand(Hand *mesa);

    jogadas acao();

    void add(int pot);

    void clearHand();

    int raise(int &oldBet);

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

    bool rodada(int blind, int &totPot, bool first = false);

    void checkWinner(int totPot);

    void addPlayer();

    void dealer();

    Jogadores *nextJog(Jogadores *turn);
  public:
    Poker(int n);

    void newGame();

    ~Poker();
};
