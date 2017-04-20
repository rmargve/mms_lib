#include <stdio.h>
#include <stdint.h>
#include "mms_api.h"

int main() {
    int * ptr = mms_alloc(5);
    printf ("%d\n", *ptr);
    int * ptr2 = mms_alloc(5);
    int * ptr3 = mms_alloc(5);
    mms_free(ptr);
    mms_free(ptr2);
    mms_free(ptr3);
    ptr = mms_alloc(19);
    ptr[0] = 3; ptr[1]= 5;
    printf("%d %d\n", ptr[0], ptr[1]);
    return 0;
}