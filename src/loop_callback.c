#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "libcr.h"
#include "log_utils.h"

static int callback_fn(void* arg)
{
	int rc;
	puts("I must close the socket first!!!!!!");
	rc = cr_checkpoint(CR_CHECKPOINT_READY);
	return 0;
}

static int checkpoint(char* filename)
{
	int rc;
//	int rc,fd;
//	struct stat s;

	if (filename) {
		unlink(filename);
		rc = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	} else {
		rc = open("/dev/null", O_WRONLY);
	}

	if (rc < 0) {
		log_error("failed to open file");
		return rc;
	}


	cr_client_id_t client_id;
	client_id = cr_init();
	if (client_id < 0) {
		log_error("cr_init failed, client_id:%d\n", client_id);
		return -1;
	}
	log_info("cr_init, client_id:%d\n", client_id);

	cr_callback_id_t cb_id;
	void* cb_args = NULL;
	cb_id = cr_register_callback(callback_fn, cb_args, CR_SIGNAL_CONTEXT);
	if (cb_id < 0) {
		log_error("cr_register_callback failed, cb_id:%d\n", cb_id);
		return -1;
	}
	log_info("cr_register_callback, cb_id:%d\n", client_id);
	

	/*
		typedef struct cr_checkpoint_args {
			cr_version_t cr_version;
			cr_scope_t   cr_scope;
			pid_t        cr_target;

			int          cr_fd;
			int          cr_signal;
			unsigned int cr_timeout;
			unsigned int cr_flags;
		} cr_checkpoint_args_t;
	 */

	cr_checkpoint_args_t my_args;
	cr_initialize_checkpoint_args_t(&my_args);
	my_args.cr_fd = rc;
	my_args.cr_scope = CR_SCOPE_PROC;
	log_info("cr_initialize_checkpoint_args_t, cr_version:%d, cr_scope:%d, cr_target:%lu\n",
					my_args.cr_version, my_args.cr_scope, my_args.cr_target);
	log_info("cr_initialize_checkpoint_args_t, cr_fd:%d, cr_signal:%d, cr_timeout:%lu, cr_flags:%lu\n",
						my_args.cr_fd, my_args.cr_signal, my_args.cr_timeout, my_args.cr_flags);

	cr_checkpoint_handle_t ckpt_handle;
	rc = cr_request_checkpoint(&my_args, &ckpt_handle);
	if (rc < 0) {
		close(my_args.cr_fd);
		if (filename) {
			unlink(filename);
		}
		log_error("cr_request_checkpoint failed\n");
		return -1;
	}
	log_info("cr_request_checkpoint, chkpt_handle:%lu\n", ckpt_handle);

	int i = 0;
	for (i = 0; i < 10; i++) {
		printf("i:%d\n", i);
	}




	 

//      /* Request a checkpoint of ourself */
//     fd =crut_checkpoint_request(&my_handle, filename);
//    if (fd < 0)
//          {
//         LOGE("crut_checkpoint_request() unexpectedly returned 0x%x/n",fd);
//         return ;
//          }
//    rc = stat(filename, &s);
//    if (rc) {
//       LOGE("stat() unexpectedly returned%d/n", rc);
//       return;
//    } else {
//       LOGV("stat(context.%d) correctlyreturned 0/n", my_pid);
//    }
//    if (s.st_size == 0) {
//       LOGE("context file unexpectedlyempty/n");
//       return;
//    } else {
//       LOGV("context.%d isnon-empty/n", my_pid);
//    }
//    /* Reap the checkpoint request */
//    rc = crut_checkpoint_wait(&my_handle,fd);
//    if (rc < 0) {
//       LOGE("crut_checkpoint_wait() #1unexpectedly returned 0x%x/n", rc);
//       return;
//    }    }
	return 0;
}
/*
 * extern cr_callback_id_t
cr_register_callback(cr_callback_t	func,
		     void*		arg,
		     int		flags);
 */
int main() {
	int i;
	char *filename = "/tmp/ckpt.test";
	checkpoint(filename);
//	for (i = 0; i < 1000; i++) {
//		print
}
