#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define SIZE 10

int main() {
    int nums[SIZE], temp[SIZE];

    srand((int)time(NULL));

    for (int i=0; i < SIZE; i++)
    {
        nums[i] = rand() % SIZE;
        temp[i] = -1;
        printf("%d ", nums[i]);
    }
    printf("\n");

    #pragma omp parallel default(none) shared(nums, temp)
    {
        #pragma omp for
        for (int i=0; i < SIZE; i++)
        {
            int currentNum = nums[i], position = 0;

            for (int k = 0; k < SIZE; k++)
                if (currentNum > nums[k] || nums[k] == currentNum && i > k)
                    position++;
            
            #ifdef _OPENMP
                printf("Thread %d doing iteration %d calculated position %d for number %d at %x\n", omp_get_thread_num(), i, position, currentNum, &currentNum);
            #endif
            temp[position] = currentNum;
        }

        #pragma omp for
        for (int i=0; i < SIZE; i++)
            nums[i] = temp[i];
    }

    for (int i=0; i < SIZE; i++)
    {
        printf("%d ", nums[i]);
    }
    printf("\n");

    return 0;
}
