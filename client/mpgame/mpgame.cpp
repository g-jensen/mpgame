#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <SFML/Network.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

#include "Game.h"

sf::TcpSocket socket;

int main()
{

    Game::state = State::MAINMENU;
    // create the window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "mpgame");
    ImGui::SFML::Init(window);

    sf::Font font;
    font.loadFromFile("UbuntuMono-Regular.ttf");
    sf::Text text;
    text.setFont(font);
    text.setPosition(0, 0);
    text.setString("default name");

    Game::client.player.label = text;
    
    text.setString("not loaded in yet");
    Game::player2.label = text;

    sf::Clock timeSinceTick;
    timeSinceTick.restart();
    
    sf::Clock deltaClock;
    bool b = false;

    char ip[16];
    memset(ip, '\0', 16);
    int port = 53001;

    // run the program as long as the window is open
    while (window.isOpen())
    {   
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (Game::isInGame()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                Game::client.move({ 0, -Game::client.moveSpeed * 20.f / deltaClock.getElapsedTime().asMicroseconds() });
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                Game::client.move({ 0, Game::client.moveSpeed * 20.f / deltaClock.getElapsedTime().asMicroseconds() });
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                Game::client.move({ -Game::client.moveSpeed * 20.f / deltaClock.getElapsedTime().asMicroseconds(), 0 });
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                Game::client.move({ Game::client.moveSpeed * 20.f / deltaClock.getElapsedTime().asMicroseconds(), 0 });
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        if (Game::state == State::MAINMENU) {
            window.setFramerateLimit(30);
            ImGui::Begin("play game");
                ImGui::InputText("username",Game::client.player.name,32);
                ImGui::Separator();
                if (ImGui::Button("singplayer test")) {
                    Game::LoadSingleplayer();
                    Game::client.player.label.setString(std::string(Game::client.player.name));
                }
                ImGui::Separator();
                ImGui::InputText("ip", ip, 16);
                ImGui::InputInt("port", &port);
                if (ImGui::Button("multiplayer test")) {
                    Game::LoadMultiplayer();
                    Game::client.player.label.setString(std::string(Game::client.player.name));
                    
                    sf::Socket::Status status = socket.connect(ip, port);
                    if (status != sf::Socket::Done)
                    {
                        // error...
                    }
                    else {
                        Trimmed_Player data;
                        strcpy_s(data.name, Game::client.player.name);
                        data.id = Game::client.player.id;
                        data.position = Game::client.player.position;

                        sf::Packet packet;
                        packet.append((void*)&data, sizeof(Trimmed_Player));
                        socket.send(packet);
                        packet.clear();
                    }
                }
            ImGui::End();
        }
        else if (Game::isInGame()) {
            window.setFramerateLimit(100);

            if (timeSinceTick.getElapsedTime().asMilliseconds() >= 50) {
                std::cout << "you: {" << Game::client.player.position.x << ", " << Game::client.player.position.y << "}" << std::endl;
                std::cout << "other: {" << Game::player2.position.x << ", " << Game::player2.position.y << "}" << std::endl;
                
                Trimmed_Player data;
                strcpy_s(data.name, Game::client.player.name);
                data.id = Game::client.player.id;
                data.position = Game::client.player.position;

                sf::Packet packet;
                packet.append((void*)&data, sizeof(Trimmed_Player));
                socket.send(packet);
                packet.clear();

                
                sf::Packet recPacket;
                socket.receive(recPacket);
                const void* pointer = recPacket.getData();
                if (pointer != nullptr) {
                    data = *(Trimmed_Player*)pointer;

                    Game::player2.id = data.id;
                    strcpy_s(Game::player2.name, data.name);
                    Game::player2.position = data.position;
                }
                
                timeSinceTick.restart();
            }

        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        if (Game::state == State::SINGLEPLAYER) {
            Game::client.player.drawPlayer(window);
        }
        else if (Game::state == State::MULTIPLAYER) {
            Game::client.player.drawPlayer(window);
            Game::player2.drawPlayer(window);
            // eventually render all the players
        }
        ImGui::SFML::Render(window);
        // end the current frame
        window.display();
    }


    ImGui::SFML::Shutdown();
    return 0;
}