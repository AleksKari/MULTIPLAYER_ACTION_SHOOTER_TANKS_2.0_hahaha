#include "NetworkManager.hpp"
#include "Player.hpp"
#include "map/Map.hpp"
#include "tile/EmptyTile.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

NetworkManager::NetworkManager() {}

NetworkManager::~NetworkManager() {
    if (server_pid_ > 0) {
        kill(server_pid_, SIGTERM);
        waitpid(server_pid_, nullptr, 0);
    }
}

std::optional<std::string> NetworkManager::host_lobby(unsigned short port) {
    server_pid_ = fork();
    
    if (server_pid_ == 0) {
        execl("./server", "./server", "0.0.0.0", std::to_string(port).c_str(), (char*)nullptr);
        _exit(127);
    }
    
    if (server_pid_ > 0) {
        sf::sleep(sf::milliseconds(100));
        if (try_join_lobby("LOCAL", port)) {
            return LobbyUtils::ipToCode(sf::IpAddress::getLocalAddress());
        }
    }
    
    return std::nullopt;
}

bool NetworkManager::try_join_lobby(const std::string& code, unsigned short port) {
    sf::IpAddress ip = LobbyUtils::codeToIp(code);
    
    if (ip == sf::IpAddress::None) {
        return false;
    }
    
    socket_.disconnect();
    socket_.setBlocking(true);
    
    if (socket_.connect(ip, port, sf::seconds(5)) == sf::Socket::Done) {
        socket_.setBlocking(false);
        return true;
    }
    
    return false;
}

void NetworkManager::update(Map& map) {
    receive_packets(map);
}

void NetworkManager::receive_packets(Map& map) {
    sf::Packet packet;
    
    while (socket_.receive(packet) == sf::Socket::Done) {
        PacketType type; 
        packet >> type;
        
        if (type == PacketType::UpdateState) {
            sf::Uint32 id; 
            packet >> id;
            map.update_remote_player(id, packet);
        } else if (type == PacketType::Shoot) {
            map.spawn_remote_projectile(packet);
        } else if (type == PacketType::HealthUpdate) {
            sf::Uint32 id; 
            int hp; 
            packet >> id >> hp;
            map.update_player_hp(id, hp);
        } else if (type == PacketType::SpawnWeapon) {
            int x;
            int y;
            int w_type;
            packet >> x >> y >> w_type;
            map.spawn_weapon_at(x, y, w_type);
        } else if (type == PacketType::RemoveTile) {
            int x;
            int y;
            packet >> x >> y;
            map.set_tile(Vec2(x, y), std::make_unique<EmptyTile>());
        }
        packet.clear();
    }
}

void NetworkManager::send_to_all(sf::Packet& packet) {
    if (socket_.getRemoteAddress() != sf::IpAddress::None) {
        socket_.send(packet);
    }
}

bool NetworkManager::is_host() const {
    return server_pid_ > 0;
}

int NetworkManager::receive_response() {
    socket_.setBlocking(true); // Переключаем в блокирующий режим для ожидания
    sf::Packet responsePacket;
    sf::Int32 result = 0;
    
    if (socket_.receive(responsePacket) == sf::Socket::Done) {
        responsePacket >> result;
    }
    
    socket_.setBlocking(false); // Возвращаем асинхронный режим назад
    return result;
}
