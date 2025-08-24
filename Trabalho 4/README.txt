Comando que uso pra compilar em minha máquina.

- g++ -IC:\SFML-2.5.1\include -c pacman.cpp -o pacman.o

- g++ -IC:\SFML-2.5.1\include -LC:\SFML-2.5.1\lib Pacman.o  -o app.exe -lmingw32 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-main -mwindows