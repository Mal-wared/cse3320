#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Constants
#define MAX_DATA 10000
#define MAX_STRING_LENGTH 32

// Error codes
#define ERROR_FILE_NOT_FOUND 1

/**
 * @file assignment2.c
 * @brief Get experience creating and running concurrent processes and threads.
 *
 * Part 1:
 *  You will create processes, run processes in parallel, and pass information
 *  between processes. \n
 *  The data to be processed are “tuples” which are numbers and associated text
 *  strings, that is lines of CSV separated values, we wish to sort these.
 *
 *  Part 2:
 *  You will create a process and threads, run in parallel, and pass information
 *  between threads. \n
 *  You will use synchronization operations to avoid overwriting
 *  shared resources.
 *
 * @author Name:       Nicholas Nhat Tran \n
 *         Student ID: 1002027150
 * @date 2024-10-19
 */

void initPart1();
int getData(int numData[MAX_DATA], char strData[MAX_DATA][MAX_STRING_LENGTH]);

int main(int argc, char *argv[])
{
    initPart1();

    return 0;
}

void initPart1()
{

    int numData[MAX_DATA];
    char strData[MAX_DATA][MAX_STRING_LENGTH];

    int getDataResult = getData(numData, strData);
    if (getDataResult == ERROR_FILE_NOT_FOUND)
    {
        printf("Error: File not found.\n");
        return;
    }

    for (int i = 0; i < MAX_DATA; i++)
    {
        printf("%d,%s\n", numData[i], strData[i]);
    }
}

int getData(int numData[MAX_DATA], char strData[MAX_DATA][MAX_STRING_LENGTH])
{
    FILE *fp;
    char dataLine[MAX_STRING_LENGTH];
    char *token;
    const char *delim = ",";

    fp = fopen("data.csv", "r");
    if (fp == NULL)
    {
        return ERROR_FILE_NOT_FOUND;
    }

    int count = 0;
    while (fgets(dataLine, MAX_STRING_LENGTH, fp) != NULL)
    {
        token = strtok(dataLine, delim);
        if (token != NULL)
        {
            strcpy(strData[count], token);

            token = strtok(NULL, delim);
            if (token != NULL)
            {
                numData[count] = atoi(token);
                count++;
            }
        }
    }

    fclose(fp);
    return 0;
}