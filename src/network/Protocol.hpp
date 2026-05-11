#pragma once
#include <SFML/Network.hpp>
#include <cstdint>

enum class PacketType : uint8_t {
    JoinLobby,
    LobbyJoined,
    PlayerInput,
    GameState
};

inline sf::Packet& operator<<(sf::Packet& packet, const PacketType& type) {
    return packet << static_cast<uint8_t>(type);
}

inline sf::Packet& operator>>(sf::Packet& packet, PacketType& type) {
    uint8_t temp;
    packet >> temp;
    type = static_cast<PacketType>(temp);
    return packet;
}