mkdir bin\deps
g++ -c src/entry.cpp -o bin/entry.o
g++ -c src/poker.cpp -o bin/poker.o
g++ -c src/pokerSemCab.cpp -o bin/pokerSemCab.o
g++ -c src/sockets.cpp -o bin/sockets.o
g++ bin/entry.o bin/poker.o bin/pokerSemCab.o bin/sockets.o -o bin.exe -lWs2_32