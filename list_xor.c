/* Answer field in the function sort() here */
#define LL1 NULL
#define LL2 NULL
#define RR1 NULL
#define RR2 NULL

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
    if (!start || !start->addr)
        return start;

    list *left = start, *right = start->addr;
    left->addr = NULL;
    right->addr = XOR(right->addr, left);

    left = sort(left);
    right = sort(right);

    for (list *merge = NULL; left || right;) {
        if (!right || (left && left->data < right->data)) {
            list *next = left->addr;
            if (next)
                next->addr = XOR(left, next->addr);

            if (!merge) {
                start = merge = left;
                merge->addr = NULL;
            } else {
                merge->addr = LL1;
                left->addr = LL2;
                merge = left;
            }
            left = next;
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

int main(int argc, char *argv[]){

    /* TODO: Automatically input testing data! */

    /* Local variable declaration */
    list *head, *ptr, *prev, *temp;
    int i;

    /* Random seed initialize */
    srand(time(NULL));

    ptr = NULL;

    /* Insert 100 nodes to the list */
    for (i = 0 ; i < 100 ; i++) {

        insert_node(&ptr, (99 - i));

        if (i == 0) head = ptr;
    }

    /* Print all nodes in the list */
    prev = NULL;
    ptr = head; // Set address to the head
    i = 0;

    while (ptr != NULL){

        printf("%d -> ", ptr->data);
        if ( (++i % 10) == 0) printf("\n");

        temp = ptr;
        ptr = XOR(prev, ptr->addr);
        prev = temp;

    }

    /* Free allocated nodes */
    delete_list(head);

    return 0;
}
