#include "Pool/pool.hpp"
#include <exception>
#include <iostream>

class A {

  int value;

public:
  A(int i) : value(i) { std::cout << "A" << value << " created \n"; }
  ~A() { std::cout << "A" << value << " destroyed \n"; }
  void print() { std::cout << "This is A" << value << "\n"; }
};

int main() {

  Pool<A> pool(5);

  try {

    {
      auto a1 = pool.acquire(1);
      auto a2 = pool.acquire(2);
    }
    auto a3 = pool.acquire(3);
    {
      auto a4 = pool.acquire(4);
      a4->print();
    }
    auto a5 = pool.acquire(5);

    pool.resize(2);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  std::cout << "\n";

  try {

    {
      auto a1 = pool.acquire(1);
      auto a2 = pool.acquire(2);
      auto a3 = pool.acquire(3);
      auto a4 = pool.acquire(4);
      a4->print();
      auto a5 = pool.acquire(5);
    }

    pool.resize(2);
    {
      auto a1 = pool.acquire(1);
      auto a2 = pool.acquire(2);
      auto a3 = pool.acquire(3);
    }
    std::cout << "Resized\n";
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
