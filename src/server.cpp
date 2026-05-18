#include <SFML/Network.hpp>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include "network/Protocol.hpp"

struct Lobby {
    sf::TcpSocket* host = nullptr;
    sf::TcpSocket* guest = nullptr;
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        return 1;
    }

    unsigned short port = std::stoi(argv[2]);

    sf::TcpListener listener;
    listener.setBlocking(false);

    if (listener.listen(port) != sf::Socket::Done) {
        return 1;
    }

    std::cout << "Authoritative relay started on port " << port << '\n';

    std::vector<std::unique_ptr<sf::TcpSocket>> clients;
    std::unordered_map<std::string, Lobby> lobbies;
    std::unordered_map<sf::TcpSocket*, std::string> socket_room;

    while (true) {
        auto incoming = std::make_unique<sf::TcpSocket>();
        if (listener.accept(*incoming) == sf::Socket::Done) {
            incoming->setBlocking(false);
            clients.push_back(std::move(incoming));
        }

        for (size_t i = 0; i < clients.size();) {
            sf::Packet packet;
            sf::Socket::Status status = clients[i]->receive(packet);

            if (status == sf::Socket::Disconnected) {
                sf::TcpSocket* disconnected = clients[i].get();
                auto room_it = socket_room.find(disconnected);
                if (room_it != socket_room.end()) {
                    auto lobby_it = lobbies.find(room_it->second);
                    if (lobby_it != lobbies.end()) {
                        if (lobby_it->second.host == disconnected) lobby_it->second.host = nullptr;
                        if (lobby_it->second.guest == disconnected) lobby_it->second.guest = nullptr;
                        if (!lobby_it->second.host && !lobby_it->second.guest) {
                            lobbies.erase(lobby_it);
                        }
                    }
                    socket_room.erase(room_it);
                }

                clients.erase(clients.begin() + i);
                continue;
            }

            if (status != sf::Socket::Done) {
                ++i;
                continue;
            }

            sf::TcpSocket* sender = clients[i].get();
            sf::Packet packet_view = packet;
            PacketType packet_type;
            if (!(packet_view >> packet_type)) {
                ++i;
                continue;
            }

            if (packet_type == PacketType::JoinLobby) {
                std::string room_code;
                bool is_host = false;
                if (!(packet_view >> room_code >> is_host) || room_code.empty()) {
                    ++i;
                    continue;
                }

                auto& lobby = lobbies[room_code];
                sf::Packet reply;
                reply << PacketType::LobbyJoined;
                sf::Int32 assigned_id = 0;

                if (is_host) {
                    if (lobby.host == nullptr) {
                        lobby.host = sender;
                        socket_room[sender] = room_code;
                        assigned_id = 1;
                        std::cout << "Host joined room " << room_code << '\n';
                    }
                } else {
                    if (lobby.host != nullptr && lobby.guest == nullptr) {
                        lobby.guest = sender;
                        socket_room[sender] = room_code;
                        assigned_id = 2;
                        std::cout << "Guest joined room " << room_code << '\n';

                        sf::Packet host_notify;
                        host_notify << PacketType::LobbyJoined << static_cast<sf::Int32>(2);
                        lobby.host->send(host_notify);
                    }
                }

                reply << assigned_id;
                sender->send(reply);
                ++i;
                continue;
            }

            auto room_it = socket_room.find(sender);
            if (room_it == socket_room.end()) {
                ++i;
                continue;
            }
            auto lobby_it = lobbies.find(room_it->second);
            if (lobby_it == lobbies.end()) {
                ++i;
                continue;
            }

            Lobby& lobby = lobby_it->second;
            sf::Packet outbound = packet;

            if (sender == lobby.host) {
                if (lobby.guest) {
                    lobby.guest->send(outbound);
                }
            } else if (sender == lobby.guest) {
                if ((packet_type == PacketType::PlayerInput || packet_type == PacketType::Shoot) && lobby.host) {
                    lobby.host->send(outbound);
                }
            }

            ++i;
        }

        sf::sleep(sf::milliseconds(5));
    }

    return 0;
}
