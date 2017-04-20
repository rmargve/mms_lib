//
// Created by rmarg on 3/4/2017.
//

#include <stdint.h>
#include "mms_api.h"

static int8_t s_mem[10002] = {-16, -40};
// *(int16_t *)s_mem == -10000;
static int8_t* s_mem_ptr = s_mem;

static void * mms_alloc_helper(int16_t initial_alloc_size, int8_t * mem_ptr){
    int16_t allocation_size;
    allocation_size = *(int16_t *)mem_ptr;
    if (initial_alloc_size + round_bytes >= -allocation_size)
    { // tu tavdapirveli motxovna ramdenime baitit naklebi iyo arsebul tavisupal space-ze, mashin davamrgvalot da damatebit bytebi gavatanot.
        *(int16_t *)mem_ptr = -allocation_size;
    }
    else
    { // tu tavisupali adgili gacilebit metia motxovnil baitebze
        //gamoyofili adgilis bolos chavwert darchenili space-s sigrdzes (chawerili zoma unda iyos minus nishnit.
        *(int16_t *)(mem_ptr + sizeof(allocation_size) + initial_alloc_size) = allocation_size + initial_alloc_size + sizeof(allocation_size);
        *(int16_t *)mem_ptr = initial_alloc_size;
    }
    return (void *)(mem_ptr + sizeof(allocation_size));
}

static alloc_data_size adjacent_merge(int8_t * ptr){
    // shevamowmebt aramxolod or mezobel regions, aramed yvela regions sanam dakavebuli regioni ar shegxvdeba
    alloc_data_size allocation_size, merge_size = 0;
    int8_t * temp = ptr;
    while ((allocation_size = *(alloc_data_size *)temp) < 0)
    {
        merge_size += allocation_size - sizeof(allocation_size); // gaertianebisas vimatebt data-stvis chasawer 2 baits
        temp += -allocation_size + sizeof(allocation_size);
        if (temp - s_mem_ptr >= 10002) break;
    }
    merge_size +=2;
    return merge_size; // vitvaliswinebt im 2 baits, sadac chaiwereba shemdegi regionis size(minus nishnit)
}

void * mms_alloc(int16_t initial_alloc_size){
    if (initial_alloc_size <= 0)
        return 0;
    alloc_data_size allocation_size; // 2 bytes to store information in array about allocated data
    int16_t iterator = 0;
    int8_t * tmp = 0;
    while (iterator < 10002)

    {
        allocation_size = *(alloc_data_size *)(s_mem_ptr + iterator);

        if (allocation_size > 0)
        { // tu masivshi allocation_size baiti ukve dakavebulia, iteratori gadavwiot allocation_size-ti + sizeof(allocation_size)
            iterator += allocation_size + sizeof(allocation_size);
        }
        else if (allocation_size < 0)
        { // tu masivshi shemdegi allocation_size baiti aris tavisupali
            int8_t * next_allocated_ptr = s_mem_ptr + iterator -allocation_size + sizeof(allocation_size);
            if (next_allocated_ptr - s_mem_ptr < 10002)
            { // tu shemdegi alokacia tavdapirveli masivis kideshi ar aris
                if (*(alloc_data_size *) next_allocated_ptr < 0) { // tu shemdegi regionic tavisupalia, gavaertianot
                    allocation_size = *(alloc_data_size *)(s_mem_ptr + iterator) = adjacent_merge(s_mem + iterator);
                }
            }
            if (initial_alloc_size == -allocation_size)
            { // tu tavisupali baitebis zoma zustad emtxveva motxovnil alokacias
                *(alloc_data_size *)(s_mem_ptr + iterator) = -allocation_size; // zomas shevucvalot nishani, anu avghnishnot rom allocation_size baiti dakavebulia.
                return (void *)(s_mem_ptr + iterator + sizeof(allocation_size));
                //davabrunot sawyis pointers + iteratori + zomis shesanaxad gamoyofili baitebi.
            }
            else if (initial_alloc_size > -allocation_size)
            { // tavisupali baitebis zoma naklebia motxovnil alokaciaze
                iterator += -allocation_size + sizeof(allocation_size);
            }
            else if (initial_alloc_size < -allocation_size)
            { // tu tavisupali baitebis zoma metia  motxovnil baitebze
                if (tmp == 0)
                    tmp = s_mem_ptr + iterator;
                else if (*(alloc_data_size *)tmp < allocation_size) // searching for best-fit
                { // *tmp-c da allocation_size-c aris minus nishniani, magalitad if (-50 < -35)
                    tmp = s_mem_ptr + iterator;
                }
                iterator += -allocation_size + sizeof(allocation_size);
            }
        }
    }
    // tu ver moidzebna exact match mashin shevamwomot tmp pointeri
    if (tmp != 0)
        return mms_alloc_helper(initial_alloc_size, tmp);
    else
        return 0;
}

void mms_free(void * ptr){
    int16_t k = *(int16_t *)((int8_t *)ptr - 2);
    *(int16_t *)((int8_t *)ptr - 2) = -*(int16_t *)((int8_t *)ptr - 2);
    k = *(int16_t *)((int8_t *)ptr - 2);
    return;
}
