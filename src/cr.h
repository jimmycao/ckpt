#ifndef CR_H_
#define CR_H_

#include "constant.h"

typedef struct cr_snapshot {
	char local_location[PATH_MAX_LEN];
	char remote_location[PATH_MAX_LEN];
	char context_filename[FILE_NAME_MAX_LEN];
	char local_full_filename[PATH_MAX_LEN+FILE_NAME_MAX_LEN];
} cr_snapshot_t;


int init();

#endif /* CR_H_ */
