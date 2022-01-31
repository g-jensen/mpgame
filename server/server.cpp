#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <thread>

typedef struct Trimmed_Player {
    char name[32];
    sf::Vector2f position;
    int id;
} Trimmed_Player;

typedef struct Connection {
    sf::TcpSocket socket;
    Trimmed_Player player;
} Connection;

std::string currentTime() {
    return std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
}

void printPlayer(Trimmed_Player player) {
    std::cout << "\n[" << currentTime() << "]\n" << "name: " << player.name << "\nid: " << player.id << "\npos: " << "{" << player.position.x << ", " << player.position.y << "}\n" << std::endl;
}

Trimmed_Player player1;
Trimmed_Player player2;

sf::TcpSocket connection1;
sf::Socket::Status status1;

sf::TcpSocket connection2;
sf::Socket::Status status2;

int main () {
    sf::TcpListener listener;

    status1 = sf::Socket::NotReady;
    status2 = sf::Socket::NotReady;

    // bind the listener to a port
    if (listener.listen(53001) != sf::Socket::Done) {
        printf("[%s] socket failed to listen on port 53001\n",currentTime().c_str());
        return 1;
    } else {
        printf("[%s] Listening on port 53001\n", currentTime().c_str());
    }

    // get first connection
    if (listener.accept(connection1) != sf::Socket::Done) {
        printf("[%s] socket failed to accept client\n",currentTime().c_str());
        return 1;
    } else {
        printf("[%s] client connected\n",currentTime().c_str());
        sf::Packet packet;
        status1 = connection1.receive(packet);
        Trimmed_Player player = *(Trimmed_Player*)packet.getData();
        player.id = 0;
        player1 = player;
    }

    // get second connection
    if (listener.accept(connection2) != sf::Socket::Done) {
        printf("[%s] socket failed to accept client\n", currentTime().c_str());
        return 1;
    } else {
        printf("[%s] client connected\n",currentTime().c_str());
        sf::Packet packet;
        status2 = connection2.receive(packet);
        Trimmed_Player player = *(Trimmed_Player*)packet.getData();
        player.id = 1;
        player2 = player;
    }

    printPlayer(player1);
    printPlayer(player2);

    std::thread thread1([]() {
        while (true) {
            if (status1 != sf::Socket::Status::NotReady) {
                sf::Packet recPacket;
                connection1.receive(recPacket);
                player1 = *(Trimmed_Player*)recPacket.getData();

                sf::Packet respPacket;
                respPacket.append((void*)&player1,sizeof(Trimmed_Player));
                // update player2 on player1's object 
                connection2.send(respPacket);
            }
        }
    });

    std::thread thread2([]() {
        while (true) {
            if (status2 != sf::Socket::Status::NotReady) {
                sf::Packet packet;
                connection2.receive(packet);
                player2 = *(Trimmed_Player*)packet.getData();

                sf::Packet respPacket;
                respPacket.append((void*)&player2,sizeof(Trimmed_Player));
                // update player1 on player2's object 
                connection1.send(respPacket);
            }
        }
    });

    thread1.join();
    thread2.join();

    // use "client" to communicate with the connected client,
    // and continue to accept new connections with the listener
}
