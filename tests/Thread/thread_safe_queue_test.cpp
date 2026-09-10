#include "Tester/runner.hpp"
#include "Thread/ThreadSafeQueue/thread_safe_queue.hpp"

#include <cassert>
#include <string>
#include <thread>
#include <vector>

int main() {
  Test::Runner runner;

  /*
      PUSH BACK / POP FRONT FIFO
  */

  runner.add("ThreadSafeQueue", "PushBackPopFront", []() {
    ThreadSafeQueue<int> queue;

    queue.push_back(10);
    queue.push_back(20);

    int first = queue.pop_front();
    int second = queue.pop_front();

    assert(first == 10);
    assert(second == 20);
  });

  /*
      PUSH FRONT / POP FRONT
  */

  runner.add("ThreadSafeQueue", "PushFrontPopFront", []() {
    ThreadSafeQueue<int> queue;

    queue.push_front(10);
    queue.push_front(20);

    int first = queue.pop_front();
    int second = queue.pop_front();

    assert(first == 20);
    assert(second == 10);
  });

  /*
      PUSH BACK / POP BACK LIFO
  */

  runner.add("ThreadSafeQueue", "PushBackPopBack", []() {
    ThreadSafeQueue<int> queue;

    queue.push_back(10);
    queue.push_back(20);

    int last = queue.pop_back();
    int previous = queue.pop_back();

    assert(last == 20);
    assert(previous == 10);
  });

  /*
      EMPTY QUEUE THROWS
  */

  runner.add("ThreadSafeQueue", "PopEmptyThrows", []() {
    ThreadSafeQueue<int> queue;

    bool thrown = false;

    try {
      queue.pop_front();
    } catch (const std::runtime_error&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      STRING SUPPORT
  */

  runner.add("ThreadSafeQueue", "StringQueue", []() {
    ThreadSafeQueue<std::string> queue;

    queue.push_back("Hello");
    queue.push_back("World");

    std::string first = queue.pop_front();
    std::string second = queue.pop_front();

    assert(first == "Hello");
    assert(second == "World");
  });

  /*
      EMPTY AFTER POP
  */

  runner.add("ThreadSafeQueue", "QueueEmptyAfterPop", []() {
    ThreadSafeQueue<int> queue;

    queue.push_back(42);

    queue.pop_front();

    assert(queue.empty());
  });

  /*
      SIZE TRACKING
  */

  runner.add("ThreadSafeQueue", "SizeTracking", []() {
    ThreadSafeQueue<int> queue;

    assert(queue.size() == 0);

    queue.push_back(1);
    queue.push_back(2);
    queue.push_back(3);

    assert(queue.size() == 3);

    queue.pop_front();

    assert(queue.size() == 2);
  });

  /*
      MULTIPLE PRODUCERS
  */

  runner.add("ThreadSafeQueue", "MultipleProducerThreads", []() {
    ThreadSafeQueue<int> queue;

    constexpr int threadCount = 4;
    constexpr int valuesPerThread = 100;

    std::vector<std::thread> threads;

    for (int i = 0; i < threadCount; i++) {
      threads.emplace_back([&queue, i]() {
        for (int j = 0; j < valuesPerThread; j++) {
          queue.push_back(i * valuesPerThread + j);
        }
      });
    }

    for (auto& thread : threads) {
      thread.join();
    }

    assert(queue.size() == threadCount * valuesPerThread);
  });

  /*
      PRODUCER / CONSUMER
  */

  runner.add("ThreadSafeQueue", "ProducerConsumer", []() {
    ThreadSafeQueue<int> queue;

    constexpr int count = 1000;

    std::thread producer([&queue]() {
      for (int i = 0; i < count; i++) {
        queue.push_back(i);
      }
    });

    std::vector<int> received;

    std::thread consumer([&queue, &received]() {
      while (received.size() < count) {

        try {
          received.push_back(queue.pop_front());
        } catch (const std::runtime_error&) {
          // queue temporarily empty
        }
      }
    });

    producer.join();
    consumer.join();

    assert(received.size() == count);
  });

  /*
      CUSTOM TYPE
  */

  struct Data {
    int id;
    double value;
  };

  runner.add("ThreadSafeQueue", "CustomType", []() {
    ThreadSafeQueue<Data> queue;

    queue.push_back({1, 3.14});

    Data result = queue.pop_front();

    assert(result.id == 1);
    assert(result.value == 3.14);
  });

  return runner.run();
}
