#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include "libcr.h"
#include "log_utils.h"

#include "cr.h"
#include "conf.h"
#include "constant.h"


cr_snapshot_t cr_snapshot;

int init() {
	strncpy(cr_snapshot.local_location, LOCAL_LOCATION, PATH_MAX_LEN - 1);
	strncpy(cr_snapshot.remote_location, REMOTE_LOCATION, PATH_MAX_LEN - 1);
	time_t tm = time(NULL);
	snprintf(cr_snapshot.context_filename, FILE_NAME_MAX_LEN - 1, "ckpt.%lu", tm);
	snprintf(cr_snapshot.local_full_filename, PATH_MAX_LEN + FILE_NAME_MAX_LEN - 1, "%s/%s",
	                cr_snapshot.local_location, cr_snapshot.context_filename);
	log_info(
	                "initing, cr_snapshot.local_location:%s, cr_snapshot.remote_location:%s, cr_snapshot.context_filename:%s, cr_snapshot.local_full_filename:%s",
	                cr_snapshot.local_location, cr_snapshot.remote_location,
	                cr_snapshot.context_filename, cr_snapshot.local_full_filename);
	return 0;
}



