# projeto-redes
Um jogo de poker através da rede, utilizando-se de sockets primitivos, com as funções socket(), send() e recv().

O programa foi escrito em c++ e o sistema de o build system é escrito em Makefile.

Um arquivo [relatório](relatorio.md) está escrito em markdown e atende aos requisitos dos relatório do projeto.

* Relatório com no máximo duas páginas.
    * Principais funcionalidades
    * O que poderia ter sido implementado a mais
    * Dificuldades encontradas no desenvolvimento do projeto
    * Não adicionar qualquer screenshot no documento
    * Instruções de como executar a aplicação

 - [ ] Relatório com as principais funcionalidades (max 2 pág.)
 - [ ] Uso de Threads

funcionamento do jogo de poker:
Ao instanciar o objeto Poker, é passado o numero de jogadores que participarão no jogo, para cada jogador é pedido o nome deste para identificação, é atribuido um valor de 200000 R$ iniciais para cada jogador e um objeto 'deck' com 52 'cartas' são instanciados, o primeiro jogador é escolhido para ser o small Blind. Cada partida é iniciada com o método newGame(), inicialmente todas as carttas voltam para o baralho que é embaralhado, e os jogadores que estão falidos (dinheiro == 0) são expulsos da mesa e o pot da mesa é iniciado com 0. A mesa recebe 5 cartas aleatórias (sem repetição) e cada rogador recebe 2 cartas cada. Cada jogador pode ver as próprias carta e inicialmente apenas as 3 primeiras cartas da mesa.
O poker consiste de 3 rodadas de apostas, na primeira rodada o rogador que é o small Blind é obrigado à apostar metade da aposta mínima e o próximo jogador em sequencia é obrigado a apostar a aposta mínima, todas as apostas vão para o pot, após essa aposta inicial, para cada jogador é oferecido 3 opções Fold (abandonar a partida), Call (cobrir as apostas anteriores) e Raise (o jogador irá dizer qual o valor da nova aposta que deve ser maior que a aposta anterior (0 = apostar tudo (All-In))). Caso todos os outros jogadores abandonem a partida, o jogador que permanecer vence a partida e recebe todo o pot, depois que todos os jogadores fizerem suas apostas 1 nova carta da mesa é revelada e outra rodada de apostas segue. Após a ultima carta da mesa ser revelada e a ultima rodada de apostas ocorrer todos os jogadores que continuarem na partida revelam suas mãos e o jogador que conseguir formar com a mesa a melhor mão (maior rank) vence a partida e recebe todo o dinheiro do pot (caso haja empate o pot é dividido).
Para ter uma nova rodada basta chamar nvamente o método newGame()

* [Sockets on linux](https://linux.die.net/man/7/socket)
* [Get socket Error String Windows](https://stackoverflow.com/questions/3400922/how-do-i-retrieve-an-error-string-from-wsagetlasterror)
* [Basic implementation of sockets server in Java](https://www.geeksforgeeks.org/multithreaded-servers-in-java/)
