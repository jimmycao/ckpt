#include <stdio.h>
#include <stdlib.h>

int main() {
	int i;
	for (i = 0; i < 1000; i++) {
		printf("%d\n", i);	
		sleep(1);
	}
	return 0;
}
