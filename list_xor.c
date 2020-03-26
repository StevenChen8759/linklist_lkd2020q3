/* Answer field in the function sort() here */
#define LL1 XOR(merge->addr, left) // LL1 Select (h)
#define LL2 merge // LL2 Select (c)
#define RR1 XOR(merge->addr, right) // RR1 Select (i)
#define RR2 merge // RR2 Select (c)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef struct __list list;
struct __list {
    int data;
    struct __list *addr;
};

#define XOR(a, b) ((list *) ((uintptr_t) (a) ^ (uintptr_t) (b)))

void insert_node(list **l, int d) {
    list *tmp = malloc(sizeof(list));
    tmp->data = d;

    if (!(*l)) {
        tmp->addr = NULL;
    } else {
        (*l)->addr = XOR(tmp, (*l)->addr);
        tmp->addr = *l;
    }
    *l = tmp;
}

void delete_list(list *l) {
    while (l) {
        list *next = l->addr;
        if (next)
            next->addr = XOR(next->addr, l);
        free(l);
        l = next;
    }
}

list *sort(list *start)
{
    /* Return while input is equal or less than 1 element */
    if (!start || !start->addr)
        return start;

    /* Divide -> Cut to two sublist */
    list *left = start, *right = start->addr;
    left->addr = NULL;						// Left sublist with 1 element only
    right->addr = XOR(right->addr, left);   // Right sublist with n-1 elements

    /* Conquer -> Sort each sublist */
    left = sort(left);   // Sort left list
    right = sort(right); // Sort right list

    /* Combine - List Merging */
    // loop: merge start with NULL, repeat until left and right are both NULL
    for (list *merge = NULL; left || right;) {
        // [Right is NULL] OR [left is not NULL and left data is smaller than right data]
        if (!right || (left && left->data < right->data)) {
            list *next = left->addr;  // Assign next as address mask
            // Mask is not NULL, merge the list
            if (next)
                next->addr = XOR(left, next->addr);

            // Merge pointer is initially NULL, link to left list initially
            if (!merge) {
                start = merge = left;
                merge->addr = NULL;
            // Not initially NULL, do some manipulation on link
            } else {
                merge->addr = LL1;  // Calculte bit mask for previous value of left
                left->addr = LL2;   // Store previous node address for addr calculation in the next run
                merge = left;       // Assign pointer of next element to merge
            }
            left = next;
        // [Right is NOT NULL] AND [left is NULL OR left data is bigger than or equal to right data]
        } else {
            list *next = right->addr;
            if (next)
                next->addr = XOR(right, next->addr);

            if (!merge) {
                start = merge = right;
                merge->addr = NULL;
            } else {
                merge->addr = RR1;
                right->addr = RR2;
                merge = right;
            }
            right = next;
        }
    }

    return start;
}

void print_list(list *head) {

    list *ptr, *prev, *temp;
    int cnt = 0;

    if (head == NULL) return;

    ptr = head; prev = NULL;

    /* Print all nodes in the list */
    prev = NULL;
    ptr = head; // Set address to the head
    cnt = 0;

    printf("<Start of list> ");

    while (ptr != NULL){

        printf("%d ", ptr->data);

        temp = ptr;
        ptr = XOR(prev, ptr->addr);
        prev = temp;

        if (ptr != NULL) {

            printf("<-> ");
            if (++cnt % 8  == 0) printf("\n");
        }

    }

    printf("<End of list>\n");

}

int main(int argc, char *argv[]){

    /* TODO: Automatically input testing data! */

    /* Local variable declaration */
    list *head, *ptr, *prev, *temp;
    int i;

    /* Random seed initialize */
    srand(time(NULL));

    /* Make pointr initialize for linking operation */
    ptr = NULL;

    /* Insert 100 nodes to the list and print it */
    for (i = 0 ; i < 100 ; i++) {

        insert_node(&ptr, (99 - i));

        if (i == 0) head = ptr;
    }

    insert_node(&ptr, 5); // head = ptr;
    insert_node(&ptr, 9);
    insert_node(&ptr, 4);
    insert_node(&ptr, 3);
    insert_node(&ptr, 2);

    printf("--------------------Before Sort--------------------\n");

    print_list(head);

    printf("---------------------------------------------------\n");

    ptr = sort(head);

    printf("--------------------After Sort--------------------\n");

    print_list(ptr);

    printf("--------------------------------------------------\n");

    /* Free allocated nodes */
    delete_list(ptr);

    return 0;
}
