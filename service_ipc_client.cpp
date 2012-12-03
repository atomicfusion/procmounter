#include <unistd.h>
#include <errno.h>
#include "sockets.h"
#include "service_ipc_client.h"

int procmounter_service_ipc_client(int dir,int type,int socket) {
	int err;
	
	if (send_fd(socket,dir) < 0) {
		return -1;
	}
	if (write(socket,&type,sizeof(type)) < sizeof(type)) {
		return -1;
	}
	if (read(socket,&err,sizeof(err)) < sizeof(err)) {
		return -1;
	}
	errno = err;
	if(err) {
		return -1;
	}
	return 0;
}