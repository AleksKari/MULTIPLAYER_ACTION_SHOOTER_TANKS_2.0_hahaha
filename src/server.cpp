#include <SFML/Network.hpp>
#include <vector>
#include <memory>
#include <string>

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
    
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;
    
    while (true) {
        auto new_client = std::make_unique<sf::TcpSocket>();
        
        if (listener.accept(*new_client) == sf::Socket::Done) {
            new_client->setBlocking(false);
            clients.push_back(std::move(new_client));
        }
        
        for (size_t i = 0; i < clients.size(); ) {
            sf::Packet packet;
            sf::Socket::Status status = clients[i]->receive(packet);
            
            if (status == sf::Socket::Disconnected) {
                clients.erase(clients.begin() + i);
            } else if (status == sf::Socket::Done) {
                for (size_t j = 0; j < clients.size(); ++j) {
                    if (i != j) {
                        clients[j]->send(packet);
                    }
                }
                ++i;
            } else {
                ++i;
            }
        }
        
        sf::sleep(sf::milliseconds(10));
    }
    
    return 0;
}
