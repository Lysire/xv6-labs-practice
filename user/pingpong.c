#include "kernel/types.h"
#include "user/user.h"

#define SEND_BYTE "c"

int main(int argc, char *argv[])
{
  // set up pipe
  int p[2];
  pipe(p);

  if (fork() != 0)
  {
    write(p[1], SEND_BYTE, 1); // send a byte to child
    char read_byte[1];
    read(p[0], &read_byte, 1); // read from child
    
    if (strcmp(read_byte, SEND_BYTE) == 0)
      printf("%d: received pong\n", getpid());
    
    exit(0);
  }
  else
  {
    char read_byte[1];
    read(p[0], read_byte, 1);
    
    if (strcmp(read_byte, SEND_BYTE) == 0)
      printf("%d: received ping\n", getpid());
    
    write(p[1], read_byte, 1); // send to parent
    exit(0);
  }
}
