/*
COP3502 Spring 2024 Bonus Assignment
Copyright 2024 Fogarty, James
Implements heap, merge, bubble, insertion,
and selection sorting algorithms
*/

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> //string.h needed for memcpy

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

void swap(int* a, int* b) { //simple swap function used in sorting algorithms
 int temp = *a;
*a = *b;
*b = temp;
}

//function sorts array data according to maximum heap property
void heapIt(int arr[], int N, int i) { 
int biggest = i; //set biggest to root
int l = 2 * i + 1; //calculate left and right indices for root's 'children'
int r = 2 * i + 2;
 
if (l<N && arr[l]>arr[biggest]) biggest = l; //if left > root and not OOB, biggest is l
if (r<N && arr[r]>arr[biggest]) biggest=r; //if right > biggest and not OOB, biggest is r
if (biggest!=i) { //if root isn't the biggest, swap the biggest with the root
 	swap(&arr[i],&arr[biggest]);
    heapIt(arr,N,biggest); //reheap the shifted branch
}

}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n, int i) {

// Convert array into a maximum heap by running heapIt function n/2 times
for (int i = n / 2 - 1; i >= 0; i--) heapIt(arr, n, i);
// Sort the maximum heap with n swaps and reheaps
for (int i = n - 1; i >= 0; i--) {
 	swap(&arr[0], &arr[i]);
    heapIt(arr, i, 0);
}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r) {
if (l<r) {
	int mid = l+(r-l)/2;
	mergeSort(pData,l,mid); //call ms on first and second half of the array
	mergeSort(pData,mid+1,r);

	int num1 = mid-l+1; //the size of the left array is mid minus left + 1
	int num2= r-mid; //the size of the right array is right minus mid

	int * lAry = (int*)Alloc(sizeof(int)*num1); //allocate temp arrays' space
	int * rAry = (int*)Alloc(sizeof(int)*num2);

	for (int i = 0;i<num1;i++) { //transfer data to temp arrays
		lAry[i]=pData[i+l];
	}
	for (int i = 0;i<num2;i++) {
		rAry[i]=pData[i+mid+1];
	}
int i = 0;
int j = 0;
int k = l;

while (i < num1 && j < num2) { //add l and r array's data to the main array in order
        if (lAry[i] <= rAry[j]) { 
            pData[k] = lAry[i]; 
            i++; 
        } 
        else { 
            pData[k] = rAry[j]; 
            j++; 
        } 
        k++; 
    } 
while (i < num1) { //append any leftover data to the end of the array
        pData[k] = lAry[i]; 
        i++; 
        k++; 
    }
	
while (j < num2) { //append any leftover data to the end of the array
        pData[k] = rAry[j]; 
        j++; 
        k++; 
    }
DeAlloc(lAry); //free the temp arrays
DeAlloc(rAry); 
}

}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n) { 
int i, least, j;
    for (i=1;i<n;i++) { //n-1 iterations
        least=pData[i]; //leftmost value of unsorted array is least
        j=i-1;
        while (j >= 0 && pData[j] > least) { //while least < arr[j] and j is in bounds
            pData[j+1] = pData[j]; //shift j's data one to the right
            j=j-1; //decrement j
        }
        pData[j+1] = least; //insert least's data in the 'hole' made by shifting values to the right
    }
	
}


// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n) { 
int i, j;
int swapped;
for (i=0;i<n-1;i++) { //sort the array n-1 times
    swapped = 0;
    for (j=0;j<n-i-1;j++) { //compare each element to its neighbor up to n-i-1
        if (pData[j]>pData[j+1]) { //if array[j]>array[j+1], swap them and set swapped to 1
            swap(&pData[j], &pData[j+1]);
            swapped = 1;
        }
    }
	if (swapped==0) break; //if the inner for loop resolves with no swaps, no further iteration is needed
    } 
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n) { 
int i, j, min; 
for (i=0;i<n-1;i++) { //n-1 iterations
    min = i; //set minimum's index to i
    for (j=i+1;j<n;j++) if (pData[j]<pData[min]) min = j; //iterate through everything after i in the array and find the minimum value
    if(min != i) swap(&pData[min], &pData[i]); //if i wasn't the index with the minimum value, swap i and the minimum value's index
} 
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r"); //opens input file for reading
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile) //if fopen succeeded
	{
		fscanf(inFile,"%d\n",&dataSz); //dataSz set to first number in input file
		*ppData = (int *)Alloc(sizeof(int) * dataSz); //int array of size dataSz allocated
		// Implement parse data block
		if (*ppData == NULL) //if alloc failed exit
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i) //scan dataSz number of integers and insert them in ppData array
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i; //simple pointer math so n gets to the right index
			*data = n;
		}

		fclose(inFile); //close file
	}
	
	return dataSz; //return size of array created
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy,dataSz,0);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
