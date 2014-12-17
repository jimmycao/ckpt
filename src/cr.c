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

extern int init()
{
	cr_client_id_t client_id;

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

	if (strlen(cr_snapshot.local_full_filename) == 0) {
		unlink(cr_snapshot.local_full_filename);
		cr_snapshot.fd = open(cr_snapshot.local_full_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	} else {
		cr_snapshot.fd = open("/dev/null", O_WRONLY);
	}
	if (cr_snapshot.fd < 0) {
		log_error("failed to open file");
		return -1;
	}

	client_id = cr_init();
	if (client_id < 0) {
		log_error("cr_init failed, client_id:%d\n", client_id);
		return -1;
	}
	log_info("cr_init successful, client_id:%d\n", client_id);
	return 0;
}

extern int register_signal_cb(cr_callback_t callback_fn, void* arg)
{
	cr_callback_id_t cb_id;
	cb_id = cr_register_callback(callback_fn, arg, CR_SIGNAL_CONTEXT);
	if (cb_id < 0) {
		log_error("register_signal_cb failed, cb_id:%d\n", cb_id);
		return -1;
	}
	log_info("register_signal_cb successful, cb_id:%d\n", cb_id);
}

extern int register_thread_cb(cr_callback_t callback_fn, void* arg)
{
	cr_callback_id_t cb_id;
	cb_id = cr_register_callback(callback_fn, arg, CR_THREAD_CONTEXT);
	if (cb_id < 0) {
		log_error("register_thread_cb failed, cb_id:%d\n", cb_id);
		return -1;
	}
	log_info("register_thread_cb successful, cb_id:%d\n", cb_id);
}


extern int finalize()
{

}



