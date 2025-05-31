#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_array.h"
#include "shell_list.h"
#include "sequence.h"

long *Array_Load_From_File(char *filename, int *size){

    *size = 0;

    FILE *file = fopen(filename, "rb");
    if (file == NULL){
        return NULL;
    }

    //file size
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET); //go back to start

    int num_elem = f_size / sizeof(long); //det # of long ints

    // Handle empty file case
    if (num_elem == 0) {
        fclose(file);
        *size = 0;
        return NULL;
    }

    long *array = malloc(num_elem * sizeof(long)); //to be returned
    if (array == NULL){
        fclose(file);
        return NULL;
    }

    //# of long in array
    *size = fread(array, sizeof(long), num_elem, file);

    fclose(file);
    return array; 
    
}

int Array_Save_To_File(char *filename, long *array, int size){

    FILE *file = fopen(filename, "wb");
    if (array == NULL || file == NULL) {
        return -1;
    }

    //# of long in array
    size_t longs = fwrite(array, sizeof(long), size, file);
    
    fclose(file);

    return longs;
}

void Array_Shellsort(long *array, int size, long *n_comp) {
    *n_comp = 0;
    
    // Handle empty array case
    if (array == NULL || size <= 0) {
        return;
    }

    int seq_size;
    long *gaps = Generate_2p3q_Seq(size, &seq_size);

    if (gaps == NULL || seq_size == 0) {
        // insertion sort if seq gen fails
        for (int i = 1; i < size; i++) {
            long temp = array[i];
            int j = i - 1;
            
            // move elements that are greater than temp
            while (j >= 0) {
                (*n_comp)++;
                if (array[j] > temp) {
                    array[j + 1] = array[j];
                    j--;
                } else {
                    break;
                }
            }
            array[j + 1] = temp;
        }
        free(gaps);
        return;
    }

    // shell sort with optimized gap sequence
    for (int k = seq_size - 1; k >= 0; k--) {
        long gap = gaps[k];
        
        // insertion sort for this gap
        for (int i = gap; i < size; i++) {
            long temp = array[i];
            int j = i;
            
            // move elements that are greater than temp
            while (j >= gap) {
                (*n_comp)++;
                if (array[j - gap] > temp) {
                    array[j] = array[j - gap];
                    j -= gap;
                } else {
                    break;
                }
            }
            array[j] = temp;
        }
    }

    free(gaps);
}
