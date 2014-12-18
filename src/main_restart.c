#include <stdio.h>
#include <stdlib.h>

#include "libcr.h"
#include "log_utils.h"

#include "cr.h"
#include "conf.h"
#include "constant.h"



int main() {
	int rc;
	pid_t pid_restarted = -1;
	cr_snapshot_t snapshot;

	char *remote_location = "/tmp";
	char *local_location = "/tmp";
	char *context_filename = "ckpt.28558.node1.1418946766";


	strncpy(snapshot.remote_location, remote_location, strlen(remote_location));
	strncpy(snapshot.local_location, local_location, strlen(local_location));
	strncpy(snapshot.context_filename, context_filename, strlen(context_filename));

	rc = restart(&snapshot, false, &pid_restarted);
	if (rc != RT_SUCCESS) {
		log_error("failed to restart()");
		return -1;
	}


	return 0;
}




