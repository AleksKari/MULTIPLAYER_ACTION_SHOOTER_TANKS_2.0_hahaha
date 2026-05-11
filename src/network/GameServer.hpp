#pragma once
#include <SFML/Network.hpp>
#include "Protocol.hpp"
#include <map>
#include <iostream>

struct ConnectedClient {
    sf::IpAddress ip;
    unsigned short port;
    uint32_t playerId;
};

class GameServer {
public:
    GameServer(unsigned short port) {
        if (socket.bind(port) != sf::Socket::Done) {
            std::cerr << "Failed to bind server port!\n";
        }
        socket.setBlocking(false);
        std::cout << "Server started on port " << port << "\n";
    }

    void update() {
        receiveNetworkData();

        broadcastGameState();
    }

private:
    void receiveNetworkData() {
        sf::Packet packet;
        sf::IpAddress sender;
        unsigned short port;

        while (socket.receive(packet, sender, port) == sf::Socket::Done) {
            PacketType type;
            if (packet >> type) {
                if (type == PacketType::JoinLobby) {
                    std::string code;
                    packet >> code;
                    handleJoin(sender, port, code);
                } else if (type == PacketType::PlayerInput) {
                    handleInput(sender, port, packet);
                }
            }
        }
    }

    void handleJoin(sf::IpAddress ip, unsigned short port, const std::string& code) {
        std::string clientKey = ip.toString() + ":" + std::to_string(port);
        
        if (clients.find(clientKey) == clients.end()) {
            clients[clientKey] = {ip, port, nextPlayerId++};
            std::cout << "Player joined! ID: " << clients[clientKey].playerId << "\n";

            sf::Packet reply;
            reply << PacketType::LobbyJoined << clients[clientKey].playerId;
            socket.send(reply, ip, port);
        }
    }

    void handleInput(sf::IpAddress ip, unsigned short port, sf::Packet& packet) {
        float moveX, moveY;
        bool isShooting;
        packet >> moveX >> moveY >> isShooting;
    }

    void broadcastGameState() {
        sf::Packet statePacket;
        statePacket << PacketType::GameState;

        for (const auto& [key, client] : clients) {
            socket.send(statePacket, client.ip, client.port);
        }
    }

    sf::UdpSocket socket;
    std::map<std::string, ConnectedClient> clients;
    uint32_t nextPlayerId = 1;
};