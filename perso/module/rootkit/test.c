#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd;

	fd = open("toto", O_RDONLY);
	if (fd == -1) {
		printf("Open failed %s : %s\n", "toto", strerror(errno));
		return -1;
	}
	printf("Open succes !\n");
	close(fd);
	return 0;
}
