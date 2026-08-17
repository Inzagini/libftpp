#pragma once

#include "Network/Message/message.hpp"
#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <functional>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

class Client {
public:
  Client();
  ~Client();
  void connect(const std::string& address, const size_t port);
  void disconnect();
  void defineAction(const Message::Type& messageType,
                    const std::function<void(const Message& msg)>& action);
  void send(const Message& message);
  void update();

private:
  void setNonBlocking();
  void receive();
  void processMessages();
  void sendAll(const uint8_t* data, size_t size);
  std::vector<uint8_t> serialize(const Message& message) const;
  bool tryExtractMessage(Message& message);

private:
  int _socket;
  std::vector<uint8_t> _receiveBuffer;
  std::unordered_map<Message::Type, std::function<void(const Message)>>
      _actions;
};
