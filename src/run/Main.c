#include <assert.h>
#include <string.h>

#include "./Run.c"

#define ARGS_COUNT 2

int main(int argc, char **argv)
{
	assert(ARGS_COUNT == argc);
	
	run(strdup(argv[1]));
}
