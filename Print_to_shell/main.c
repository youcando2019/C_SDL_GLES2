#include <stdio.h>
#include <stdlib.h>


int main( int argc, char *argv[] )
{

  FILE *fp;
  char path[1035];

  /* Open the command for reading. */
  //fp = popen("/bin/ls /etc/", "r");
  fp = popen("sudo pacman -Ss", "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }

  /* Read the output a line at a time - output it. */
  int i = 0;
  while (fgets(path, sizeof(path)-1, fp) != NULL)
  {
    i += 1;
    if(i%2 != 0)
    {
        printf("%s", path);
    }
  }

  /* close */
  pclose(fp);

  return 0;
}
