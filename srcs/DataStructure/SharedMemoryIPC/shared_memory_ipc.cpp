#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

constexpr size_t SIZE = 1024;

int main(int argc, char** argv) {

  if (argc == 2) {

    if (std::string(argv[1]) == "WRITE") {

      shm_unlink("/test");

      int fd = shm_open("/test", O_CREAT | O_EXCL | O_RDWR, 0600);
      if (fd == -1) {
        std::perror("shm_open failed");
        return 1;
      }

      if (ftruncate(fd, SIZE) == -1) {
        std::perror("ftruncate failed");
        return 1;
      }

      auto ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
      if (ptr == MAP_FAILED) {
        std::perror("mmap failed");
        return 1;
      }

      const char* str = "Child Write";
      memcpy(ptr, str, strlen(str) + 1);
      std::printf("Wrote: %s\n", (const char*)ptr);

    } else if (std::string(argv[1]) == "READ") {

      int fd = shm_open("/test", O_RDWR, 0600);
      if (fd == -1) {
        std::perror("shm_open failed");
        return 1;
      }

      auto ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
      if (ptr == MAP_FAILED) {
        std::perror("mmap failed");
        return 1;
      }

      std::printf("Read: %s\n", (const char*)ptr);

      munmap(ptr, SIZE);
      close(fd);
      shm_unlink("/test");
    }
  }
  return 0;
}
