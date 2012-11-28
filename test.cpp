#include <unistd.h>
#include <fcntl.h>
#include "procmounter.h"

int main(int argc,char **argv) {
	if(argc<2) {
		return 1;
	}
	int dir = open(argv[1],O_RDONLY);
	if(procmount(dir)) {
		return 2;
	}
	return 0;
}