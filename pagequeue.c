/** pagequeue.c
 * ===========================================================
 * Name: _______________________, __ ___ 2026
 * Section: CS483 / ____
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer
    PageQueue *pagequeue = malloc(sizeof(PageQueue));
    pagequeue->head = NULL;
    pagequeue->tail = NULL;
    pagequeue->size = 0;
    pagequeue->maxSize = maxSize;
    return pagequeue;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    PqNode *current = pq->tail;
    int depth = 0;
    while(current != NULL){
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
        if (current->pageNum == pageNum){
            if (current == pq->tail){
                return depth;
            }
            if (current == pq->head){
                pq->head = pq->head->next;
                pq->head->prev = NULL;
            }
            else{
                if (current->next != NULL){
                    current->next->prev = current->prev;
                }
                if (current->prev != NULL){
                    current->prev->next = current->next;
                }
            }
            current->prev = pq->tail;
            pq->tail->next = current;
            pq->tail = current;
            pq->tail->next = NULL;
            return depth;
        }
        current = current ->prev;
        depth++;
    }   
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.
    PqNode *node = malloc(sizeof(PqNode));
    node->pageNum = pageNum;
    node->prev = pq->tail;
    if (pq->tail == NULL){
        pq->head = node;
        pq->tail = node;
    }else{
        pq->tail->next = node;
        pq->tail = node;
    }
    pq->size++;
    if (pq->size > pq->maxSize){
        PqNode *temp = pq->head;
        pq->head = pq->head->next;
        pq->head->prev = NULL;
        free(temp);
        pq->size--;
    }
    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.
    PqNode *current = pq->head;
    while (current != NULL){
        PqNode *next = current->next;
        free(current);
        current = next;
    }
    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.
}
