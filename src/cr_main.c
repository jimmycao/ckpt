#include <stdio.h>
#include <stdlib.h>

#include "libcr.h"
#include "log_utils.h"

#include "cr.h"
#include "conf.h"
#include "constant.h"


static int callback_fn(void* arg)
{
	int rc;
	puts("I must close the socket first!!!!!!");
	rc = cr_checkpoint(CR_CHECKPOINT_READY);
	return 0;
}

int main() {
	int rc;

	rc = init();
	if (rc < 0) {
		log_error("failed to init()");
		return -1;
	}

	rc = register_signal_cb(callback_fn, NULL);
	if (rc < 0) {
		log_error("failed to register_signal_cb");
		return -1;
	}


	rc = checkpoint();
	if (rc < 0) {
		log_error("failed to checkpoint()");
		return -1;
	}

	int i = 0;
	for (i = 0; i < 10; i++) {
		printf("i:%d\n", i);
	}
	return 0;
}



