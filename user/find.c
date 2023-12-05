#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char buf[512];

char* fmtname(char *path)
{
  char *p;

  for (p = path + strlen(path); p >= path && *p != '/'; --p);
  ++p;

  return p;
}

void find(char *path, char *file_name)
{
  char *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type)
  {
  case T_DEVICE:
  case T_FILE:
    if (strcmp(fmtname(path), file_name) == 0)
      printf("%s", path);
    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
      printf("find: path too long\n");
      break;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0)
        continue;

      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      if (stat(buf, &st) < 0)
      {
        printf("find: cannot stat %s\n", buf);
        continue;
      }

      char *last_name = fmtname(buf);

      switch (st.type)
      {
      case T_DEVICE:
      case T_FILE:
        if (strcmp(last_name, file_name) == 0)
          printf("%s\n", buf);
        break;

      case T_DIR:
        if (strcmp(last_name, ".") != 0 && strcmp(last_name, "..") != 0)
          find(buf, file_name);
        break;
      }
    }

    break;
  }
  close(fd);
}

int main(int argc, char *argv[])
{

  if (argc <= 2)
  {
    fprintf(2, "usage: find dir name\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}
