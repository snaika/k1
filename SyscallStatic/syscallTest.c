#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main()
{
  {
    time_t value = 0;
    time_t valuePtr = 35;

    printf("Premier essai, avec un paramètre passé :\n");
    printf("Before syscall = valuePtr addr : %p, valuePtr value : %ld\n", &valuePtr, valuePtr);
    value = syscall(325, &valuePtr);
    printf("After syscall  = valuePtr addr : %p, valuePtr value : %ld\n", &valuePtr, valuePtr);
    printf("return value : %ld, parameter value : %ld\n", value, valuePtr);
  }

  printf("\nNouvel essai (Avec Null passé en paramètre):\n");

  {
    time_t value = 0;
    
    value = syscall(325, NULL);
    printf("return value : %ld\n", value);
  }
  return 0;
}
