#include "kernel/types.h"

#include "user/user.h"

int main(int argc, char *argv[])
{
  int sleep_time = 0;

  if (argc > 1)
    sleep_time = atoi(argv[1]); // use the first number

  sleep(sleep_time); // call the syscall
  exit(0);
}
