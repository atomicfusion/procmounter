#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include "sockets.h"
#include "diy.h"
#include "service_ipc_server.h"

int procmounter_service_ipc_server(int socket) {
	int dir;
	int type;
	int err = 0;
	ucred creds;
	
	get_local_socket_credentials(socket,&creds);
	
	dir = recv_fd(socket);
	if (dir < 0 && !err) {
		err = errno;
	}
	if(read(socket,&type,sizeof(type)) != sizeof(type) && !err) {
		err = errno;
	}
	
	int ret = procmount_diy(dir,type,creds.pid);
	if(ret) {
		err = errno;
	}
	
	close(dir);
	
	if(write(socket,&err,sizeof(err)) != sizeof(err) && !err) {
		err = errno;
	}
	return err;
}