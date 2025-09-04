#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];

void
wc(int fd, char *name, int flags[])
{
  int i, n;
  int l, w, c, inword;

  l = w = c = 0;
  inword = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      c++;
      if(buf[i] == '\n')
        l++;
      if(strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if(!inword){
        w++;
        inword = 1;
      }
    }
  }
  if(n < 0){
    printf("wc: read error\n");
    exit(1);
  }
  
  if (flags[0] == 0 && flags[1] == 0 && flags[2] == 0){
    printf("%d %d %d %s\n", l, w, c, name);
  } else {
    if (flags[0] == 1){printf("%d ", l);}
    if (flags[1] == 1){printf("%d ", w);}
    if (flags[2] == 1){printf("%d ", c);}
    printf("%s\n", name);
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;
  int argStart = 1;
  int flags[3] = {0,0,0};


  for (int i = 1; i <= 3; i++) {

    if (strcmp(argv[i],"-l") == 0){
    argStart = argStart + 1;
    flags[0] = 1;
    continue;
    }

    if (strcmp(argv[i],"-w") == 0){
      argStart = argStart + 1;
      flags[1] = 1;
      continue;
    }

    if (strcmp(argv[i],"-c") == 0){
      argStart = argStart + 1;
      flags[2] = 1;
      continue;
    }
    break;
  }

  printf("-l: %d, -w: %d, -c: %d\n", flags[0], flags[1], flags[2]);


  // if | then read from 0. to know if using | then check argv for the of any files (ie args that do not have a "-" in them)
  // if no args then it will print nothing
  if(argc - (argStart - 1) <= 1){
    wc(0, "", flags); // 0 is file descriptor for stdin
    exit(0);
  }

  for(i = argStart; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      printf("wc: cannot open %s\n", argv[i]);
      exit(1);
    }
    wc(fd, argv[i],flags);
    close(fd);
  }
  exit(0);
}
