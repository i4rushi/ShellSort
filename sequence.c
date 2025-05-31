#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_array.h"
#include "shell_list.h"
#include "sequence.h"

long* Generate_2p3q_Seq(int n, int* seq_size) {
    *seq_size = 0;
    
    if (n <= 1) {
        *seq_size = 0;
        return NULL;
    }
    
    int max_elements = 0;
    
    int max_p = 0;
    long* powers2 = (long*)malloc(sizeof(long) * 32);
    if (powers2 == NULL) {
        *seq_size = 0;
        return NULL;
    }
    
    powers2[0] = 1;
    while (powers2[max_p] * 2 < n && max_p < 31) {
        max_p++;
        powers2[max_p] = powers2[max_p - 1] * 2;
    }

    int max_q = 0;
    long* powers3 = (long*)malloc(sizeof(long) * 32);
    if (powers3 == NULL) {
        free(powers2);
        *seq_size = 0;
        return NULL;
    }
    
    powers3[0] = 1;
    while (powers3[max_q] * 3 < n && max_q < 20) { 
        max_q++;
        powers3[max_q] = powers3[max_q - 1] * 3;
    }

    max_elements = 1;
    for (int p = 0; p <= max_p; p++) {
        for (int q = 0; q <= max_q; q++) {
            long value = powers2[p] * powers3[q];
            if (value < n && value > 1) {
                max_elements++;
            }
        }
    }

    if (max_elements <= 1) {
        free(powers2);
        free(powers3);
        *seq_size = 0;
        return NULL;
    }

    long* seq = (long*)malloc(max_elements * sizeof(long));
    if (seq == NULL) {
        free(powers2);
        free(powers3);
        *seq_size = 0;
        return NULL;
    }

    seq[(*seq_size)++] = 1;
    
    for (int p = 0; p <= max_p; p++) {
        for (int q = 0; q <= max_q; q++) {
            long value = powers2[p] * powers3[q];
            if (value < n && value > 1) {
                seq[(*seq_size)++] = value;
            }
        }
    }

    for (int i = 1; i < *seq_size; i++) {
        long key = seq[i];
        int j = i - 1;
        while (j >= 0 && seq[j] > key) { 
            seq[j + 1] = seq[j];
            j--;
        }
        seq[j + 1] = key;
    }

    free(powers2);
    free(powers3);
    return seq;
}

// int main(int argc, char** argv) {
//     if (argc != 2) {
//         printf("Usage: %s <size>\n", argv[0]);
//         return EXIT_FAILURE;
//     }

//     int n = atoi(argv[1]);
//     int seq_size = 0;

//     long* sequence = Generate_2p3q_Seq(n, &seq_size);
    
//     // if (sequence == NULL) {
//     //     printf("Failed to generate sequence\n");
//     //     return EXIT_FAILURE;
//     // }

//     printf("Generated sequence of size %d:\n", seq_size);
//     for (int i = 0; i < seq_size; i++) {
//         printf("%ld ", sequence[i]);
//     }
//     printf("\n");

//     free(sequence);
//     return EXIT_SUCCESS;
// }