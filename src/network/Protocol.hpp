#pragma once
#include <SFML/Network.hpp>
#include <cstdint>
#include <string>

enum class PacketType : uint8_t {
    JoinLobby,
    LobbyJoined,
    PlayerInput,
    GameState,
    UpdateState,
    Shoot,
    HealthUpdate,
    SpawnWeapon,
    RemoveTile
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

namespace LobbyUtils {
    inline std::string ipToCode(const sf::IpAddress& ip) {
        std::string s = ip.toString();
        if (s == "0.0.0.0" || s == "None") return "LOCAL";
        for (char& c : s) if (c == '.') c = 'A';
        return s;
    }

    inline sf::IpAddress codeToIp(std::string code) {
        if (code == "LOCAL") return sf::IpAddress::getLocalAddress();
        for (char& c : code) if (c == 'A') c = '.';
        return sf::IpAddress(code);
    }
}
