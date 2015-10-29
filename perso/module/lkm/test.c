#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "version.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd;
	int is_version_modified;

	fd = open("/dev/version", O_RDONLY);
	if (fd == -1) {
		printf("open failed : %s\n", strerror(errno));
		return -1;
	}

	if (argc == 1) {
		if (ioctl(fd, VERSION_MODIFIED, &is_version_modified) != 0) {
			printf("ioctl failed : %s\n", strerror(errno));
			close(fd);
			return -1;
		}
		printf("is modified => %d\n", is_version_modified);
	} else {
		if (ioctl(fd, VERSION_RESET, 0) != 0) {
			printf("ioctl failed : %s\n", strerror(errno));
			close(fd);
			return -1;
		}
		printf("version reset\n");
	}
	close(fd);
	return 0;
}
