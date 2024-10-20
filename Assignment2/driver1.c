/* Driver Program for Assignment 2 Part 1*/

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

int main(void)
{
	time_t t;
	char s[256];
	int i, j;
	pid_t p, pp;

	t = time(NULL);
	printf("Driver Time: %s\n", ctime(&t));
	p = getpid();
	pp = getppid();
	printf("Driver PID: %d   Driver PPID: %d \n", p, pp);
	char *args[] = {"./assignment2", NULL};

	for (int i = 0; i < 10; i++)
	{
		p = fork();
		if (p == 0)
		{
			// Child created
			printf("Child %d Separator -------------------\n", i + 1);
			execv(args[0], args);
		}
		else if (p > 0)
		{
			printf("Parent created child %d with PID: %d\n", i + 1, p);
		}
		else
		{
			printf("Error creating child process\n");
		}
	}

	sleep(1);
	t = time(NULL);
	printf("Driver Time again: %s\n", ctime(&t));
	exit(0);
}
