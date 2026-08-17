#include "Network/Client/client.hpp"
#include <netinet/in.h>
#include <stdexcept>

Client::Client() : _socket(-1) {};

Client::~Client() { disconnect(); }

void Client::connect(const std::string& address, const size_t port) {

  if (_socket != -1)
    throw std::runtime_error("Client already connected");

  _socket = socket(AF_INET, SOCK_STREAM, 0);

  if (_socket == -1)
    throw std::runtime_error("Failed to create an socket");

  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(static_cast<uint16_t>(port));

  if (inet_pton(AF_INET, address.c_str(), &serverAddress.sin_addr) <= 0) {
    close(_socket);
    _socket = -1;

    throw std::runtime_error("Invalid server address");
  }

  if (::connect(_socket,

                reinterpret_cast<sockaddr*>(&serverAddress),

                sizeof(serverAddress)) == -1)

  {
    close(_socket);
    _socket = -1;

    throw std::runtime_error("Failed to connect to server");
  }

  setNonBlocking();
}

void Client::disconnect() {
  if (_socket == -1)
    return;

  close(_socket);
  _socket = -1;
  _receiveBuffer.clear();
}

void Client::setNonBlocking() {
  int flags = fcntl(_socket, F_GETFL, 0);

  if (flags == -1)
    throw std::runtime_error("Failed to get socket flags");

  if (fcntl(_socket, F_SETFL, flags | O_NONBLOCK) == -1)
    throw std::runtime_error("Failed to set socket to non-blocking");
}

void Client::defineAction(
    const Message::Type& messageType,
    const std::function<void(const Message& msg)>& action) {
  _actions[messageType] = action;
}

void Client::send(const Message& message) {
  if (_socket == -1)
    throw std::runtime_error("Client is not connected");

  std::vector<uint8_t> packet = serialize(message);
  sendAll(packet.data(), packet.size());
}

std::vector<uint8_t> Client::serialize(const Message& message) const {
  uint32_t payloadSize = static_cast<uint32_t>(message.data().size());
  int32_t type = static_cast<int32_t>(message.type());

  const size_t headerSize = sizeof(payloadSize) + sizeof(type);

  std::vector<uint8_t> packet(headerSize + payloadSize);

  size_t offset = 0;

  std::memcpy(packet.data() + offset, &payloadSize, sizeof(payloadSize));
  offset += sizeof(payloadSize);
  std::memcpy(packet.data() + offset, &type, sizeof(type));
  offset += sizeof(type);

  if (payloadSize > 0)
    std::memcpy(packet.data() + offset, message.data().data(), payloadSize);

  return packet;
}

void Client::sendAll(const uint8_t* data, size_t size) {
  size_t totalSent = 0;

  while (totalSent < size) {
    ssize_t result = ::send(_socket, data + totalSent, size - totalSent, 0);

    if (result > 0) {
      totalSent += static_cast<size_t>(result);
      continue;
    }

    if (result == -1 && (errno == EINTR))
      continue;

    throw std::runtime_error("Failed to send message");
  }
}

void Client::receive() {
  uint8_t buffer[4096];
  while (true) {
    ssize_t received = ::recv(_socket, buffer, sizeof(buffer), 0);

    if (received > 0) {
      _receiveBuffer.insert(_receiveBuffer.end(), buffer, buffer + received);

      continue;
    }

    if (received == 0) {
      disconnect();
      return;
    }

    if (errno == EAGAIN || errno == EWOULDBLOCK)
      break;

    if (errno == EINTR)
      continue;

    disconnect();
    throw std::runtime_error("Failed to receive data");
  }
}

void Client::processMessages() {
  while (true) {
    Message message(0);

    if (!tryExtractMessage(message))
      break;

    auto it = _actions.find(message.type());

    if (it != _actions.end())
      it->second(message);
  }
}

void Client::update() {
  if (_socket == -1)
    return;

  receive();
  processMessages();
}

bool Client::tryExtractMessage(Message& message) {
  constexpr size_t headerSize = sizeof(uint32_t) + sizeof(int32_t);

  if (_receiveBuffer.size() < headerSize)
    return false;

  uint32_t payloadSize;
  int32_t type;

  std::memcpy(&payloadSize, _receiveBuffer.data(), sizeof(payloadSize));
  std::memcpy(&type, _receiveBuffer.data() + sizeof(payloadSize), sizeof(type));

  const size_t totalSize = headerSize + payloadSize;

  if (_receiveBuffer.size() < totalSize)
    return false;

  message = Message(type);

  if (payloadSize > 0) {
    message.data().assign(_receiveBuffer.begin() + headerSize,
                          _receiveBuffer.begin() + totalSize);
  }

  _receiveBuffer.erase(_receiveBuffer.begin(),
                       _receiveBuffer.begin() + totalSize);

  return true;
}
