// server.cpp
#include <SFML/Network.hpp>
#include <iostream>
#include <map>

const unsigned short PORT = 53000;
const float SPEED = 100.f;

struct Player {
    sf::Vector2f position = { 100, 100 };
};

int main() {
    sf::TcpListener listener;
    listener.listen(PORT);
    listener.setBlocking(false);

    std::map<int, sf::TcpSocket*> clients;
    std::map<int, Player> players;

    sf::SocketSelector selector;
    selector.add(listener);
    int nextId = 0;

    sf::Clock clock;

    while (true) {
        if (selector.wait(sf::milliseconds(10))) {
            if (selector.isReady(listener)) {
                sf::TcpSocket* client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done) {
                    client->setBlocking(false);
                    selector.add(*client);
                    clients[nextId] = client;
                    players[nextId] = Player();
                    std::cout << "New client connected! ID: " << nextId << std::endl;
                    nextId++;
                }
                else {
                    delete client;
                }
            }

            for (auto& [id, client] : clients) {
                if (selector.isReady(*client)) {
                    sf::Packet packet;
                    if (client->receive(packet) == sf::Socket::Done) {
                        sf::Int32 inputCode;
                        packet >> inputCode;
                        char input = static_cast<char>(inputCode);

                        std::cout << "Received input from client " << id << ": " << input << "\n";  // 👈 Add this

                        // char input;
                        // int tempInput;
                        // packet >> tempInput;
                        // input = static_cast<char>(tempInput);

                        float dt = clock.restart().asSeconds();
                        auto& pos = players[id].position;

                        switch (input) {
                        case 'W': pos.y -= SPEED * dt; break;
                        case 'S': pos.y += SPEED * dt; break;
                        case 'A': pos.x -= SPEED * dt; break;
                        case 'D': pos.x += SPEED * dt; break;
                        }

                        // Broadcast all positions
                        sf::Packet sendPacket;
                        sendPacket << static_cast<int>(players.size());
                        for (auto& [pid, p] : players)
                            sendPacket << pid << p.position.x << p.position.y;

                        for (auto& [_, c] : clients)
                            c->send(sendPacket);
                    }
                }
            }
        }
    }

    return 0;
}
