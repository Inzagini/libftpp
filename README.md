This project has been created as part of the 42 curriculum by `quannguy`.

# 🧰 libftpp - A Reusable C++ Utility Library

libftpp is a header-and-source C++ library developed as part of the 42 curriculum. The goal of the project is to collect a set of reusable components – data structures, design patterns, threading primitives, networking, and mathematics helpers – into a single, easy-to-link library that can be reused across future projects.

The project is written in C++ and targets the C++20 standard. It is built with CMake and ships with a lightweight header-based test runner and an extensive set of self-checking test cases.

The main objectives of the project are to understand and implement fundamental concepts related to:

- Generic programming and C++ templates.
- Memory management and object lifetime.
- Multithreading, synchronisation and thread pools.
- Non-blocking TCP networking and binary serialization.
- Classic software design patterns.
- 2D/3D mathematics and procedural generation.
- Build systems and automated testing.

libftpp provides a compact foundation that other projects can depend on instead of re-implementing the same boilerplate.

## Features ✨

- **Data structures**: binary `DataBuffer` and a fixed-size object `Pool`.
- **Design patterns**: `Memento`, `Observer`, `Singleton` and `StateMachine`.
- **Threading**: `Thread`, `ThreadSafeQueue`, `WorkerPool` and `PersistentWorker`.
- **Networking**: non-blocking TCP `Server` and `Client` built on a serializable `Message`.
- **Mathematics**: `IVector2`, `IVector3`, `PerlinNoise2D` and `Random2DCoordinateGenerator`.
- **I/O**: `ThreadSafeIOStream` with per-thread prefixes.
- **Utilities**: a `Timer` and a small `Tester` runner/printer.
- Header-first, template-based design with CMake integration.
- Lightweight and easy to extend.

## Project Structure 📂

```
libftpp/
├── .clang-format
├── .gitignore
├── CMakeLists.txt
├── build/
├── includes/
│   ├── DataStructure/
│   │   ├── data_structure.hpp
│   │   ├── DataBuffer/
│   │   │   └── data_buffer.hpp
│   │   └── Pool/
│   │       └── pool.hpp
│   ├── DesignPatterns/
│   │   ├── design_patterns.hpp
│   │   ├── Memento/
│   │   │   └── memento.hpp
│   │   ├── Observer/
│   │   │   └── observer.hpp
│   │   ├── Singleton/
│   │   │   └── singleton.hpp
│   │   └── StateMachine/
│   │       └── state_machine.hpp
│   ├── IOStream/
│   │   └── ThreadSafeIOStream/
│   │       └── thread_safe_iostream.hpp
│   ├── Mathematics/
│   │   ├── IVector2/
│   │   │   └── ivector2.hpp
│   │   ├── IVector3/
│   │   │   └── ivector3.hpp
│   │   ├── PerlinNoise2D/
│   │   │   └── perlin_noise_2D.hpp
│   │   └── Random2DCoordinateGenerator/
│   │       └── random_2D_coordinate_generator.hpp
│   ├── Network/
│   │   ├── Client/
│   │   │   └── client.hpp
│   │   ├── Message/
│   │   │   └── message.hpp
│   │   └── Server/
│   │       └── server.hpp
│   ├── Tester/
│   │   ├── printer.hpp
│   │   └── runner.hpp
│   ├── Thread/
│   │   ├── threading.hpp
│   │   ├── PersistentWorker/
│   │   │   └── persistent_worker.hpp
│   │   ├── Thread/
│   │   │   └── thread.hpp
│   │   ├── ThreadSafeQueue/
│   │   │   └── thread_safe_queue.hpp
│   │   └── WorkerPool/
│   │       └── worker_pool.hpp
│   └── Timer/
│       └── timer.hpp
├── srcs/
│   ├── DataStructure/
│   │   └── DataBuffer/data_buffer.cpp
│   ├── DesignPatterns/
│   │   └── Memento/memento.cpp
│   ├── IOStream/
│   │   └── ThreadSafeIOStream/thread_safe_iostream.cpp
│   ├── Mathematics/
│   │   ├── IVector2/ivector2.cpp
│   │   ├── IVector3/ivector3.cpp
│   │   ├── PerlinNoise2d/perlin_noise_2D.cpp
│   │   └── Random2DCoordinateGenerator/random_2D_coordinate_generator.cpp
│   ├── Network/
│   │   ├── Client/client.cpp
│   │   ├── Message/message.cpp
│   │   └── Server/server.cpp
│   └── Thread/
│       ├── PersistentWorker/persistent_worker.cpp
│       ├── Thread/thread.cpp
│       └── WorkerPool/worker_pool.cpp
└── tests/
    ├── DataStructure/
    │   ├── data_buffer_test.cpp
    │   └── pool_test.cpp
    ├── DesignPatterns/
    │   ├── memento_test.cpp
    │   ├── observer_test.cpp
    │   ├── singleton_test.cpp
    │   └── state_machine_test.cpp
    ├── IOStream/
    │   └── thread_safe_iostream_test.cpp
    ├── Mathematics/
    │   ├── ivector2_test.cpp
    │   ├── ivector3_test.cpp
    │   ├── perlin_noise_2D_test.cpp
    │   └── random_2D_coordinate_generator_test.cpp
    └── Thread/
        ├── persistent_worker_test.cpp
        ├── thread_safe_queue_test.cpp
        ├── thread_test.cpp
        └── worker_pool_test.cpp
```

