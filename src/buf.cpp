#include "buf.h"
#include <stdio.h>

void _test_buf() {
    Buf<int> rb(10);
    for (int i = 0; i < 10; ++i) {
        rb.push(i);
    }
    printf("RingBuffer size: %d\n", rb.size());
    printf("RingBuffer is full: %d\n", rb.is_full());
    printf("RingBuffer is empty: %d\n", rb.is_empty());
    printf("RingBuffer front: %d\n", rb.front());
    rb.pop();
    printf("RingBuffer front after pop: %d\n", rb.front());
    printf("RingBuffer size after pop: %d\n", rb.size());
    rb.clear();
    printf("RingBuffer size after reset: %d\n", rb.size());
    printf("RingBuffer is empty after reset: %d\n", rb.is_empty());
    for (int i = 0; i < 10; ++i) {
        rb.push(i);
    }
    printf("RingBuffer size after push: %d\n", rb.size());
    printf("RingBuffer is full after push: %d\n", rb.is_full());
    int arr[10];
    rb.copy_to(arr, rb.size());
    printf("RingBuffer copy to array: ");
    for (int i = 0; i < rb.size(); ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    rb.pop();
    rb.pop();
    rb.pop();
    printf("RingBuffer size after pop: %d\n", rb.size());
    rb.copy_to(arr, rb.size());
    printf("RingBuffer copy to array after pop: ");
    for (int i = 0; i < rb.size(); ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    rb.clear();
    printf("RingBuffer size after reset: %d\n", rb.size());
    printf("RingBuffer is empty after reset: %d\n", rb.is_empty());

}
