#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#include "libcr.h"
#include "cr.h"

#include "conf.h"
#include "constant.h"
#include "log_utils.h"
#include "argv.h"

static pid_t my_pid = -1;

static cr_snapshot_t cr_snapshot;

extern int init()
{
	int rc;
	char hostname[256];
	cr_client_id_t client_id;

	my_pid = getpid();
	rc = gethostname(hostname, 255);
	if (rc < 0) {
		log_error("failed to gethostname()");
		return RT_ERROR;
	}
	time_t tm = time(NULL);

	strncpy(cr_snapshot.local_location, LOCAL_LOCATION, PATH_MAX_LEN - 1);
	strncpy(cr_snapshot.remote_location, REMOTE_LOCATION, PATH_MAX_LEN - 1);

	snprintf(cr_snapshot.context_filename, FILE_NAME_MAX_LEN - 1, "ckpt.%lu.%s.%lu", my_pid, hostname, tm);
	snprintf(cr_snapshot.local_full_filename, PATH_MAX_LEN + FILE_NAME_MAX_LEN - 1, "%s/%s",
	                cr_snapshot.local_location, cr_snapshot.context_filename);
	log_info(
	                "initing, cr_snapshot.local_location:%s, cr_snapshot.remote_location:%s, cr_snapshot.context_filename:%s, cr_snapshot.local_full_filename:%s",
	                cr_snapshot.local_location, cr_snapshot.remote_location,
	                cr_snapshot.context_filename, cr_snapshot.local_full_filename);

	if (strlen(cr_snapshot.local_full_filename) != 0) {
		unlink(cr_snapshot.local_full_filename);
		cr_snapshot.fd = open(cr_snapshot.local_full_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	} else {
		cr_snapshot.fd = open("/dev/null", O_WRONLY);
	}
	if (cr_snapshot.fd < 0) {
		log_error("failed to open file");
		return RT_ERROR;
	}

	client_id = cr_init();
	if (client_id < 0) {
		log_error("cr_init failed, client_id:%d", client_id);
		return RT_ERROR;
	}
	log_info("cr_init successful, client_id:%d", client_id);
	return RT_SUCCESS;
}

extern int default_signal_callback_fn(void* arg)
{
	int i;
	int rc;
	const struct cr_checkpoint_info *ckpt_info = NULL;

	if (arg != NULL) {
		callback_arg_t* cb_arg_p = (callback_arg_t*) arg;
		for (i = 0; i < cb_arg_p->fd_array_size; i++) {
			close(cb_arg_p->fd_array[i]);
		}
	}

	ckpt_info = cr_get_checkpoint_info();

	if (ckpt_info->requester == my_pid && !IF_CHECKPOINT_MYSELF) {
		rc = cr_checkpoint(CR_CHECKPOINT_OMIT);
	}  else {
		rc = cr_checkpoint(CR_CHECKPOINT_READY);
	}
	return RT_SUCCESS;
}

extern int register_signal_cb(cr_callback_t callback_fn, void* arg)
{
	cr_callback_id_t cb_id;
	cb_id = cr_register_callback(callback_fn, arg, CR_SIGNAL_CONTEXT);
	if (cb_id < 0) {
		log_error("register_signal_cb failed, cb_id:%d", cb_id);
		return RT_ERROR;
	}
	log_info("register_signal_cb successful, cb_id:%d", cb_id);
	return RT_SUCCESS;
}



extern int register_thread_cb(cr_callback_t callback_fn, void* arg)
{
	cr_callback_id_t cb_id;
	cb_id = cr_register_callback(callback_fn, arg, CR_THREAD_CONTEXT);
	if (cb_id < 0) {
		log_error("register_thread_cb failed, cb_id:%d", cb_id);
		return RT_ERROR;
	}
	log_info("register_thread_cb successful, cb_id:%d", cb_id);
	return RT_SUCCESS;
}


extern int checkpoint(cr_snapshot_t* snapshot)
{
	cr_checkpoint_args_t my_args;
	cr_checkpoint_handle_t ckpt_handle;
	struct stat file_stat;
	int rc;

	/* init ckpt args */
	cr_initialize_checkpoint_args_t(&my_args);
	my_args.cr_fd = cr_snapshot.fd;
	my_args.cr_scope = CR_SCOPE_PROC;
	log_info("cr_initialize_checkpoint_args_t, cr_version:%d, cr_scope:%d, cr_target:%lu",
	                my_args.cr_version, my_args.cr_scope, my_args.cr_target);
	log_info(
	                "cr_initialize_checkpoint_args_t, cr_fd:%d, cr_signal:%d, cr_timeout:%lu, cr_flags:%lu",
	                my_args.cr_fd, my_args.cr_signal, my_args.cr_timeout, my_args.cr_flags);

	/* submit ckpt request */
	rc = cr_request_checkpoint(&my_args, &ckpt_handle);
	if (rc < 0) {
		close(cr_snapshot.fd);
		unlink(cr_snapshot.local_full_filename);
		log_error("cr_request_checkpoint failed");
		return RT_ERROR;
	}
	log_info("cr_request_checkpoint success, chkpt_handle:%lu", ckpt_handle);

	/* wait for checkpoint to finish */
	do {
		rc = cr_poll_checkpoint(&ckpt_handle, NULL);
		if (rc < 0) {
			if ((rc == CR_POLL_CHKPT_ERR_POST) && (errno == CR_ERESTARTED)) {
				/* Check if restarting. This is not an error. */
				break;
			} else if (errno == EINTR) {
				/* If Call was interrupted by a signal, retry the call */
				;
			} else {
				/* Otherwise this is a real error that need to deal with */
				return RT_ERROR;
			}
		}
	} while (rc < 0);

	/* stat the ckpt file */
	rc = stat(cr_snapshot.local_full_filename, &file_stat);
	if (rc < 0) {
		log_error("failed to stat file:%s", cr_snapshot.local_full_filename);
		return RT_ERROR;
	} else if (file_stat.st_size == 0) {
		log_error("ckpt file:%s is empty", cr_snapshot.local_full_filename);
		return RT_ERROR;
	}
	log_info("ckpt successful, ckpt file:%s size:%lu", cr_snapshot.local_full_filename, file_stat.st_size);

	/* Close the file */
	close(my_args.cr_fd);

	//TODO: /* transfer ckpt file to remote ckpt server */
	strncpy(snapshot->local_location, cr_snapshot.local_location, PATH_MAX_LEN - 1);
	strncpy(snapshot->remote_location, cr_snapshot.remote_location, PATH_MAX_LEN - 1);
	strncpy(snapshot->context_filename, cr_snapshot.context_filename, FILE_NAME_MAX_LEN - 1);

	return RT_SUCCESS;
}

/*
 	typedef struct cr_snapshot {
	char local_location[PATH_MAX_LEN];
	char remote_location[PATH_MAX_LEN];
	char context_filename[FILE_NAME_MAX_LEN];
	char local_full_filename[PATH_MAX_LEN+FILE_NAME_MAX_LEN];
	int  fd;
} cr_snapshot_t;
 */
extern int restart(cr_snapshot_t* snapshot, bool spawn_child, pid_t *child_pid)
{
	char restart_cmd[CMD_MAX_LEN];
	char **argv;
	int rt;

	if (strlen(snapshot->remote_location) == 0 || strlen(snapshot->context_filename) == 0) {
		log_error("restart: snapshot->remote_location or snapshot->context_filename is empty");
		return RT_ERROR;
	}
	//TODO: /* here we read ckpt file from remote ckpt server to local FS */
	strncpy(snapshot->local_location, "/tmp", 256);

	snprintf(snapshot->local_full_filename, PATH_MAX_LEN + FILE_NAME_MAX_LEN - 1, "%s/%s",
	                snapshot->local_location, snapshot->context_filename);

	snprintf(restart_cmd, CMD_MAX_LEN - 1, "cr_restart %s", snapshot->local_full_filename);

	puts(restart_cmd);

	if (!spawn_child) {
		argv = argv_split(restart_cmd, ' ');
		rt = execvp("cr_restart", argv);
		if (rt < 0) {
			log_error("restart: failed to restart %s via execvp", snapshot->local_full_filename);
			return RT_ERROR;
		}
		argv_free(argv);
	} else {


	}

	return RT_SUCCESS;
}

