#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_array.h"
#include "shell_list.h"
#include "sequence.h"

int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        return EXIT_FAILURE;
    }

    char *mode = argv[1];
    char *input_file = argv[2];
    char *output_file = argv[3];

    long n_comp = 0;

    //array mode
    if (strcmp(mode, "-a") == 0) {
        int size = 0;
        long *array = Array_Load_From_File(input_file, &size);
        
        if (array == NULL || size == 0) {
            // Invalid or empty input file, don't create output file
            printf("0\n"); // 0 comparisons for empty file
            return EXIT_SUCCESS;
        }

        Array_Shellsort(array, size, &n_comp);

        if (Array_Save_To_File(output_file, array, size) < 0) {
            free(array);
            //printf("array to file fail");
            return EXIT_FAILURE;
        }

        printf("%ld\n", n_comp);
        free(array);
    }

    //linked list mode
    else if (strcmp(mode, "-l") == 0) {
        int status = 0;
        Node *list = List_Load_From_File(input_file, &status);
        
        if (status != 0 || list == NULL) {
            // Invalid or empty input file, don't create output file
            if (status == 0) {
                printf("0\n"); // 0 comparisons for empty file
                return EXIT_SUCCESS;
            }
            return EXIT_FAILURE;
        }

        list = List_Shellsort(list, &n_comp);
        
        if (List_Save_To_File(output_file, list) < 0) {
            while (list != NULL) {
                Node *temp = list;
                list = list->next;
                free(temp);
            }
            //printf("List save to file fail");
            return EXIT_FAILURE;
        }

        printf("%ld\n", n_comp);
        
        while (list != NULL) { 
            Node *temp = list;
            list = list->next;
            free(temp);
        }
    }
    // Invalid mode
    else {
        //printf("Failed");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
