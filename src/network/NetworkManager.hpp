#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <memory>
#include <string>
#include <optional>
#include "Protocol.hpp"

class Map;

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();
    std::optional<std::string> host_lobby(unsigned short port);
    bool try_join_lobby(const std::string& code, unsigned short port);
    void update(Map& map);
    void send_to_all(sf::Packet& packet);
    bool is_host() const;
    
    // Новый метод для безопасного получения ответа авторизации комнаты
    int receive_response();

private:
    void receive_packets(Map& map);

    sf::TcpSocket socket_;
    pid_t server_pid_ = -1;
};
