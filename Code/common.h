#ifndef COMMON_H_
#define COMMON_H_

#define QUEUE_NAME "/Queue"
#define MAX_SIZE 1024
#define MSG_StOP "exit"

#define CHECK(x) \
	do { \
		if (!(x)) { \
			fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
			perror(#x); \
			exit(-1); \
		} \
	} while (0) \

#endif
