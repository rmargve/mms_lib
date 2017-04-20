//
// Created by rmarg on 3/4/2017.
//

#ifndef MMS_LIB_MMS_API_H
#define MMS_LIB_MMS_API_H


#include <stdint.h>

#define round_bytes 4

typedef int16_t alloc_data_size;

void * mms_alloc(int16_t);
void mms_free(void *);


#endif //MMS_LIB_MMS_API_H
