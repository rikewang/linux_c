#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char **argv)
{
	int c;
	int digit_optind = 0; /* what's fuck */

	while(1)
	{
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;

		static sturct option long_options[] = {
			{"add", 1, 0, 0},
			{"append", 0, 0, 0},
			{"delete", 1, 0, 0},
			{"verbose", 0, 0, 0},
			{"create", 1, 0, 'c'},
			{"file", 1, 0, 0},
			{0, 0, 0 ,0}
		}
	}

}