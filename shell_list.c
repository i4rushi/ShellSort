#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_array.h"
#include "shell_list.h"
#include "sequence.h"

Node *List_Load_From_File(char *filename, int *status){

    *status = 0;

    FILE *file = fopen(filename, "rb");
    if (file == NULL){
        *status = -1;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET); //go back to start

    int num_elem = f_size / sizeof(long);

    if (num_elem == 0){
        fclose(file);
        return NULL; 
    }

    Node *head = NULL;
    Node *current = NULL;
    long value;

    while (fread(&value, sizeof(long), 1, file) == 1){
        Node *new_node = (Node *)malloc(sizeof(Node));  //malloc per node
        if (new_node == NULL){
            *status = -1;
            fclose(file);

            while (head != NULL){  //clean up
                Node *temp = head;
                head = head->next;
                free(temp);
            }
            return NULL;
        }

        new_node->value = value; 
        new_node->next = NULL;

        if (current == NULL){  
            head = new_node;
            current = new_node;
        }
        else{
            current->next = new_node;
            current = new_node;
        }
    }

    fclose(file);
    return head;

}

int List_Save_To_File(char *filename, Node *list){
    
    FILE *file = fopen(filename, "wb");
    if (list == NULL || file == NULL){
        return -1;
    }

    int count = 0;
    Node *current = list;
    while (current != NULL){
        if (fwrite(&(current->value), sizeof(long), 1, file) != 1){
            fclose(file);
            return -1;
        }
        current = current->next;
        count++;
    }

    fclose(file);
    return count;
}

Node *List_Shellsort(Node *list, long *n_comp) {
    Node *head = list;
    int size = 0;
    *n_comp = 0;
    
    // Handle empty list case
    if (head == NULL) {
        return NULL;
    }

    // Count nodes in linked list
    for(; head != NULL; head = head->next) {
        size++;
    }
    
    //printf("List size is %d elements\n", size);
    
    int seq_len;
    long *gaps = Generate_2p3q_Seq(size, &seq_len);

    if (gaps != NULL) {
        //printf("Using shellsort with %d gap values\n", seq_len);
        
        // Process each gap in reverse order
        for (int i = seq_len - 1; i >= 0; i--) {
            int gap = gaps[i];
            //printf("Processing with gap = %d\n", gap);
            
            // Setup pointers
            Node sen;
            sen.next = list;
            
            Node *cur, *far, *preCur, *preFar;
            cur = far = list;
            preCur = &sen;
            preFar = NULL;

            // Position far pointer gap positions ahead
            int j = 0;
            while(j < gap) {
                if (j == gap - 1) 
                    preFar = far;
                far = far->next;
                j++;
            }

            // Process pairs of elements with gap between them
            while (far != NULL) {
                (*n_comp)++;  // Count comparison
                
                // Check if swap needed
                int need_swap = (cur != NULL) && (cur->value > far->value);
                
                if (need_swap) {
                    //printf("Swapping values %ld and %ld\n", cur->value, far->value);
                    
                    Node *tmp = NULL;
                    int is_head_swap = (cur == list);
                    
                    // Handle different swap cases
                    if (gap == 1) {
                        // Direct adjacent swap
                        cur->next = far->next;
                        far->next = cur;
                        preCur->next = far;
                        preFar = far;
                        
                        if (is_head_swap) {
                            sen.next = far;
                        }
                    } 
                    else {
                        // Non-adjacent swap
                        tmp = cur->next;
                        cur->next = far->next;
                        far->next = tmp;
                        
                        preCur->next = far;
                        preFar->next = cur;
                        
                        if (is_head_swap) {
                            sen.next = far;
                        }
                    }
                }
                
                // Advance pointers
                preCur = preCur->next;
                cur = preCur->next;
                
                if (gap == 1) {
                    preFar = cur;
                } else {
                    preFar = preFar->next;
                }
                
                far = preFar->next;
                list = sen.next;  // Update head reference
            }
        }
    }
    else {
        //printf("No sequence generated, falling back to bubble sort\n");
        
        // Bubble sort implementation
        for (int pass = 0; pass < size; pass++) {
            //printf("Bubble sort pass %d of %d\n", pass+1, size);
            
            Node sen;
            sen.next = list;
            
            Node *cur = list;
            Node *next = list->next;
            Node *preCur = &sen;
            Node *preNext = next;

            while (next != NULL) {
                (*n_comp)++;
                
                if (cur->value > next->value) {
                    //printf("Swapping values %ld and %ld\n", cur->value, next->value);
                    
                    // Perform swap
                    int is_head = (cur == list);
                    cur->next = next->next;
                    next->next = cur;
                    preCur->next = next;
                    preNext = next;
                    
                    if (is_head) {
                        sen.next = next;
                    }
                }

                // Move to next pair
                preCur = preCur->next;
                cur = preCur->next;
                preNext = cur;
                next = preNext->next;
                
                // Update head reference
                list = sen.next;
            }
        }
    }
    
    //printf("Sort completed with %ld comparisons\n", *n_comp);
    if (gaps != NULL) {
        free(gaps);
    }
    return list;
}



