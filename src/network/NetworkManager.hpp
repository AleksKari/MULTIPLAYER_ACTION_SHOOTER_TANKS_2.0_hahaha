#pragma once
#include <SFML/Network.hpp>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <network/Protocol.hpp>

class Map;
class Player;

class NetworkManager {
 public:
  NetworkManager();
  ~NetworkManager();
  bool try_join_lobby(const std::string& code, unsigned short port);
  int join_lobby(const std::string& lobby_code, bool host);
  void update(Map& map, Player* local_player, Player* remote_player,
              bool host_authority);
  void send_to_all(sf::Packet& packet);
  bool is_connected() const;
  bool opponent_joined() const;
  void reset_opponent_joined();
  bool connection_lost() const;
  void reset_connection_lost();
  void disconnect();

 private:
  void receive_packets(Map& map, Player* local_player, Player* remote_player,
                       bool host_authority);

  sf::TcpSocket socket_;
  bool opponent_joined_ = false;
  bool connection_lost_ = false;
};
