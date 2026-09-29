#include "Network/network.hpp"
#include "Tester/runner.hpp"

#include <cassert>
#include <chrono>
#include <functional>
#include <string>
#include <thread>
#include <vector>

namespace {

bool waitFor(Server& server, std::vector<Client*>& clients,
             const std::function<bool()>& done, int timeoutMs = 2000) {
  const auto deadline =
      std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);

  while (std::chrono::steady_clock::now() < deadline) {
    server.update();

    for (Client* client : clients)
      client->update();

    if (done())
      return true;

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return false;
}

} // namespace

void register_Network(Test::Runner& runner) {

  runner.add("Network", "MessageRoundTrip", []() {
    Message original(7);
    original << 42 << std::string("hello") << 3.5;

    Message copy = original;

    int number = 0;
    std::string text;
    double fraction = 0.0;
    copy >> number >> text >> fraction;

    assert(original.type() == 7);
    assert(number == 42);
    assert(text == "hello");
    assert(fraction == 3.5);
  });

  runner.add("Network", "ServerClientEcho", []() {
    Server server;
    server.start(45678);

    server.defineAction(1, [&server](long long& clientID, const Message& msg) {
      Message input = msg;
      std::string text;
      input >> text;

      Message reply(2);
      reply << std::string("echo: ") + text;
      server.sendTo(reply, clientID);
    });

    Client client;
    client.connect("127.0.0.1", 45678);

    std::string received;
    client.defineAction(2, [&received](const Message& msg) {
      Message input = msg;
      input >> received;
    });

    Message hello(1);
    hello << std::string("hello");
    client.send(hello);

    std::vector<Client*> clients{&client};
    bool ok = waitFor(server, clients, [&]() { return !received.empty(); });

    assert(ok);
    assert(received == "echo: hello");

    client.disconnect();
  });

  runner.add("Network", "ServerBroadcastToAll", []() {
    Server server;
    server.start(45679);

    server.defineAction(1, [&server](long long&, const Message&) {
      Message broadcast(2);
      broadcast << 99;
      server.sendToAll(broadcast);
    });

    Client first;
    Client second;
    first.connect("127.0.0.1", 45679);
    second.connect("127.0.0.1", 45679);

    int firstCount = 0;
    int secondCount = 0;
    first.defineAction(2, [&firstCount](const Message&) { firstCount++; });
    second.defineAction(2, [&secondCount](const Message&) { secondCount++; });

    Message trigger(1);
    first.send(trigger);

    std::vector<Client*> clients{&first, &second};
    bool ok = waitFor(server, clients,
                      [&]() { return firstCount > 0 && secondCount > 0; });

    assert(ok);
    assert(firstCount == 1);
    assert(secondCount == 1);

    first.disconnect();
    second.disconnect();
  });

  runner.add("Network", "ServerSurvivesClientDisconnect", []() {
    Server server;
    server.start(45680);

    server.defineAction(1, [&server](long long& clientID, const Message& msg) {
      Message input = msg;
      std::string text;
      input >> text;

      Message reply(2);
      reply << text;
      server.sendTo(reply, clientID);
    });

    {
      Client first;
      first.connect("127.0.0.1", 45680);

      std::string received;
      first.defineAction(2, [&received](const Message& msg) {
        Message input = msg;
        input >> received;
      });

      Message message(1);
      message << std::string("one");
      first.send(message);

      std::vector<Client*> clients{&first};
      assert(waitFor(server, clients, [&]() { return !received.empty(); }));
      assert(received == "one");

      first.disconnect();
    }

    // Let the server observe the closed socket and drop the client.
    for (int i = 0; i < 50; ++i) {
      server.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    Client second;
    second.connect("127.0.0.1", 45680);

    std::string received;
    second.defineAction(2, [&received](const Message& msg) {
      Message input = msg;
      input >> received;
    });

    Message message(1);
    message << std::string("two");
    second.send(message);

    std::vector<Client*> clients{&second};
    bool ok = waitFor(server, clients, [&]() { return !received.empty(); });

    assert(ok);
    assert(received == "two");

    second.disconnect();
  });

}

#ifndef LIBFTPP_TEST_NO_MAIN
int main(int argc, char** argv) {
  Test::Runner runner;
  register_Network(runner);
  return runner.run(argc > 1 ? argv[1] : "");
}
#endif

