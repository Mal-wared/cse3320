/* Driver Program
 */

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define MAX_THREADS 10

void *threadFunction(void *arg);

int main(void)
{
	time_t t;
	pthread_t threadList[MAX_THREADS];
	int threadNums[MAX_THREADS];

	t = time(NULL);
	printf("Driver Time: %s\n", ctime(&t));

	for (int i = 0; i < MAX_THREADS; i++)
	{
		threadNums[i] = i + 1;
		if (pthread_create(&threadList[i], NULL, threadFunction, (void *)&threadNums[i]))
		{
			printf("Error creating thread\n");
			return 1;
		}
		else
		{
			printf("Thread %d created\n", i + 1);
		}
	}

	for (int i = 0; i < MAX_THREADS; i++)
	{
		pthread_join(threadList[i], NULL);
	}

	t = time(NULL);
	printf("Driver Time again: %s\n", ctime(&t));
	exit(0);
}

void *threadFunction(void *arg)
{
	int threadNum = *((int *)arg); // The thread # is passed in through as an argument
	printf("Thread %d created with TID: %lu\n", threadNum, pthread_self());

	// Like execv, but for threads
	char *args[] = {"./assignment2", "1", NULL};
	printf("Thread %d executing \"./assignment2\"\n", threadNum);

	system("./assignment2");

	pthread_exit(NULL); // Thread completes and exits
}
