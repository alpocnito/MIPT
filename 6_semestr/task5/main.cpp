#include "pthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define handle_error_en(er, msg) \
	do {errno = er; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
	do {perror(msg); exit(EXIT_FAILURE); } while (0)


void* thread_func(void* v_num)
{
	unsigned num = *((unsigned*)v_num);

	printf("%3u thread id: %lu\n", num, pthread_self());
	return NULL;
}

int main(int argc, char** argv)
{
	assert(argc == 2);
	unsigned num_threads = unsigned(atoi(argv[1]));
	if (num_threads == 1)
		return 0;

	pthread_t* thread_ids = (pthread_t*)calloc(num_threads, sizeof(thread_ids[0]));
	unsigned*  args       = (unsigned*) calloc(num_threads, sizeof(args[0]));

	for (unsigned j = 0; j < num_threads; ++j)
	{
		args[j] = j;
		
		int ret = pthread_create(&(thread_ids[j]), NULL, thread_func, (void*)&(args[j]));
		if (ret != 0)
			handle_error_en(ret, "pthread_create");
	}
	
	for (unsigned j = 0; j < num_threads; ++j)
	{
		int ret = pthread_join(thread_ids[j], NULL);
		if (ret != 0)
			handle_error_en(ret, "pthread_join");
	}
	
	free(thread_ids);
	free(args);
	return 0;
}
