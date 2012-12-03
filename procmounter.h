#ifndef AFFLUENCE_PROCMOUNTER_H_
#define AFFLUENCE_PROCMOUNTER_H_

#define MOUNT_PROC	1
#define MOUNT_PTS		2

int procmount(int dir,int type);

#endif /* AFFLUENCE_PROCMOUNTER_H_ */