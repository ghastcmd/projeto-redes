---
title: Relatório da aplicação
subtitle: Relatório de explicação e documentação da aplicação
author: |
    | Eduardo Brasil Araujo
    | Cesar Henrique Cícero
geometry: margin=4cm 
---

# Como Executar a Aplicação

## O funcionamento do jogo de Poker

Ao instanciar o objeto poker, é passado o número de jogadores que participarão do jogo. Para cada jogador é pedido o nome para identificação e é atribuído um saldo de R$ 200.000,00, além de um objeto `deck` com 52 `cartas` (ambos classes no programa).

O primeiro é jogador é escolhido para ser o *small Blind*[^small_blind]. Cada partida é iniciada com o método `newGame()`, e, inicialmente, todas as cartas volta para o baralho que é embaralhado e os jogadores que estão falidos (com `dinheiro == 0`) são expulsos da mesa e o *pot*[^pot] da mesa é iniciado com 0.

[^small_blind]: Fica localizado à esquerda da pessoa que dá as cartas, é o jogador que dá a menor aposta da mesa.
[^pot]: O pot é onde ficam armazenadas as apostas de todos os jogadores da mesa.

A mesa recebe 5 cartas aleatórias, sem repetição, e cada jogador recebe 2 (duas) cartas cada. Cada jogador pode ver as próprias cartas e, inicialmente, apenas as 3 primeiras cartas da mesa.

O poker consiste em 3 rodadas de apostas. Na primeira, o *small Blind* é obrigado à apostar metade da aposta mínima, e o próximo jogador, em sequência, é obrigado a apostar a aposta mínima; todas as apostas vão para o *pot*. Após a aposta inicial, para cada jogador é oferecido 3 opções: Fold (abandonar a partida), Call (cobrir as apostas ateriores) e Raise (aumentar a aposta pela quantidade que será especificada pelo jogador, ou zero para apostar tudo). Caso os outros jogadores abandonem a partida, o jogador que permanecer vence a partida e recebe todo o pot. Depois que todos fizerem suas apostas, 1 nova carta da mesa é revelada e outra rodada de apostas segue.

Após a última carta da mesa ser revelada, e a última rodada de apostas ocorrer, todos os jogadores que continuarem na partida revelam suas mãos e o jogador que conseguir formar com a mesa a melhor mão (maior rank) vence a partida e recebe todo o dinheiro no *pot*; caso haja empate, o *pot* é dividido.

Para cada nova rodada é chamada a função de membro `newGame()` no objeto `Poker`.
