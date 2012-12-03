#include <unistd.h>
#include <fcntl.h>
#include "procmounter.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc,char **argv) {
	if(argc<3) {
		fprintf(stderr,"error: args\n");
		return 1;
	}
	int type;
	if (!strcmp(argv[1],"proc")) {
		type = MOUNT_PROC;
	} else if (!strcmp(argv[1],"pts")) {
		type = MOUNT_PTS;
	} else if (!strcmp(argv[1],"umount")) {
		type = MOUNT_UMOUNT;
	} else {
		fprintf(stderr,"error: \"%s\" is not a valid mount type.\n",argv[1]);
		return 1;
	}
	int dir = open(argv[2],O_RDONLY);
	if (dir < 0) {
		fprintf(stderr,"open() error: %m\n",errno);
		return 2;
	}
	if(procmount(dir,type)) {
		fprintf(stderr,"procmount() error: %m\n",errno);
		return 2;
	}
	return 0;
}