#include <unistd.h>
#include <sys/stat.h>
#include "sockets.h"
#include "diy.h"
#include "service_ipc_server.h"

//FIXME: IMPORTANT: Rewrite affluence sockets to support argument passing for accept_stream_socket.

bool accepting = true;

static int accept_server(const char *socket_path,int socket_backlog) {
	//FIXME: Fixup affluence sockets's return/error codes.
	int socket = create_local_stream_socket(socket_path,socket_backlog);
	if(socket < 0) {
		return -1;
	}
	
	while(accepting) {
		int connection;
		connection = accept(socket,NULL,NULL);
		if(connection < 0) {
			continue;
		}
		procmounter_service_ipc_server(connection);
		//FIXME: Check for too small buffer error return code from above?
		close(connection);
		//FIXME: Check for close errors?
	}
	close(socket);
	unlink(socket_path);
	return 0;
}
int main(int argc,char **argv) {
	if(argc != 2) {
		return 2;
	}
	
	char *socket_path;
	
	socket_path = argv[1];
	
	if(accept_server(socket_path,10)) {
		return 1;
	}
}