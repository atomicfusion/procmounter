#ifndef AFFLUENTSOCKETS_H_
#define AFFLUENTSOCKETS_H_

#include <sys/socket.h>
#include <linux/un.h>

int create_local_stream_socket(const char *socket_path,int socket_backlog);

int open_local_stream_socket(const char *socket_path);

void accept_stream_socket(int socket,bool *condition,void (*handler)(int connection, struct sockaddr_un *address, socklen_t *address_length));

int send_fd(int socket, int fd_to_send);

int recv_fd(int socket);

int get_local_socket_credentials(int fd,struct ucred *credentials);

#endif /* AFFLUENTSOCKETS_H_ */