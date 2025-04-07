// client.cpp
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

const unsigned short PORT = 53000;
const std::string SERVER_IP = "172.28.140.243";
const float PLAYER_RADIUS = 20.f;

int main() {
    sf::TcpSocket socket;
    if (socket.connect(SERVER_IP, PORT) != sf::Socket::Done) {
        std::cout << "Connection failed\n";
        return 1;
    }
    socket.setBlocking(false);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Client");
    std::map<int, sf::CircleShape> players;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            sf::Packet packet;
            sf::Int32 inputCode = 'W';
            packet << inputCode;
            socket.send(packet);
            std::cout << "Sent input: W\n";  // 👈 Add this


        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            sf::Packet packet;
            packet << 'S';
            socket.send(packet);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            sf::Packet packet;
            packet << 'A';
            socket.send(packet);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            sf::Packet packet;
            packet << 'D';
            socket.send(packet);
        }

        // Receive updates
        sf::Packet recvPacket;
        if (socket.receive(recvPacket) == sf::Socket::Done) {
            int count;
            recvPacket >> count;
            for (int i = 0; i < count; ++i) {
                int id;
                float x, y;
                recvPacket >> id >> x >> y;

                if (players.find(id) == players.end()) {
                    sf::CircleShape shape(PLAYER_RADIUS);
                    shape.setFillColor(sf::Color::Green);
                    shape.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS);
                    players[id] = shape;
                }
                players[id].setPosition(x, y);
            }
        }

        // Render
        window.clear();
        for (auto& [_, shape] : players)
            window.draw(shape);
        window.display();

        sf::sleep(sf::milliseconds(10));
    }

    return 0;
}
