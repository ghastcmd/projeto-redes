# projeto-redes
Um jogo de poker através da rede, utilizando-se de sockets primitivos, como as funções socket(), send() e recv().

O programa foi escrito em c++ e o build system é escrito em Makefile. É necessário que o sistema de compilação da aplicação seja o linux que e este possua o gcc e o Makefile instalado.

Um arquivo [relatório](relatorio.md) está escrito em markdown e atende aos requisitos dos relatório do projeto.

O arquivo relatório também foi transformado para pdf e está presente no diretório raiz da aplicação. No script Makefile contém uma tag de comando que transforma o arquivo de markdown para pdf, com o comand `make relatorio`. Porém, também é disponibilizado o arquivo em pdf previamente transformado [aqui](relatorio.pdf).

* Relatório com no máximo duas páginas.
    * Principais funcionalidades
    * O que poderia ter sido implementado a mais
    * Dificuldades encontradas no desenvolvimento do projeto
    * Não adicionar qualquer screenshot no documento
    * Instruções de como executar a aplicação

 - [x] Uso da tecnologia de sockets para a comunicação em rede
 - [x] Relatório com as principais funcionalidades (max 2 pág.)
 - [x] Uso de Threads


* [Sockets on linux](https://linux.die.net/man/7/socket)
* [Get socket Error String Windows](https://stackoverflow.com/questions/3400922/how-do-i-retrieve-an-error-string-from-wsagetlasterror)
* [Basic implementation of sockets server in Java](https://www.geeksforgeeks.org/multithreaded-servers-in-java/)
