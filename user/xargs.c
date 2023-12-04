#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXARGSZ 100

char whitespaces[] = " \t\r\n\v";

// returns -1 if EOF
int get_line(char *buf, int nbuf)
{
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  
  if (buf[0] == 0)
    return -1;
  
  return 0;
}

/**
 * ps: passed str, es: end of passed str
 */

int get_token(char **ps, char *es, char **tok, char **end_tok)
{
  char *s;
  int ret;

  // skip whitespaces
  s = *ps;
  while (s < es && strchr(whitespaces, *s))
    ++s;

  // set token start
  if (tok)
    *tok = s;
  
  ret = *s;
  
  // find end of current string
  switch (*s)
  {
  case 0:
    break;
  default:
    ret = 'a';
    while (s < es && !strchr(whitespaces, *s))
      ++s;
    break;
  }

  // set token end
  if (end_tok)
    *end_tok = s;

  // adjust start to next non whitespace char
  while (s < es && strchr(whitespaces, *s))
    ++s;
  *ps = s;

  return ret;
}

int main(int argc, char *argv[])
{
  char *xargs[MAXARG];
  
  for (int i = 1; i < argc; ++i)
    xargs[i - 1] = argv[i]; // skip xargs name
  
  static char buf[MAXARG][MAXARGSZ];
  char *tok, *end_tok;
  
  int i = 0;
  int j = argc - 1;

  while (get_line(buf[i], sizeof(buf[i])) >= 0)
  {
    char *s = buf[i];
    char *es = s + strlen(s);

    while (get_token(&s, es, &tok, &end_tok) != 0)
    {
      xargs[j] = tok;
      *end_tok = 0;
      ++j;
      ++i;
    }
  }

  int pid = fork();
  if (pid == 0)
    exec(xargs[0], xargs);
  else
    wait(&pid);

  exit(0);
}
