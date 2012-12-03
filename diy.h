#ifndef AFFLUENCE_PROCMOUNTER_DIY_H_
#define AFFLUENCE_PROCMOUNTER_DIY_H_

#include <sys/socket.h>

int procmount_diy(int dir,int type,ucred pid);

#endif /* AFFLUENCE_PROCMOUNTER_DIY_H_ */