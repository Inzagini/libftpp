#include "Tester/runner.hpp"

#include <string>

void register_DataBuffer(Test::Runner&);
void register_Pool(Test::Runner&);
void register_RingBuffer(Test::Runner&);
void register_Memento(Test::Runner&);
void register_Observer(Test::Runner&);
void register_Singleton(Test::Runner&);
void register_StateMachine(Test::Runner&);
void register_ThreadSafeIOStream(Test::Runner&);
void register_Log(Test::Runner&);
void register_Thread(Test::Runner&);
void register_WorkerPool(Test::Runner&);
void register_PersistentWorker(Test::Runner&);
void register_ThreadSafeQueue(Test::Runner&);
void register_IVector2(Test::Runner&);
void register_IVector3(Test::Runner&);
void register_Random2DCoordinateGenerator(Test::Runner&);
void register_PerlinNoise2D(Test::Runner&);
void register_Network(Test::Runner&);
void register_Printer(Test::Runner&);

int main(int argc, char** argv) {
  Test::Runner runner;

  register_DataBuffer(runner);
  register_Pool(runner);
  register_RingBuffer(runner);
  register_Memento(runner);
  register_Observer(runner);
  register_Singleton(runner);
  register_StateMachine(runner);
  register_ThreadSafeIOStream(runner);
  register_Log(runner);
  register_Thread(runner);
  register_WorkerPool(runner);
  register_PersistentWorker(runner);
  register_ThreadSafeQueue(runner);
  register_IVector2(runner);
  register_IVector3(runner);
  register_Random2DCoordinateGenerator(runner);
  register_PerlinNoise2D(runner);
  register_Network(runner);
  register_Printer(runner);

  if (argc > 1) {
    if (std::string(argv[1]) == "All")
      return runner.run("");

    return runner.run(argv[1]);
  }

  const auto suites = runner.suiteNames();

  std::cout << " 1. Run all tests\n";

  for (std::size_t i = 0; i < suites.size(); ++i)
    std::cout << ' ' << (i + 2) << ". " << suites[i] << '\n';

  std::cout << "Select a test suite to run: ";

  int choice = 0;
  std::cin >> choice;

  if (choice == 1)
    return runner.run("");

  if (choice >= 2 && choice <= static_cast<int>(suites.size()) + 1)
    return runner.run(suites[choice - 2]);

  std::cout << "Invalid selection\n";
  return 1;
}
