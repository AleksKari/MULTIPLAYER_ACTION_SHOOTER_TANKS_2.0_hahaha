#pragma once
#include <SFML/Network.hpp>
#include <network/Protocol.hpp>
#include <string>
#include <iostream>

class ClientNetwork {
public:
    ClientNetwork() {
        socket.setBlocking(false);
    }

    bool connectToLobby(const std::string& ip, unsigned short port, const std::string& lobbyCode) {
        serverIp = sf::IpAddress(ip);
        serverPort = port;

        sf::Packet packet;
        packet << PacketType::JoinLobby << lobbyCode;
        
        return socket.send(packet, serverIp, serverPort) == sf::Socket::Done;
    }

    void sendInput(float moveX, float moveY, bool isShooting) {
        sf::Packet packet;
        packet << PacketType::PlayerInput << moveX << moveY << isShooting;
        socket.send(packet, serverIp, serverPort);
    }

    void receiveUpdates() {
        sf::Packet packet;
        sf::IpAddress sender;
        unsigned short port;

        while (socket.receive(packet, sender, port) == sf::Socket::Done) {
            if (sender != serverIp) continue; // Игнорируем левые пакеты

            PacketType type;
            if (packet >> type) {
                processPacket(type, packet);
            }
        }
    }

private:
    void processPacket(PacketType type, sf::Packet& packet) {
        if (type == PacketType::GameState) {
            uint32_t entityCount;
            packet >> entityCount;
        }
    }

    sf::UdpSocket socket;
    sf::IpAddress serverIp;
    unsigned short serverPort;
};