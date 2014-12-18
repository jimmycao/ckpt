#ifndef CR_H_
#define CR_H_

#include "constant.h"

typedef struct cr_snapshot {
	char local_location[PATH_MAX_LEN];
	char remote_location[PATH_MAX_LEN];
	char context_filename[FILE_NAME_MAX_LEN];
	char local_full_filename[PATH_MAX_LEN+FILE_NAME_MAX_LEN];
	int  fd;
} cr_snapshot_t;

typedef struct callback_arg {
	int* fd_array;
	int fd_array_size;
} callback_arg_t;

extern pid_t my_pid;

extern int init();

extern int default_signal_callback_fn(void* arg);

extern int register_signal_cb(cr_callback_t callback_fn, void* arg);

extern int register_thread_cb(cr_callback_t callback_fn, void* arg);

extern int checkpoint(cr_snapshot_t* snapshot);

extern int restart(cr_snapshot_t* snapshot);

#endif /* CR_H_ */
