#pragma once

#include "Network/Message/message.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

class Server {

public:
  Server();
  ~Server();
  void start(const size_t& port);
  void defineAction(const Message::Type& messageType,
                    const std::function<void(long long& clientID,
                                             const Message& msg)>& action);

  void sendTo(const Message& message, long long clientID);
  void sendToArray(const Message& message, std::vector<long long> clientIDs);
  void sendToAll(const Message& message);

  void update();

private:
  struct ClientConnection {
    long long id;
    int socket;
    std::vector<uint8_t> receiveBuffer;
  };
  int _listenSocket;
  long long _nextClientID;
  std::unordered_map<long long, ClientConnection> _clients;
  std::unordered_map<Message::Type, std::function<void(long long& clientID,
                                                       const Message& mgs)>>
      _actions;

private:
  void setNonBlocking(int socket);
  void acceptClients();
  void receiveFromClient(ClientConnection& client);
  void processMessage(ClientConnection& client);
  bool tryExtractMessage(ClientConnection& client, Message& msg);
  void disconnectClient(long long clientID);
  void sendMessage(int socket, const Message& msg);
  void sendAll(int socket, const uint8_t* data, const size_t size);
  std::vector<uint8_t> serialize(const Message& msg) const;
};
