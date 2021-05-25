#include "pch.hpp"
#include "poker.hpp"
#include "tables.h"

using namespace std;

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

card::card(int numero){
    n = numero;
    b = n >> 2;
    s = n & 0x3;
    carta = numeros[b] + suitsChar[s];
    suit_hash = suitbit_by_id[n];
    binaryId = binaries_by_id[n];
}

Deck::Deck(){
    int i;
    for(i = 0; i < numCards; i++){
        cards[i] = new card(i);
        chosed[i] = false;
    }
}

void Deck::reshuffle(){
    int i;
    for(i = 0; i < numCards; i++)
        chosed[i] = false;
}

card* Deck::pick(){
    int i;
    do{
        i = rand() % numCards;
    } while(chosed[i]);
    chosed[i] = true;
    return cards[i];
}

Deck::~Deck(){
    int i;
    for(i = 0; i < numCards; i++){
        delete cards[i];
    }
}

Hand::Hand(){
    hand = NULL;
    last = NULL;
    tam = 0;
}

void Hand::pick(Deck *d, int n){
    while(n--)
        add(d->pick());
}

void Hand::add(card *c){
    listCards *novo = new listCards;
    novo->c = c;
    if(hand == NULL)
        hand = novo;
    else
        last->next = novo;
    last = novo;
    tam++;
}

void Hand::discarAll(){
    listCards *aux = hand;
    while(aux != NULL){
        hand = hand->next;
        delete aux;
        aux = hand;
    }
    hand = NULL;
    tam = 0;
}

