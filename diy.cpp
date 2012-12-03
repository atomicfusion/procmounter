#include <unistd.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "diy.h"
#include "procmounter.h"

char *fdpath(int fd) {
	char *path;
	if (asprintf(&path,"/proc/self/fd/%d",fd) < 0) {
		return NULL;
	}
	return path;
}

int procmount_diy(int dir,int type,pid_t pid) {
	char *dirpath = fdpath(dir);
	int ret;
	switch (type) {
		case MOUNT_PROC:
			char *flags;
			asprintf(&flags,"nsofpid=%d",pid);
			ret = mount("proc",dirpath,"proc",0,flags);
			free(flags);
			break;
		case MOUNT_PTS:
			ret = mount("pts",dirpath,"devpts",0,"newinstance");
			break;
		default:
			errno = EINVAL;
			ret = -1;
	}
	free(dirpath);
	return ret;
}