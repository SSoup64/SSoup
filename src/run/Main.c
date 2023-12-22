#include <assert.h>
#include <string.h>

#include "./Run.c"

int main(int argc, char **argv)
{
	assert(argc >= 2);
	
	run(strdup(argv[1]));
}
