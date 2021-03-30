// init: The initial user-level program

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "syslib.c"

//char *argv[] = { "sh", 0 };

int
main(void)
{
  
  
  if (term_color(2,0) == 2){
      printf(1,"worked");
      if (term_color(0,1) == 2){
          printf(1,"worked 1");
          exit();
      }
  }
  printf(1,"Failed");
  /*
  for(;;){
    printf(1, "init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf(1, "init: fork failed\n");
      exit();
    }
    if(pid == 0){
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while((wpid=wait()) >= 0 && wpid != pid)
      printf(1, "zombie!\n");
  }*/
  exit();
}
