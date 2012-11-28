#include <stdlib.h>
#include <errno.h>
#include "sockets.h"
#include "service_ipc_client.h"
#include "procmounter.h"

//FIXME: Fixup error codes. For example: it doesn't make much sense to the user if it returns "No such file or directory." when the socket (that the user doesn't know about) doesn't exist.
//FIXME: Workout the heirarchy for determining socket path.

int procmount(int dir) {
	const char *socket_path;
	int socket;
	
	socket_path = getenv("PROCMOUNTER_SOCKET");
	if(!socket_path) {
		//FIXME: Is ENOSYS the right error? Should we be returning ENOSYS on other socket errors?
		errno = ENOSYS;
		return -1;
	}
	
	socket = open_local_stream_socket(socket_path);
	if(socket < 0) {
		return -1;
	}
	
	return procmounter_service_ipc_client(dir,socket);
}