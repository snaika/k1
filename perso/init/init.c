#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#define _GNU_SOURCE

int main(int argc, char **argv)
{
  time_t time;

  while (1) {
    printf("uptime => %ld\n", syscall(325, &time));
    sleep(1);
  }
  return 0;
}
