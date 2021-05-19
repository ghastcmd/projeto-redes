#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <bitset>
#include "tables.h"

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
    int suit_hash;
    int binaryId;
    int n;
    int b, s;
    string carta;
    card(int numero){
      n = numero;
      b = n >> 2;
      s = n & 0x3;
      carta = numeros[b] + suitsChar[s];
      suit_hash = suitbit_by_id[n];
      binaryId = binaries_by_id[n];
    }
};

class Deck{
  private:
    card *cards[numCards];
    bool chosed[numCards];
  public:
    Deck(){
      int i;
      for(i = 0; i < numCards; i++){
        cards[i] = new card(i);
        chosed[i] = false;
      }
    }

    void reshuffle(){
      int i;
      for(i = 0; i < numCards; i++){
        chosed[i] = false;
      }
    }

    card* pick(){
      int i;
      do{
        i = rand() % numCards;
      }while(chosed[i]);
      chosed[i] = true;
      return cards[i];
    }

    ~Deck(){
      int i;
      for(i = 0; i < numCards; i++){
        delete cards[i];
      }
    }
};

class Hand{
  private:
    typedef struct listCards{
      card *c;
      listCards *next;
    }listCards;
    int tam;
    listCards *hand, *last;
    void add(card *c){
      listCards *novo = new listCards;
      novo->c = c;
      if(hand == NULL)
        hand = novo;
      else
        last->next = novo;
      last = novo;
      tam++;
    }
  public:
    Hand(){
      hand = NULL;
      last = NULL;
      tam = 0;
    }

    void pick(Deck *d, int n = 1){
      while(n--)
        add(d->pick());
    }

    void discarAll(){
      listCards *aux = hand;
      while(aux != NULL){
        hand = hand->next;
        delete aux;
        aux = hand;
      }
      hand = NULL;
      tam = 0;
    }

    int evaluate(){
      int suit_hash = 0;
      listCards *aux = hand;
      while(aux != NULL){
        suit_hash += aux->c->suit_hash;
        aux = aux->next;
      }
      int s = suits[suit_hash] - 1;
      if(s > 0){
        int suit_binary = 0;
        aux = hand;
        while(aux != NULL){
          if(s == aux->c->s)
            suit_binary |= aux->c->binaryId;
          aux = aux->next;
        }
        return nflush[suit_binary];
      }
      unsigned char quinary[13] = {0};
      aux = hand;
      while(aux != NULL){
        quinary[aux->c->b]++;
        aux = aux->next;
      }
      int sum = 0, i, k = tam;
      for(i = 0; i < 13; i++){
        sum += dp[quinary[i]][12-i][k];
        k -= quinary[i];
        if(k <= 0)
          break;
      }
        
      if(tam == 7)
        return noflush7[sum];
      if(tam == 6)
        return noflush6[sum];
      return noflush5[sum];
    }

    Hand *operator+(const Hand& h2){
      int i = 0;
      Hand *newHand = new Hand();
      listCards *aux = hand;
      while(aux != NULL){
        newHand->add(aux->c);
        if(++i == h2.tam)
          break;
        aux = aux->next;
      }
      aux = h2.hand;
      i = 0;
      while(aux != NULL){
        newHand->add(aux->c);
        if(++i == h2.tam)
          break;
        aux = aux->next;
      }
      return newHand;
    }

    void printHand(){
      int i = 0;
      listCards *aux = hand;
      while(aux != NULL){
        cout << aux->c->carta;
        i++;
        aux = aux->next;
        if(i == tam)
          break;
        cout << " | ";
      }
      cout << endl;
    }

    ~Hand(){
      discarAll();
    }
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
    Jogador(){
      dinheiro = 200000;
      cout << "Digite seu nome: ";
      cin >> name;
    }

    string getName(){
      return name + "(" + to_string(dinheiro) + ")";
    }

    void deal(Deck *deck){
      hand.pick(deck, 2);
    }

    void printHand(){
      hand.printHand();
    }

    void checkHand(Hand *mesa){
      Hand *bestHand = hand + *mesa;
      rank = bestHand->evaluate();
      rank_category r;
      if (rank > 6185) r = HIGH_CARD;// 1277 high card
      else if (rank > 3325) r = ONE_PAIR;// 2860 one pair
      else if (rank > 2467) r = TWO_PAIR;//  858 two pair
      else if (rank > 1609) r = THREE_OF_A_KIND;//  858 three-kind
      else if (rank > 1599) r = STRAIGHT;//   10 straights
      else if (rank > 322)  r = FLUSH;// 1277 flushes
      else if (rank > 166)  r = FULL_HOUSE;//  156 full house
      else if (rank > 10)   r = FOUR_OF_A_KIND;//  156 four-kind
      else r = STRAIGHT_FLUSH;
      delete bestHand;
      dscRankDesc = rank_description[rank][1];
      rankDesc = rank_category_description[r];
    }

    jogadas acao(){
      //{CALL, FOLD, RAISE};
      if(dinheiro == 0)
        return CALL;
      int i;
      cout << "Escolha ação jogador " << getName() << " (0 - FOLD, 1 - CALL; 2 - RAISE): ";
      cin >> i;
      if(i == 0)
        return FOLD;
      if(i == 2)
        return RAISE;
      return CALL;
    }

    void add(int pot){
      dinheiro += pot;
    }

    void clearHand(){
      hand.discarAll();
    }

    int raise(int &oldBet){
      int newBet;
      do{
        cout << "A aposta atual é de " << oldBet << ". Qual o valor da nova aposta? (0 == All-In): ";
        cin >> newBet;
        if(newBet == 0)
          newBet = dinheiro + pot;
      }while(newBet < oldBet || (newBet - pot) > dinheiro);
      oldBet = newBet;
      return bet(oldBet);
    }