## Prerequisites 🛠️

- A C++ compiler that supports C++20 or later.
- CMake 3.16 or later.
- A POSIX environment with socket headers (the networking module uses `<sys/socket.h>`).

1. Clone the repository:

   ```bash
   git clone https://github.com/Inzagini/libftpp.git
   cd libftpp
   ```

2. Configure and build the library and its tests with CMake:

   ```bash
   cmake -S . -B build
   cmake --build build
   ```

3. Run a test executable from the generated `build/tests/` directory:
   ```bash
   ./build/tests/StateMachineTest
   ```

## Usage 🎮

- Include the umbrella header for a module, or the individual component header:
  ```cpp
  #include "DesignPatterns/design_patterns.hpp"
  #include "Thread/threading.hpp"
  #include "Mathematics/IVector3/ivector3.hpp"
  ```
- Link against the `libftpp` CMake target and add the `includes/` directory, which is exported publicly by the build:
  ```cmake
  add_subdirectory(path/to/libftpp)
  target_link_libraries(your_target PRIVATE libftpp)
  ```
- Write a test using the bundled runner:
  ```cpp
  Test::Runner runner;
  runner.add("Suite", "Case", []() { assert(true); });
  return runner.run();
  ```
- Run each test binary in `build/tests/` directly: `./build/tests/IVector3Test`.

## File Descriptions 📜

- `includes/`: Public headers, organised by module.
- `srcs/`: Compiled source files for the non-template components.
- `tests/`: Self-checking test cases, one executable per component.
- `CMakeLists.txt`: Builds the `libftpp` library, exposes its include directory, and registers every test.
- `build/`: Generated build directory created by CMake. It should not be committed to the repository.
- `.clang-format`: LLVM-based formatting rules (2-space indent, 80 columns).
- `.gitignore`: Excludes build artifacts and local files from version control.

## AI Usage 🤖

AI tools were used as an auxiliary resource during the development of this project.

AI assistance was used for tasks such as:

- Clarifying C++ template, memory and concurrency concepts.
- Reviewing design-pattern implementations and suggesting edge cases.
- Improving and structuring project documentation, including this README.

## License 📄

This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments 🙌

- [C++ Reference](https://en.cppreference.com/) - For language and standard library documentation.
- [Learn C++](https://www.learncpp.com/) - For tutorials and resources on modern C++.
- 42 - For the curriculum and project requirements.
