#include <SFML/Network.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct Client {
  std::unique_ptr<sf::TcpSocket> socket;
  std::string lobby_code = "";
};

int main(int argc, char* argv[]) {
  unsigned short port = 35678;
  if (argc >= 3) {
    port = std::stoi(argv[2]);
  }

  sf::TcpListener listener;
  listener.setBlocking(false);

  if (listener.listen(port) != sf::Socket::Done) {
    std::cerr << "Failed to bind port " << port << std::endl;
    return 1;
  }
  std::cout << "Multi-Lobby Server started on port " << port << "!"
            << std::endl;

  std::vector<std::unique_ptr<Client>> clients;

  while (true) {
    auto new_socket = std::make_unique<sf::TcpSocket>();
    if (listener.accept(*new_socket) == sf::Socket::Done) {
      new_socket->setBlocking(false);
      auto client = std::make_unique<Client>();
      client->socket = std::move(new_socket);
      clients.push_back(std::move(client));
      std::cout << "New raw connection established." << std::endl;
    }

    for (size_t i = 0; i < clients.size();) {
      sf::Packet packet;
      sf::Socket::Status status = clients[i]->socket->receive(packet);

      if (status == sf::Socket::Disconnected) {
        std::cout << "Client disconnected from lobby: "
                  << clients[i]->lobby_code << std::endl;
        clients.erase(clients.begin() + i);
      } else if (status == sf::Socket::Done) {
        // Извлекаем тип пакета
        sf::Int32 packet_type;
        sf::Packet copy_packet = packet;  // Копия для чтения заголовка

        if (copy_packet >> packet_type) {
          // Если это пакет регистрации лобби (например, тип 0 или 1 -
          // Host/Join)
          if (packet_type == 0 || packet_type == 1) {
            std::string code;
            if (copy_packet >> code) {
              clients[i]->lobby_code = code;
              std::cout << "Client registered in Room/Lobby: " << code
                        << std::endl;
            }
          }
        }

        // Пересылаем пакет только тем, кто находится в ТАКОЙ ЖЕ комнате
        // (lobby_code)
        if (!clients[i]->lobby_code.empty()) {
          for (size_t j = 0; j < clients.size(); ++j) {
            if (i != j && clients[j]->lobby_code == clients[i]->lobby_code) {
              clients[j]->socket->send(packet);
            }
          }
        }
        ++i;
      } else {
        ++i;
      }
    }
    sf::sleep(sf::milliseconds(5));
  }
  return 0;
}