int Hand::evaluate(){
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

Hand * Hand::operator+(const Hand& h2){
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

string Hand::printHand(){
    int i = 0;
    string s = "";
    listCards *aux = hand;
    while(aux != NULL){
        s += aux->c->carta;
        i++;
        aux = aux->next;
        if(i == tam)
            break;
        s += " | ";
    }
    s += "\n";
    return s;
}

Hand::~Hand(){
    discarAll();
}

Jogador::Jogador(int p){
    porta = p;
    dinheiro = 20000;
}

void perguntarNome(conn::socket_t sock_fd, Jogador* jog){
    conn::sock csock(sock_fd);
    csock.send("Olá jogador qual o seu nome?\n");
    constexpr size_t packet_size = 25;
    char msg[packet_size] {0};
    int lenght;
    do{
      lenght = csock.recv(msg, packet_size); 
    }while(lenght <= 0);
    msg[--lenght] = '\0';
    jog->name = string(msg);
    csock.send("Bem vindo ao jogo, você recebeu 20000 R$\n");
    csock.send((to_string(jog->porta) + "\n").c_str());
}

string Jogador::getName(){
    return name + "(" + to_string(dinheiro) + ")";
}

void Jogador::deal(Deck *deck){
    hand.pick(deck, 2);
}

string  Jogador::printHand(){
    return hand.printHand();
}

void Jogador::checkHand(Hand *mesa){
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

jogadas Jogador::acao(){
    conn::server server_instance(porta);
    server_instance.bind();
    server_instance.listen(1);
    conn::socket_t sock_fd = server_instance.accept();
    conn::sock csock(sock_fd);
    if(dinheiro == 0){
      csock.send("Você não possui mais dinheiro para apostar (CALL)\n");
      return CALL;
    }
    int i;
    csock.send("Escolha ação você vai fazer (0 - FOLD, 1 - CALL; 2 - RAISE):)");
    constexpr size_t packet_size = 25;
    char msg[packet_size] {0};
    csock.recv(msg, packet_size);
    if(msg[0] == '0')
        return FOLD;
    if(msg[0] == '2')
        return RAISE;
    return CALL;
}

void Jogador::add(int pot){
    dinheiro += pot;
}

void Jogador::clearHand(){
    hand.discarAll();
}

int Jogador::raise(int *oldBet){
    int newBet;
    conn::server server_instance(porta);
    server_instance.bind();
    server_instance.listen(1);
    conn::socket_t sock_fd = server_instance.accept();
    conn::sock csock(sock_fd);
    constexpr size_t packet_size = 25;
    char msg[packet_size] {0};
    do{
        csock.send(("A aposta atual é de " + to_string(*oldBet) + ". Qual o valor da nova aposta? (0 == All-In): ").c_str());
        csock.recv(msg, packet_size);
        newBet = stoi(string(msg));
        if(newBet == 0)
            newBet = dinheiro + pot;
    }while(newBet <= *oldBet || (newBet - pot) > dinheiro);
    *oldBet = newBet;
    return bet(*oldBet);
}

int Jogador::bet(int b){
    int act_bet = (b - pot);
    if(dinheiro < act_bet)
        act_bet = dinheiro;
    dinheiro -= act_bet;
    pot += act_bet;
    return act_bet;
}

bool Jogador::isBankrupt(){
return dinheiro == 0;
}

bool Jogador::operator>(const Jogador& j){
    return rank < j.rank;
}

bool Jogador::operator==(const Jogador& j){
    return rank == j.rank;
}

Poker::Poker(int n){
    deck = new Deck();
    nJog = 0;
    mesaJog = NULL;
    conn::server server_instance(2222);
    server_instance.bind();
    server_instance.listen(n);
    vector<thread> threads;
    while(n--){
      conn::socket_t sock_fd = server_instance.accept();
      Jogador* jog = addPlayer();
      threads.push_back(thread(perguntarNome, sock_fd, jog));
    }
    
    for (auto &val: threads)
    {
        val.join();
    }

    smallBlind = mesaJog;
}

void printHand(conn::socket_t sock_fd, string s, Jogador* jog, bool first){
    conn::sock csock(sock_fd);
    csock.send(s.c_str());
    if(first){
      csock.send(("Sua mão:\n" + jog->printHand() + "\n").c_str());
      csock.send(("Você possui: " + string(jog->rankDesc) + "\n").c_str());
    }
}

void Poker::cast(string s, bool first = false){
  Jogadores *aux = nextJog(mesaJog->prev);
  Jogadores *stop = nextJog(mesaJog->prev);
  if(!first)
    stop = mesaJog;
  vector<thread> threads;
  do{
    conn::server server_instance(aux->jog->porta);
    server_instance.bind();
    server_instance.listen(1);
    conn::socket_t sock_fd = server_instance.accept();
    if(first){
      aux->jog->checkHand(&mesa);
      aux->jog->pot = 0;
    }
    threads.push_back(thread(printHand, sock_fd, s, aux->jog, first));
    if(first)
      aux = nextJog(aux);
    else
      aux = aux->next;
  }while(aux != stop);
  for (auto &val: threads)
    {
      val.join();
    }
}

bool Poker::rodada(int blind, int *totPot, bool first){
    int pot = 0;
    string s = "Cartas da Mesa:\n" + mesa.printHand() + "\n";
    cast(s, true);
    int bet = blind;
    if(first){
        turn = smallBlind;
        smallBlind = smallBlind->next;
        pot += turn->jog->bet(bet / 2);
        cast("Jogador " + turn->jog->getName() + " apostou " + to_string(bet / 2) + "\n");
        turn = nextJog(turn);
        pot += turn->jog->bet(bet);
        cast("Jogador " + turn->jog->getName() + " apostou " + to_string(bet) + "\n");
    }
    Jogadores *raisePlayer = turn;
    if(first)
        turn = nextJog(turn);
    do{
        Jogadores *nextJogador = nextJog(turn);
        if(nextJogador == turn){
            cast("Jogador " + turn->jog->getName() + " ganhou por ser o único jogador na partida\n");
            turn->jog->add(*totPot);
            return true;
        }
        jogadas j = turn->jog->acao();
        if(j == CALL){
            pot += turn->jog->bet(bet);
            cast("Jogador " + turn->jog->getName() + " apostou " + to_string(bet) + "\n");
        }else if(j == FOLD){
            cast("Jogador " + turn->jog->getName() + " apandonou essa partida!\n");
            turn->inGame = false;
        }else{
            pot += turn->jog->raise(&bet);
            raisePlayer = turn;
            cast("Jogador " + turn->jog->getName() + " aumentou a aposta para " + to_string(bet) + "\n");
        }
        turn = nextJogador;
    }while(turn != raisePlayer);
    *totPot += pot;
    return false;
}

void Poker::checkWinner(int totPot){
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
    string s = "";
    if(draw != 1)
        s += "Empate:\n";
    totPot /= draw;
    aux = winner;
    do{
        if(*(aux->jog) == *(winner->jog)){
            s += "Jogador " + aux->jog->getName() + " ganhou com " + aux->jog->dscRankDesc + "\n";
            aux->jog->add(totPot);
        }
        aux = nextJog(aux);
    }while(aux != winner);
    s += "Valor recebido por cada vencedor :" + to_string(totPot);
    cast(s);
}

Jogador* Poker::addPlayer(){
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
    aux->jog = new Jogador(portas[nJog]);
    aux->inGame = true;
    nJog++;
    return aux->jog;
}

void Poker::dealer(){
    Jogadores *aux = mesaJog;
    deck->reshuffle();
    do{
        if(aux->jog->isBankrupt()){
            Jogadores *auxDel = aux;
            if(auxDel == smallBlind)
              smallBlind = smallBlind->next;
            aux = aux->next;
            aux->prev = auxDel->prev;
            aux->prev->next = aux;
            cast("Jogador " + auxDel->jog->name + " faliu e será desconectado...\n");
            delete auxDel->jog;
            delete auxDel;
        }else{
            aux->jog->clearHand();
            aux->inGame = true;
            aux->jog->deal(deck);
            aux = aux->next;
        }
    }while(aux != mesaJog);
    mesa.discarAll();
    mesa.pick(deck, 3);
}

Jogadores * Poker::nextJog(Jogadores *turn){
    do{
        turn = turn->next;
    }while(! turn->inGame);
    return turn;
}

void Poker::newGame(){
    dealer();
    int totPot = 0;
    if(rodada(200, &totPot, true))
        return;
    mesa.pick(deck);
    if(rodada(200, &totPot))
        return;
    mesa.pick(deck);
    if(rodada(200, &totPot))
        return;
    checkWinner(totPot);
}

Poker::~Poker(){
    delete deck;
    mesaJog->prev->next = NULL;
    while(mesaJog != NULL){
        smallBlind = mesaJog;
        mesaJog = mesaJog->next;
        delete smallBlind->jog;
        delete smallBlind;
    }
}
