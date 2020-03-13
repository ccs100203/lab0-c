#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"
#include "strnatcmp.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    // if malloc return NULL
    if (q == NULL)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    // NULL queue
    if (q == NULL)
        return;

    for (list_ele_t *tmp = q->head; q->head; tmp = q->head) {
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    // NULL queue
    if (q == NULL)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    // if malloc fail
    if (newh == NULL)
        return false;
    newh->value = malloc((strlen(s) + 1) * sizeof(char));
    // if malloc fail
    if (newh->value == NULL) {
        free(newh);
        return false;
    }

    memset(newh->value, 0, (strlen(s) + 1) * sizeof(char));
    strncpy(newh->value, s, strlen(s));
    newh->next = q->head;
    q->head = newh;
    q->size++;
    if (q->size == 1)
        q->tail = newh;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    // NULL queue
    if (q == NULL)
        return false;

    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));
    // if malloc fail
    if (!newt)
        return false;
    newt->value = malloc((strlen(s) + 1) * sizeof(char));
    // if malloc fail
    if (newt->value == NULL) {
        free(newt);
        return false;
    }

    memset(newt->value, 0, (strlen(s) + 1) * sizeof(char));
    strncpy(newt->value, s, strlen(s));
    newt->next = NULL;
    // if queue is empty
    if (q->size == 0) {
        q->head = newt;
        q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    // NULL or empty queue
    if (q == NULL || q->head == NULL)
        return false;

    if (sp != NULL && q->head->value) {
        memset(sp, 0, bufsize);
        strncpy(sp, q->head->value, bufsize - 1);
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return (q == NULL) ? 0 : q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    // NULL queue or element < 2
    if (q == NULL || q->size < 2)
        return;

    list_ele_t *prev, *next, *curr;

    prev = NULL;
    next = q->head->next;
    curr = q->head;

    while (curr != NULL) {
        curr->next = prev;
        prev = curr;
        curr = next;
        if (next != NULL)
            next = next->next;
    }
    curr = q->head;
    q->head = q->tail;
    q->tail = curr;
}


list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t *head = NULL;
    list_ele_t **tmp = &head;

    // sort and connect two list
    // don't move head, use tmp traverse all
    while (l1 && l2) {
        if (strnatcmp(l1->value, l2->value) < 0) {
            *tmp = l1;
            l1 = l1->next;
        } else {
            *tmp = l2;
            l2 = l2->next;
        }
        tmp = &((*tmp)->next);
    }

    // connect remaining node
    if (l1)
        *tmp = l1;
    if (l2)
        *tmp = l2;

    return head;
}

list_ele_t *mergeSortList(list_ele_t *head)
{
    // merge sort
    if (!head || !head->next)
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    // split list, find the middle of list
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    // sort each list
    list_ele_t *l1 = mergeSortList(head);
    list_ele_t *l2 = mergeSortList(fast);

    // merge sorted l1 and sorted l2
    return merge(l1, l2);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    // NULL queue or element < 2
    if (q == NULL || q->size < 2)
        return;
    q->head = mergeSortList(q->head);

    // update q->tail pointer
    list_ele_t *tmp = q->head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    q->tail = tmp;
}
