g++ -c server.cpp
g++ server.o -o server -pthread -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system
rm server.o
