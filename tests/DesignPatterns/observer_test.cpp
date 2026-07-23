#include "DesignPatterns/Observer/observer.hpp"

#include "../utils/test_runner.hpp"

#include <cassert>
#include <string>

enum class Event { START, STOP, UPDATE };

int main() {
  TestRunner runner;

  /*
      BASIC SUBSCRIBE / NOTIFY
  */

  runner.add("Observer", "BasicNotify", []() {
    Observer<Event> observer;

    bool called = false;

    observer.subscribe(Event::START, [&]() { called = true; });

    observer.notify(Event::START);

    assert(called);
  });

  /*
      MULTIPLE SUBSCRIBERS
  */

  runner.add("Observer", "MultipleSubscribers", []() {
    Observer<Event> observer;

    int counter = 0;

    observer.subscribe(Event::START, [&]() { counter++; });

    observer.subscribe(Event::START, [&]() { counter++; });

    observer.notify(Event::START);

    assert(counter == 2);
  });

  /*
      EVENTS ARE INDEPENDENT
  */

  runner.add("Observer", "DifferentEvents", []() {
    Observer<Event> observer;

    bool start = false;
    bool stop = false;

    observer.subscribe(Event::START, [&]() { start = true; });

    observer.subscribe(Event::STOP, [&]() { stop = true; });

    observer.notify(Event::START);

    assert(start);
    assert(!stop);
  });

  /*
      UNKNOWN EVENT
  */

  runner.add("Observer", "NotifyUnknownEvent", []() {
    Observer<Event> observer;

    // should not throw

    observer.notify(Event::UPDATE);
  });

  /*
      SAME CALLBACK TWICE
  */

  runner.add("Observer", "DuplicateSubscription", []() {
    Observer<Event> observer;

    int count = 0;

    auto callback = [&]() { count++; };

    observer.subscribe(Event::START, callback);

    observer.subscribe(Event::START, callback);

    observer.notify(Event::START);

    assert(count == 2);
  });

  /*
      CALLBACK MODIFIES OBJECT
  */

  runner.add("Observer", "CallbackStateChange", []() {
    Observer<Event> observer;

    int value = 10;

    observer.subscribe(Event::UPDATE, [&]() { value += 5; });

    observer.notify(Event::UPDATE);

    assert(value == 15);
  });

  /*
      MULTIPLE NOTIFICATIONS
  */

  runner.add("Observer", "MultipleNotifyCalls", []() {
    Observer<Event> observer;

    int count = 0;

    observer.subscribe(Event::START, [&]() { count++; });

    observer.notify(Event::START);
    observer.notify(Event::START);
    observer.notify(Event::START);

    assert(count == 3);
  });

  /*
      CALLBACK EXCEPTION
  */

  runner.add("Observer", "CallbackThrows", []() {
    Observer<Event> observer;

    observer.subscribe(Event::START,
                       []() { throw std::runtime_error("callback error"); });

    bool thrown = false;

    try {
      observer.notify(Event::START);
    } catch (const std::exception&) {
      thrown = true;
    }

    assert(thrown);
  });

  return runner.run();
}
