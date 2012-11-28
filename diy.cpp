#include <unistd.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "diy.h"

char *fdpath(int fd) {
	char *path;
	if (asprintf(&path,"/proc/self/fd/%d",fd) < 0) {
		return NULL;
	}
	return path;
}

int procmount_diy(int dir,pid_t pid) {
	char *dirpath = fdpath(dir);
	char *flags;
	asprintf(&flags,"nsofpid=%d",pid);
	int ret = mount("proc",dirpath,"proc",0,flags);
	free(dirpath);
	free(flags);
	return ret;
}