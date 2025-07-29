#include "coord_queue.h"
#include <stdlib.h>

CoordQueue test;

void init_coord_queue(CoordQueue* q) {
    q->front = NULL;
    q->rear = NULL;
}

void enqueue_coord(CoordQueue* q, float x, float y) {
    CoordNode* new_node = (CoordNode*)malloc(sizeof(CoordNode));
    if (!new_node) return;

    new_node->data.x = x;
    new_node->data.y = y;
    new_node->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = new_node;
    } else {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

int dequeue_coord(CoordQueue* q, Point* out_point) {
    if (q->front == NULL) return 0;

    CoordNode* temp = q->front;
    *out_point = temp->data;

    q->front = temp->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    return 1;
}

void free_coord_queue(CoordQueue* q) {
    Point dummy;
    while (dequeue_coord(q, &dummy));
}

int is_coord_queue_empty(CoordQueue* q) {
    return (q->front == NULL);
}
