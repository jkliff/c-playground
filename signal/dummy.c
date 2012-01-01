/*
 * dummy.c
 *
 *  Created on: Oct 14, 2011
 *      Author: john
 */
#include <stdio.h>
#include<time.h>

int main () {
	for (;;) {
		printf (".");
		fflush (stdout);
		sleep (1);
	}
}
