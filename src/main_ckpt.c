#include <stdio.h>
#include <stdlib.h>

#include "libcr.h"
#include "log_utils.h"

#include "cr.h"
#include "conf.h"
#include "constant.h"



int main() {
	int rc;
	int i = 0;
	cr_snapshot_t snapshot;
	pid_t pid_restarted = -1;

	for (i = 0; i < 10; i++) {
		printf("i:%d\n", i);
	}

	rc = init();
	if (rc != RT_SUCCESS) {
		log_error("failed to init()");
		return -1;
	}

	rc = register_signal_cb(default_signal_callback_fn, NULL);
	if (rc != RT_SUCCESS) {
		log_error("failed to register_signal_cb");
		return -1;
	}

	rc = checkpoint(&snapshot);
	if (rc != RT_SUCCESS) {
		log_error("failed to checkpoint()");
		return -1;
	}

	for (i = 10; i < 20; i++) {
		printf("i:%d\n", i);
	}

	return 0;
}




