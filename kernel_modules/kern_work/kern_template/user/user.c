#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define IOCTL_SET_AGE	1001
#define IOCTL_GET_AGE	1002

#define DEFAULT_AGE_VALUE 42

static void usage();
static void poll_test(int fd);
static void ioctl_set_test(int fd, int age);
static void ioctl_get_test(int fd);

int main(int argc, char *argv[])
{
	int fd;

	if (argc != 3) {
		usage();
		return 1;
	}

	printf("opening %s\n", argv[1]);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("failed to open");
		return 1;
	}

	if (!strcmp(argv[2], "poll")) {
		poll_test(fd);
		return 0;
	} else if (!strcmp(argv[2], "ioctlset")) {
		ioctl_set_test(fd, DEFAULT_AGE_VALUE);
	} else if (!strcmp(argv[2], "ioctlget")) {
		ioctl_get_test(fd);
	}

	return 0;
}

static void usage()
{
	printf("./userapp <dev-name> <command>\n");
	printf("./userapp /dev/hello-dev poll\n");
	printf("./userapp /dev/hello-dev ioctlset\n");
	printf("./userapp /dev/hello-dev ioctlget\n");
}

static void poll_test(int fd)
{
	int ret;
	struct pollfd pfd;

	pfd.fd = fd;
	pfd.events = POLLIN;

	// infinite wait
	printf("polling\n");
	ret = poll(&pfd, 1, -1);

	if (ret == -1) {
		perror("poll");
		return;
	}

	if (pfd.revents & POLLIN) {
		int nbytes = 0, total = 0;
		char buffer[256] = { 0 };

		printf("wake up\n");
		do {
			nbytes = read(pfd.fd, &buffer[total], 1);
			total += nbytes;
			printf("nbytes: %d\n", nbytes);
		} while (nbytes > 0);
		printf("buffer: %s\n", buffer);
	}
}

static void ioctl_set_test(int fd, int age)
{
	int rc;

	printf("%s %d\n", __func__, age);
	rc = ioctl(fd, IOCTL_SET_AGE, &age);

	if (rc < 0) {
		printf("ioctl_set_msg failed:%d\n", rc);
		exit(-1);
	}

	printf("%s %d\n", __func__, age);
	ioctl_get_test(fd);
}

static void ioctl_get_test(int fd)
{
	int rc = 0;
	int age = 0;

	rc = ioctl(fd, IOCTL_GET_AGE, &age);

	if (rc < 0) {
		printf("ioctl_get_msg failed:%d\n", rc);
		exit(-1);
	}

	printf("%s %d\n", __func__, age);
}
