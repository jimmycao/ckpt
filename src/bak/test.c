CheckPoint cp=newCheckPoint();
try {
   //Start profiling the zygote initialization.
SamplingProfilerIntegration.start();
  preloadClasses();
  preloadResources();
  cp.checkPoint("/data/zygote.blcr");
  registerZygoteSocket();

    /* Reap the checkpoint request */
    rc = crut_checkpoint_wait(&my_handle,fd);
    if (rc < 0) {
       LOGE("crut_checkpoint_wait() #1unexpectedly returned 0x%x/n", rc);
       return;
    }    }}




#define _LARGEFILE64_SOURCE 1   /* For O_LARGEFILE */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "crut_util.h"



int
crut_checkpoint_wait(cr_checkpoint_handle_t *handle_p, int fd) {
    int rc, save_err;

    do {
        rc = cr_poll_checkpoint(handle_p, NULL);
        if (rc < 0) {
            if ((rc == CR_POLL_CHKPT_ERR_POST) && (errno == CR_ERESTARTED)) {
                /* restarting -- not an error */
                rc = 1; /* Signify RESTART to caller */
            } else if (errno == EINTR) {
                /* poll was interrupted by a signal -- retry */
continue;
            } else {
                /* return the error to caller */
                break;
            }
        } else if (rc == 0) {
            fprintf(stderr, "cr_poll_checkpoint returned unexpected 0/n");
            rc = -1;
            goto out;
        } else {
            rc = 0; /* Signify CONTINUE to caller */
}
    } while (rc < 0);

    save_err = errno;
#if 0 // Nothing in the testsuite needs this, but your APP might want it.
    (void)fsync(fd);
#endif
    (void)close(fd);
    errno = save_err;

out:
    return rc;
}

int
crut_checkpoint_block(const char *filename) {
    cr_checkpoint_handle_t my_handle;
    int ret, fd, save_err;

    fd = crut_checkpoint_request(&my_handle, filename);
    if (fd < 0) return fd;

    ret = crut_checkpoint_wait(&my_handle, fd);

    save_err = errno;
    (void)close(fd);
    errno = save_err;

    return ret;
}
