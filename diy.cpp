#include <unistd.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include "diy.h"
#include "procmounter.h"

char *fdpath(int fd) {
	char *path;
	if (asprintf(&path,"/proc/self/fd/%d",fd) < 0) {
		return NULL;
	}
	return path;
}

int procmount_diy(int dir,int type,ucred creds) {
	char *dirpath = fdpath(dir);
	int ret;
	switch (type) {
		case MOUNT_PROC:
			char *flags;
			asprintf(&flags,"nsofpid=%d",creds.pid);
			ret = mount("proc",dirpath,"proc",0,flags);
			free(flags);
			break;
		case MOUNT_PTS:
			/*
			char *flags;
			asprintf(&flags,"newinstance,uid=%d,gid=%d",creds.uid,creds.gid);
			ret = mount("pts",dirpath,"devpts",0,flags);
			free(flags);
			*/
			ret = mount("pts",dirpath,"devpts",0,"newinstance,ptmxmode=666");
			break;
		case MOUNT_UMOUNT:
			ret = umount2(dirpath,MNT_DETACH);
			break;
		default:
			errno = EINVAL;
			ret = -1;
	}
	free(dirpath);
	return ret;
}