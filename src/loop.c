#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	int i;
	for (i = 0; i < 1000; i++) {
		fprintf(stderr, "%d\n", i);	
		sleep(1);
	}
	return 0;
}
