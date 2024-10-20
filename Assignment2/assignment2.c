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
 *  References:
 *  - https://www.geeksforgeeks.org/insertion-sort-algorithm/
 *
 * @author Name:       Nicholas Nhat Tran \n
 *         Student ID: 1002027150
 * @date 2024-10-19
 */

void initSort();
int getData(int numData[MAX_DATA], char strData[MAX_DATA][MAX_STRING_LENGTH]);
void insertionSort(int numData[MAX_DATA], char strData[MAX_DATA][MAX_STRING_LENGTH], int dataListSize);

int main(int argc, char *argv[])
{
    initSort();
    return 0;
}

void initSort()
{
    // 1. Get the data set
    int numData[MAX_DATA];
    char strData[MAX_DATA][MAX_STRING_LENGTH];

    int getDataResult = getData(numData, strData);
    if (getDataResult == ERROR_FILE_NOT_FOUND)
    {
        printf("Error: File not found.\n");
        return;
    }

    // 2. Sort the data, that is each "record" (line, tuple) in ascending order by numeric value
    // 3. “Instrument” your program (time it).
    int dataListSize = sizeof(numData) / sizeof(numData[0]);

    // Start the timer
    clock_t startTime = clock();

    // Execute the sorting algorithm
    insertionSort(numData, strData, dataListSize);

    // End the timer
    clock_t endTime = clock();
    double elapsedTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    // Print time taken to sort the algorithm
    printf("Insertion sort took %.3f seconds\n", elapsedTime);
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

// Source code pulled from https://www.geeksforgeeks.org/insertion-sort-algorithm/
void insertionSort(int numData[MAX_DATA], char strData[MAX_DATA][MAX_STRING_LENGTH], int dataListSize)
{
    for (int i = 1; i < dataListSize; ++i)
    {
        int key = numData[i];
        char strKey[MAX_STRING_LENGTH];
        strcpy(strKey, strData[i]);
        int j = i - 1;

        while (j >= 0 && numData[j] > key)
        {
            numData[j + 1] = numData[j];
            strcpy(strData[j + 1], strData[j]);
            j = j - 1;
        }
        numData[j + 1] = key;
        strcpy(strData[j + 1], strKey);
    }
}