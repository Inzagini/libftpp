#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  int fd = shm_open("/test", O_CREAT | O_RDWR, 0600);

  ftruncate(fd, 1024);

  auto ptr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (ptr == MAP_FAILED)
    return 1;

  const char* str = "TEST";

  memcpy(ptr, str, 5);

  std::printf("Wrote: %s\n", (const char*)str);

  std::printf("Read: %s\n", (const char*)ptr);

  munmap(ptr, 1024);

  close(fd);

  shm_unlink("/test");

  return 0;
}
