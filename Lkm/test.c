#include <sys/ioctl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "version.h"

#define TARGET_PATH "/dev/version"
#define TEST_STRING_1 "pony-kernel-x1"

char *getVersion(int fd, char *str, int *readValue)
{
  char buffer[4097];
  int ret;
  int error;
  
  memset(buffer, 0, 4097);
  memset(str, 0, 4097);
  
  ret = read(fd, buffer, 4096);
  if (readValue != NULL)
    *readValue = ret;
  
  if (ret == -1)
    {
      error = errno;
      printf("\tEchec de la lecture : %s\n", strerror(error));
      return str;
    }

  snprintf(str, 4097, "%s", buffer);
  return str;
}

int main()
{
  int fd;
  char buffer[4097];
  int ret;
  int error;
  
  printf("Opening %s\n", TARGET_PATH);

  fd = open(TARGET_PATH, O_CLOEXEC | O_RDWR);
  if (fd > 0)
    {
      
      printf("%s was opened\n", TARGET_PATH);
      printf("------------------\n");
      printf("Testing ioctl ...\n");

      ret = ioctl(fd, VERSION_MODIFIED);
      printf("Version modified : %s\n", ret == 0 ? "Non" : "Oui");
      ret = ioctl(fd, VERSION_RESET);
      printf("Version reset : %d\n", ret);
      ret = ioctl(fd, _IO(0, 1)); // Au hasard (Enfin pas trop non plus)
      printf("ioctl invalide : %d\n", ret);


      printf("ioctl testing over.\n");
      printf("------------------\n");
      printf("Testing read/write ...\n");

      getVersion(fd, buffer, &ret);
      printf("Version is '%s' | Number of characters read : %d\n", buffer, ret);

      ret = write(fd, TEST_STRING_1, strlen(TEST_STRING_1));
      error = errno;
      printf("Writing : New version is '%s' | Number of characters writed : %d\n", TEST_STRING_1, ret);
      if (ret < 0)
	printf("\tError while writing : %s\n", strerror(error));
      else
	printf("\tTesting : version = '%s'\n", getVersion(fd, buffer, NULL));
	
      printf("\nWriting : Sending invalid ptr\n");
      error = 0;
      ret = write(fd, NULL, 87);
      error = errno;
      printf("\tWriting : return value = %d | errno = %s\n", ret, strerror(error));

							  getVersion(fd, buffer, &ret);
      printf("Version is '%s' | Number of characters read : %d\n", buffer, ret);

      {
	char miniBuf[10];
	memset(miniBuf, 0, 10);
	printf("\nReading with small buffer\n");
	ret = read(fd, miniBuf, 9);
	printf("\tVersion is '%s' | ret = %d\n", miniBuf, ret);
      }

      {
	char *maxiBuf = malloc(9000);
	int i = 0;
	while (i < 9000)
	  {
	    maxiBuf[i] = ((i % ('z' - 'a' + 1)) + 'a');
	    i++;
	  }
	
	ret = write(fd, maxiBuf, 9000);
	printf("Write (ret:%d)\n", ret);
	getVersion(fd, buffer, &ret);
	printf("Version is '%s' (ret:%d)\n", buffer, ret);
	free(maxiBuf);
      }

      {
	char *maxiBuf = malloc(9001);
	memset(maxiBuf, 0, 9001);

	ret = read(fd, maxiBuf, 9000);
	printf("'%s' (ret:%d)\n", maxiBuf, ret);
	free(maxiBuf);
      }
      
      ioctl(fd, VERSION_RESET);
      ret = write(fd, TEST_STRING_1, strlen(TEST_STRING_1));
      printf("Rewriting new version '%s' (ret:%d)\n", getVersion(fd, buffer, NULL), ret);

      printf("------------------\n");
      printf("Re-testing ioctl\n");

      printf("Version actuelle '%s'\n", getVersion(fd, buffer, NULL));
      printf("Version modifiée ? %s\n", ioctl(fd, VERSION_MODIFIED) ? "Oui" : "Non");
      ioctl(fd, VERSION_RESET);
      printf("Version actuelle '%s'\n", getVersion(fd, buffer, NULL));
      printf("Version modifiée ? %s\n", ioctl(fd, VERSION_MODIFIED) ? "Oui" : "Non");

      printf("------------------\n");
      printf("Un dernier test pour la forme :\n'%s'\n", getVersion(fd, buffer, NULL));
      ioctl(fd, VERSION_RESET);
      printf("'%s'\n", getVersion(fd, buffer, NULL));
      
      close(fd);
      return 0;
    }
  else
    {
      error = errno;
      printf("Impossible d'ouvrir %s, %s\n", TARGET_PATH, strerror(error));
      if (error == EACCES)
	printf("Avez-vous les accès lecture/écriture ?\n");
      return -1;
    }
}