    int bet(int b){
      int act_bet = (b - pot);
      if(dinheiro < act_bet)
        act_bet = dinheiro;
      dinheiro -= act_bet;
      pot += act_bet;
      cout << "Jogador " << getName() << " você apostou: " << act_bet << endl;
      return act_bet;
    }

    bool isBankrupt(){
      return dinheiro == 0;
    }

    bool operator>(const Jogador& j){
      return rank < j.rank;
    }

    bool operator==(const Jogador& j){
      return rank == j.rank;
    }

};

class Poker{
  private:
    Deck *deck;
    Hand mesa;
    int nJog;
    typedef struct Jogadores{
      Jogador* jog;
      Jogadores *next, *prev;
      bool inGame;
    }Jogadores;
    Jogadores *mesaJog, *smallBlind, *turn;
    bool rodada(int blind, int &totPot, bool first = false){
      int pot = 0;
      cout << "Mesa:" << endl;
      mesa.printHand();
      Jogadores *aux = nextJog(mesaJog->prev);
      do{
        cout << "Jogador " << aux->jog->getName() << ":" << endl;
        if(first)
          aux->jog->deal(deck);
        aux->jog->printHand();
        aux->jog->checkHand(&mesa);
        aux->jog->pot = 0;
        cout << "você tem um " << aux->jog->rankDesc << endl;
        aux = nextJog(aux);
      }while(aux != mesaJog);
      int bet = blind;
      if(first){
        turn = smallBlind;
        smallBlind = smallBlind->next;
        pot += turn->jog->bet(bet / 2);
        turn = nextJog(turn);
        pot += turn->jog->bet(bet);
      }
      Jogadores *raisePlayer = turn;
      if(first)
        turn = nextJog(turn);
      do{
        Jogadores *nextJogador = nextJog(turn);
        if(nextJogador == turn){
          cout << "Jogador " << turn->jog->getName() << " ganhou por ser o único jogador na partida" << endl;
          turn->jog->add(totPot);
          return true;
        }
        jogadas j = turn->jog->acao();
        if(j == CALL)
          pot += turn->jog->bet(bet);
        else if(j == FOLD){
          cout << "Jogador " << turn->jog->getName() << " apandonou esse jogo!" << endl;
          turn->inGame = false;
        }else{
          pot += turn->jog->raise(bet);
          raisePlayer = turn;
        }
        turn = nextJogador;
      }while(turn != raisePlayer);
      totPot += pot;
      return false;
    }

    void checkWinner(int totPot){
      int draw = 1;
      Jogadores* winner = mesaJog;
      Jogadores* aux = nextJog(mesaJog);
      while(aux != mesaJog){
        if(*(aux->jog) == *(winner->jog))
          draw++;
        else if(*(aux->jog) > *(winner->jog)){
          winner = aux;
          draw = 1;
        }
        aux = nextJog(aux);
      }
      if(draw != 1)
        cout << "Empate:" << endl;
      totPot /= draw;
      aux = winner;
      do{
        if(*(aux->jog) == *(winner->jog)){
          cout << "Jogador " << aux->jog->getName() << " ganhou com " << aux->jog->dscRankDesc << endl;
          aux->jog->add(totPot);
        }
        aux = nextJog(aux);
      }while(aux != winner);
    }

    void addPlayer(){
      Jogadores *aux = new Jogadores;
      if(mesaJog == NULL){
        mesaJog = aux;
        aux->prev = mesaJog;
      }else{
        aux->prev = mesaJog->prev;
        mesaJog->prev->next = aux;
        mesaJog->prev = aux;
      }
      aux->next = mesaJog;
      aux->jog = new Jogador();
      aux->inGame = true;
      nJog++;
    }

    void dealer(){
      Jogadores *aux = mesaJog;
      do{
        if(aux->jog->isBankrupt()){
          Jogadores *auxDel = aux;
          aux = aux->next;
          aux->prev = auxDel->prev;
          aux->prev->next = aux;
          cout << "Jogador " + auxDel->jog->getName() + " faliu e será desconectado..." << endl;
          delete auxDel->jog;
          delete auxDel;
        }else{
          aux->jog->clearHand();
          aux->inGame = true;
          aux = aux->next;
        }
      }while(aux != mesaJog);
      mesa.discarAll();
      deck->reshuffle();
    }

    Jogadores *nextJog(Jogadores *turn){
      do{
        turn = turn->next;
      }while(! turn->inGame);
      return turn;
    }
  public:
    Poker(int n){
      deck = new Deck();
      nJog = 0;
      mesaJog = NULL;
      while(n--)
        addPlayer();
      smallBlind = mesaJog;
    }

    void newGame(){
      dealer();
      int totPot = 0;
      mesa.pick(deck, 3);
      if(rodada(200, totPot, true))
        return;
      mesa.pick(deck);
      if(rodada(200, totPot))
        return;
      mesa.pick(deck);
      if(rodada(200, totPot))
        return;
      checkWinner(totPot);
    }

    ~Poker(){
      delete deck;
      mesaJog->prev->next = NULL;
      while(mesaJog != NULL){
        smallBlind = mesaJog;
        mesaJog = mesaJog->next;
        delete smallBlind->jog;
        delete smallBlind;
      }
    }
};

#if defined(POKER_CAB_TEST)

int main() {
  srand(time(NULL));
  int i;
  Poker p(4);
  p.newGame();
  p.newGame();
  p.newGame();
}

#endif