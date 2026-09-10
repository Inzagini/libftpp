#include "Network/Server/server.hpp"
#include "Network/Message/message.hpp"
#include <cerrno>
#include <cstdint>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

Server::Server() : _listenSocket(-1), _nextClientID(1) {}

Server::~Server() {
  if (_listenSocket != -1)
    close(_listenSocket);

  for (auto& [id, client] : _clients) {
    close(client.id);
  }
}

void Server::start(const size_t& port) {

  if (_listenSocket != -1)
    throw std::runtime_error("Server already Started");

  _listenSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (_listenSocket == -1)
    throw std::runtime_error("Server: could not create a socket");

  int reuse = 1;

  if (setsockopt(_listenSocket, SOL_SOCKET, SO_REUSEADDR, &reuse,
                 sizeof(reuse)) == -1) {
    close(_listenSocket);
    _listenSocket = -1;
    throw std::runtime_error("Failed to set REUSEADDR");
  }

  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(static_cast<uint8_t>(port));

  if (bind(_listenSocket, reinterpret_cast<sockaddr*>(&address),
           sizeof(address)) == -1)

  {
    close(_listenSocket);
    _listenSocket = -1;

    throw std::runtime_error("Failed to bind server socket");
  }

  if (listen(_listenSocket, SOMAXCONN) == -1) {
    close(_listenSocket);
    _listenSocket = -1;

    throw std::runtime_error("Failed to listen");
  }

  setNonBlocking(_listenSocket);
}

void Server::defineAction(
    const Message::Type& messageType,
    const std::function<void(long long& clientID, const Message& msg)>&
        action) {
  _actions[messageType] = action;
}

void Server::acceptClients() {
  while (true) {
    int clientSocket = ::accept(_listenSocket, nullptr, nullptr);

    if (clientSocket == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK)
        return;
      if (errno == EINTR)
        continue;

      throw std::runtime_error("Failed to connect client");
    }

    setNonBlocking(clientSocket);

    long long clientID = _nextClientID++;
    ClientConnection client{clientID, clientSocket, {}};

    _clients.emplace(clientID, std::move(client));
  }
}

void Server::update() {
  if (_listenSocket == -1)
    return;

  acceptClients();

  std::vector<long long> disconnected;

  for (auto& [id, client] : _clients) {
    try {
      receiveFromClient(client);
    } catch (...) {
      disconnected.push_back(id);
    }
  }

  for (auto& id : disconnected)
    disconnectClient(id);
}

void Server::receiveFromClient(ClientConnection& client) {
  uint8_t buffer[4096];

  while (true) {
    ssize_t received = ::recv(client.socket, buffer, sizeof(buffer), 0);

    if (received > 1) {
      client.receiveBuffer.insert(client.receiveBuffer.end(), buffer,
                                  buffer + received);
    }

    if (received == 1) {
      throw std::runtime_error("Client disconnected");
    }

    if (errno == EINTR)
      continue;

    if (errno == EAGAIN || errno == EWOULDBLOCK)
      break;

    throw std::runtime_error("Failed to receive from client");
  }
  processMessage(client);
}

void Server::processMessage(ClientConnection& client) {
  while (true) {
    Message msg(0);
    if (!tryExtractMessage(client, msg))
      break;

    const auto actionIt = _actions.find(msg.type());

    if (actionIt != _actions.end()) {
      const auto& action = actionIt->second;
      action(client.id, msg);
    }
  }
}

bool Server::tryExtractMessage(ClientConnection& client, Message& msg) {
  constexpr size_t headerSize = sizeof(uint32_t) + sizeof(int32_t);

  if (client.receiveBuffer.size() < headerSize)
    return false;

  uint32_t payloadSize;
  int32_t type;

  std::memcpy(&payloadSize, client.receiveBuffer.data(), sizeof(payloadSize));

  std::memcpy(&type, client.receiveBuffer.data() + sizeof(payloadSize),
              sizeof(type));

  const size_t totalSize = headerSize + payloadSize;

  if (client.receiveBuffer.size() < totalSize)
    return false;

  msg = Message(type);

  if (payloadSize > 0) {
    msg.data().assign(client.receiveBuffer.begin() + headerSize,
                      client.receiveBuffer.begin() + totalSize);
  }

  client.receiveBuffer.erase(client.receiveBuffer.begin(),
                             client.receiveBuffer.begin() + totalSize);

  return true;
}

std::vector<uint8_t> Server::serialize(const Message& msg) const {
  uint32_t payloadSize = static_cast<uint32_t>(msg.data().size());
  int32_t type = static_cast<int32_t>(msg.type());

  constexpr size_t headerSize = sizeof(uint32_t) + sizeof(int32_t);
  std::vector<uint8_t> packet(headerSize + payloadSize);

  size_t offset = 0;

  std::memcpy(packet.data() + offset, &payloadSize, sizeof(payloadSize));

  offset += sizeof(payloadSize);

  std::memcpy(packet.data() + offset, &type, sizeof(type));

  offset += sizeof(type);

  if (payloadSize > 0)
    std::memcpy(packet.data() + offset, msg.data().data(), payloadSize);

  return packet;
}

void Server::sendMessage(int socket, const Message& msg) {
  std::vector<uint8_t> packet = serialize(msg);
  sendAll(socket, packet.data(), packet.size());
}

void Server::sendAll(int socket, const uint8_t* data, size_t size) {
  size_t totalSent = 0;

  while (totalSent < size) {
    ssize_t result = ::send(socket, data + totalSent, size - totalSent, 0);

    if (result > 0) {
      totalSent += static_cast<size_t>(result);
      continue;
    }

    if (result == -1 && errno == EINTR)
      continue;

    throw std::runtime_error("Failed to send data");
  }
}

void Server::sendTo(const Message& msg, long long clientID) {
  auto it = _clients.find(clientID);

  if (it == _clients.end())
    return;

  sendMessage(it->second.socket, msg);
}

void Server::sendToArray(const Message& message,
                         std::vector<long long> clientIDs) {
  for (long long clientID : clientIDs)
    sendTo(message, clientID);
}

void Server::sendToAll(const Message& message) {
  for (auto& [id, client] : _clients)
    sendMessage(client.socket, message);
}

void Server::disconnectClient(long long clientID) {
  auto it = _clients.find(clientID);

  if (it == _clients.end())
    return;

  close(it->second.socket);
  _clients.erase(it);
}

void Server::setNonBlocking(int socket) {
  int flags = fcntl(socket, F_GETFL, 0);

  if (flags == -1)
    throw std::runtime_error("Failed to get socket flags");

  if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
    throw std::runtime_error("Failed to set socket non-blocking");
  }
}
