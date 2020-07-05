/*
 ============================================================================
 Name        : detector_deol.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int processedProcesses; // lol
int processCount;
int resourceCount;
int deadlocked[256]; // Arrays involved in deadlock
int blocked[256]; // Arrays to be terminated
int completed[256]; // Array that shows completed processes

int inBlocked(int);
int inCompleted(int);
int inDeadlocked(int);
void appendBlocked(int);
void appendCompleted(int);
void appendDeadlocked(int);
int compare(int, int[][resourceCount], int[]);
void unallocate(int, int[][resourceCount], int[]);
void printArray(int[]);

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);

	scanf("%d", &processCount);
	scanf("%d", &resourceCount);

	int requestArray[processCount][resourceCount]; // Ri
	int allocatedArray[processCount][resourceCount]; // Ai
	int unallocatedArray[resourceCount]; // Ui

	for (int i = 0; i < processCount; i++) {
		deadlocked[i] = 0;
		blocked[i] = 0;
		completed[i] = 0;
	}

	// Populate 2d arrays
	for (int row = 0; row < processCount; row++) {
		for (int column = 0; column < resourceCount; column++) {
			scanf("%d", &requestArray[row][column]);
		}
	}
	for (int row = 0; row < processCount; row++) {
		for (int column = 0; column < resourceCount; column++) {
			scanf("%d", &allocatedArray[row][column]);
		}
	}

	for (int column = 0; column < resourceCount; column++) {
		scanf("%d", &unallocatedArray[column]);
	}

	int row = 0;
	int block = 0;
	while (processedProcesses < processCount) {
		if (inCompleted(row) == 1 || inBlocked(row) == 1) {
			row++;
			continue;
		}
		if (compare(row, requestArray, unallocatedArray) == 1) {
			unallocate(row, allocatedArray, unallocatedArray);
			appendCompleted(row);
			processedProcesses++;
			row++;
			if(row == processCount){
				row = 0;
				block = 1;
			}
			continue;
		} else if(row == processCount - 1 || block == 1){
			// We now have a deadlock
			int blocked = 0;
			for(row = 0; row < processCount; row++){
				if (inCompleted(row) == 1 || inBlocked(row) == 1) {
					continue;
				}
				appendDeadlocked(row);
				if(blocked == 0){
					unallocate(row, allocatedArray, unallocatedArray);
					appendBlocked(row);
					blocked = 1;
					processedProcesses++;
				}
			}
			row = 0;
		} else if (row < processCount) {
			row++;
		} else{
			row = 0;
		}
	}


	printArray(deadlocked);

	printArray(blocked);

	printArray(completed);

	exit(0);
}

int inBlocked(int row) {
	for (int i = 0; i < processCount; i++) {
		if (blocked[i] == (row + 1))
			return 1;
	}
	return 0;
}

int inCompleted(int row) {
	for (int i = 0; i < processCount; i++) {
		if (completed[i] == (row + 1))
			return 1;
	}
	return 0;
}

int inDeadlocked(int row) {
	for (int i = 0; i < processCount; i++) {
		if (deadlocked[i] == (row + 1))
			return 1;
	}
	return 0;
}

void appendBlocked(int row){
	for (int i = 0; i < processCount; i++)
		if (blocked[i] == 0) {
			blocked[i] = (row + 1);
			return;
		}
}

void appendCompleted(int row){
	for (int i = 0; i < processCount; i++)
		if (completed[i] == 0) {
			completed[i] = (row + 1);
			return;
		}
}

void appendDeadlocked(int row){
	for (int i = 0; i < processCount; i++)
		if (deadlocked[i] == 0) {
			deadlocked[i] = (row + 1);
			return;
		}
}

void printArray(int array[]) {
	for (int column = 0; column < processCount; column++) {
		if(array[column] == 0)
			continue;
		printf("%d ", array[column]);
	}
	printf("\n");
}

void unallocate(int row, int allocatedArray[][resourceCount],
		int unallocatedArray[]) {
	for (int column = 0; column < resourceCount; column++) {
		unallocatedArray[column] += allocatedArray[row][column];
		allocatedArray[row][column] = 0;
	}
}

int compare(int row, int requestArray[][resourceCount], int unallocatedArray[]) {

	for (int column = 0; column < resourceCount; column++)
		if (requestArray[row][column] > unallocatedArray[column])
			return 0;
	return 1;
}
