#include "pch.hpp"
#include "poker.hpp"

using namespace std;

const string suits[4] = {"\u2660", "\u2661", "\u2662", "\u2663"};
const string numeros[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

card::card(int n){
    numero = n % 13;
    suit = n / 13;
    carta = numeros[numero] + suits[suit];
}

bool card::operator<(const card& c2){
    return numero < c2.numero;
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
    for(i = 0; i < numCards; i++){
        chosed[i] = false;
    }
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

Hand::Hand(bool ordenado = false){
    ord = ordenado;
    hand = NULL;
    tam = 0;
}

void Hand::revN(int n){
    tam = n;
}

void Hand::pick(Deck *d, int n = 1){
    while(n--)
    add(d->pick());
}

void Hand::add(card *c){
    listCards *novo = new listCards;
    novo->c = c;
    if(!ord || hand == NULL || (hand != NULL && *c < *(hand->c))){
    novo->next = hand;
    hand = novo;
    }else{
    listCards *aux = hand;
    while(aux->next != NULL && *(aux->next->c) < *c){
        aux = aux->next;
    }
    novo->next = aux->next;
    aux->next = novo;
    }
    tam++;
}

void Hand::discard(int j){
    j--;
    int i = 0;
    listCards *aux = hand;
    if(j == 0){
        hand = hand->next;
        delete aux;
    }else{
        while(aux != NULL){
            i++;
            if(i == j)
            break;
            aux = aux->next;
        }
        if(aux != NULL){
            listCards *aux2 = aux->next;
            if(aux2 != NULL){
                aux->next = aux2->next;
                delete aux2; 
            }
        }
    }
    tam--;
}

void Hand::discarAll(){
    listCards *aux = hand;
    while(aux != NULL){
        hand = hand->next;
        delete aux;
        aux = hand;
    }
    tam = 0;
}

Hand * Hand::operator+(const Hand& h2){
    int i = 0;
    Hand *newHand = new Hand(true);
    listCards *aux = hand;
    while(aux != NULL){
        newHand->add(aux->c);
        aux = aux->next;
    }
    aux = h2.hand;
    while(aux != NULL){
        newHand->add(aux->c);
        i++;
        if(i == h2.tam)
            break;
        aux = aux->next;
    }
    return newHand;
}

void Hand::printHand(){
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

Hand::~Hand(){
    discarAll();
}

Poker::Poker(){
    deck = new Deck();
}

void Poker::newGame(int n){
    deck->reshuffle();
    jogadores = new Hand[n];
    mesa.pick(deck, 5);
    int i;
    for(i = 0; i < n; i++){
        cout << "Jogador 0" << (i + 1) << ":" << endl;
        jogadores[i].pick(deck, 2);
        jogadores[i].printHand();
    }
    mesa.revN(3);
    cout << "Mesa:" << endl;
    mesa.printHand();
    mesa.discarAll();
    delete [] jogadores;
}

Poker::~Poker(){
    delete deck;
}

#if defined(POKER_TEST)

int main() {
    srand(time(NULL));
    int i;
    Poker p;
    p.newGame(5);
    p.newGame(5);
}

#endif
