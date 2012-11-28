#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <errno.h>
#include "sockets.h"

//FIXME: Migrate to a new, real library.

int create_local_stream_socket(const char *socket_path,int socket_backlog) {
	int socket_fd;
	struct sockaddr_un socket_address;

	socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(socket_fd < 0) {
		return -1;
	}
	
	//Clear address. (Why is this needed?)
	memset(&socket_address, 0, sizeof(struct sockaddr_un));

	//Set address options.
	socket_address.sun_family = AF_UNIX;
	snprintf(socket_address.sun_path, UNIX_PATH_MAX, "%s",socket_path);

	//Bind the socket.
	if(bind(socket_fd,(struct sockaddr *) &socket_address, sizeof(struct sockaddr_un))) {
		return -2;
	}

	//Listen on the socket.
	if(listen(socket_fd,socket_backlog) != 0) {
		return -3;
	}

	return socket_fd;
}

int open_local_stream_socket(const char *socket_path) {
	int socket_fd;
	struct sockaddr_un socket_address;

	socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(socket_fd < 0) {
		return -1;
	}

	//Clear address. (Why is this needed?)
	memset(&socket_address, 0, sizeof(struct sockaddr_un));

	//Set address options.
	socket_address.sun_family = AF_UNIX;
	snprintf(socket_address.sun_path, UNIX_PATH_MAX, "%s",socket_path);

	//Connect
	if(connect(socket_fd,(struct sockaddr *) &socket_address,sizeof(struct sockaddr_un))) {
		return -2;
	}
	return socket_fd;
}

void accept_stream_socket(int socket,bool *condition,void (*handler)(int connection, struct sockaddr_un *address, socklen_t *address_length)) {
	int connection;
	while(*condition) {
		struct sockaddr_un address;
		socklen_t address_length;
		
		address_length = sizeof(address);
		connection = accept(socket,(struct sockaddr *) &address,&address_length);
		if(connection >= 0) {
			(*handler)(connection, &address, &address_length);
		}
	}
}

int send_fd(int socket, int fd) {
	int fd_buffer [1];
	fd_buffer[0] = fd;
	
	msghdr message;
	
	message.msg_name = NULL;
	message.msg_namelen = 0;
	message.msg_flags = MSG_NOSIGNAL;
	
	iovec dummy;
	char dummy_buffer [1];
	//FIXME: Should send specific dummy data (to be checked)?
	//dummy_buffer[0] = 'F';
	dummy.iov_base = &dummy_buffer;
	dummy.iov_len = sizeof(dummy_buffer);
	message.msg_iov = &dummy;
	message.msg_iovlen = 1;
	
	char control_buffer [CMSG_SPACE(sizeof(fd_buffer))];
	message.msg_control = control_buffer;
	message.msg_controllen = sizeof(control_buffer);
	
	cmsghdr *control = CMSG_FIRSTHDR(&message);
	control->cmsg_len = CMSG_LEN(sizeof(fd_buffer));
	control->cmsg_level = SOL_SOCKET;
	control->cmsg_type = SCM_RIGHTS;
	mempcpy(CMSG_DATA(control),fd_buffer,sizeof(fd_buffer));
	
	ssize_t written = sendmsg(socket,&message,MSG_NOSIGNAL);
	if(written < 1) {
		//FIXME: Should send different data on failure?
		//FIXME: Should we even try to resend on failure? The goal is to make sure that the data stream doesn't get off by one byte if sending the FD fails. 
		send(socket,dummy_buffer,sizeof(dummy_buffer),MSG_NOSIGNAL);
	}
}

int recv_fd(int socket) {
	int fd_buffer [1];
	
	msghdr message;
	
	message.msg_name = NULL;
	message.msg_namelen = 0;
	message.msg_flags = MSG_NOSIGNAL;
	
	iovec dummy;
	char dummy_buffer [1];
	dummy.iov_base = &dummy_buffer;
	dummy.iov_len = sizeof(dummy_buffer);
	message.msg_iov = &dummy;
	message.msg_iovlen = 1;
	
	char control_buffer [CMSG_SPACE(sizeof(fd_buffer))];
	message.msg_control = control_buffer;
	message.msg_controllen = sizeof(control_buffer);
	
	if(recvmsg(socket,&message,MSG_NOSIGNAL) < 0) {
		return -1;
	}
	
	//FIXME: Should check dummy data?
	
	cmsghdr *control = CMSG_FIRSTHDR(&message);
	if(control->cmsg_level == SOL_SOCKET
		&& control->cmsg_type == SCM_RIGHTS
	) {
		mempcpy(fd_buffer,CMSG_DATA(control),sizeof(fd_buffer));
		return fd_buffer[0];
	}
	errno = ENOMSG;
	return -1;
}

int get_local_socket_credentials(int fd,struct ucred *credentials) {
	unsigned int credentials_length = sizeof(*credentials);
	return getsockopt(fd,SOL_SOCKET,SO_PEERCRED,credentials,&credentials_length);
}