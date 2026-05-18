#include "NetworkManager.hpp"
#include "Player.hpp"
#include "map/Map.hpp"
#include "tile/EmptyTile.hpp"
#include <iostream>

NetworkManager::NetworkManager() {}

NetworkManager::~NetworkManager() = default;

bool NetworkManager::try_join_lobby(const std::string& code, unsigned short port) {
    sf::IpAddress ip = LobbyUtils::codeToIp(code);
    
    if (ip == sf::IpAddress::None) {
        return false;
    }
    
    socket_.disconnect();
    socket_.setBlocking(true);
    opponent_joined_ = false;
    
    if (socket_.connect(ip, port, sf::seconds(5)) == sf::Socket::Done) {
        socket_.setBlocking(false);
        return true;
    }
    
    return false;
}

int NetworkManager::join_lobby(const std::string& lobby_code, bool host) {
    if (!is_connected()) return 0;

    sf::Packet join_packet;
    join_packet << PacketType::JoinLobby << lobby_code << host;
    if (socket_.send(join_packet) != sf::Socket::Done) return 0;

    int result_id = 0;
    sf::SocketSelector selector;
    selector.add(socket_);
    if (selector.wait(sf::seconds(2))) {
        sf::Packet response;
        if (socket_.receive(response) == sf::Socket::Done) {
            PacketType response_type;
            if ((response >> response_type) && response_type == PacketType::LobbyJoined) {
                sf::Int32 assigned_id = 0;
                response >> assigned_id;
                result_id = assigned_id;
            }
        }
    }
    return result_id;
}

void NetworkManager::update(Map& map, Player* local_player, Player* remote_player, bool host_authority) {
    receive_packets(map, local_player, remote_player, host_authority);
}

void NetworkManager::receive_packets(Map& map, Player* local_player, Player* remote_player, bool host_authority) {
    (void)local_player;
    sf::Packet packet;
    
    while (socket_.receive(packet) == sf::Socket::Done) {
        PacketType type; 
        if (!(packet >> type)) {
            packet.clear();
            continue;
        }
        
        if (type == PacketType::LobbyJoined) {
            sf::Int32 joined_id = 0;
            packet >> joined_id;
            if (host_authority && joined_id == 2) {
                opponent_joined_ = true;
            }
        } else if (type == PacketType::PlayerInput) {
            if (host_authority && remote_player) {
                bool w = false, a = false, s = false, d = false;
                packet >> w >> a >> s >> d;
                remote_player->set_input(w, a, s, d);
            }
        } else if (type == PacketType::Shoot) {
            if (host_authority && remote_player && !remote_player->is_dead()) {
                remote_player->attack(map);
            }
        } else if (type == PacketType::GameState) {
            if (!host_authority) {
                map.apply_game_state(packet);
            }
        } else if (type == PacketType::UpdateState) {
            sf::Uint32 id; 
            packet >> id;
            map.update_remote_player(id, packet);
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

bool NetworkManager::is_connected() const {
    return socket_.getRemoteAddress() != sf::IpAddress::None;
}

bool NetworkManager::opponent_joined() const {
    return opponent_joined_;
}

void NetworkManager::reset_opponent_joined() {
    opponent_joined_ = false;
}
