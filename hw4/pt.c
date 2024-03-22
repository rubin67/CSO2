#include "mlpt.h"
#include "config.h"
#include <assert.h>
#include <stdalign.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MAX (size_t)(-1)
#define vpnbits (POBITS - 3)

size_t ptbr = 0;

size_t translate(size_t va)
{
    
    // Remove offset bits
    size_t index = va >> POBITS;

    // Store value of offset
    size_t offset = va & ((1 << POBITS) - 1);
    
    // Stores index
    size_t index_Holder = index;

    // Stores levels
    size_t levelCount = LEVELS;

    // Returns -1 is invalid page
    if (ptbr == 0)
    {
        return MAX;
    }

    // Store page table register
    size_t ptbr_Holder = ptbr;

    // Runs through all LEVELS, until it reaches the bottom level
    while (levelCount > 0)
    {
        levelCount--;

        // Remove any higher levels
        index_Holder = index >> (levelCount * vpnbits);

        // Remove unused bits with a mask
        index_Holder = index_Holder & ((1 << vpnbits) - 1);

        // Move to index of ptbr and shift through by POBITS to the address
        size_t ppn = *((size_t *)ptbr_Holder + index_Holder) >> POBITS;

        // Shift left by POBITS to create space
        size_t page_address = ppn << POBITS;

        // Set page table entry equal to the new location
        size_t pte = ((size_t *)ptbr_Holder)[index_Holder];
        if (!(pte & 1))
        {
            return MAX;
        }
        if ((page_address != 0))
        {
            ptbr_Holder = page_address;
        }
        else
        {
            return MAX;
        }
    }
    printf("translate output is %zx: \n", ptbr_Holder | offset);
    return ptbr_Holder | offset;
}

int countPages = 0;

size_t *insertPageWhenInvalid(size_t ptr)
{
    size_t *pointer;
    int size = pow(2, POBITS);
    posix_memalign((void **)&pointer, size, size);
    countPages++;
    memset((void *)pointer, 0, size);
    return pointer;
}

void page_allocate(size_t va)
{
    // Remove offset bits
    size_t index = va >> POBITS;

    // Stores index
    size_t index_Holder = index;

    // Stores levels
    size_t levelCount = LEVELS;

    // Inserts page if there is not already a page
    if (ptbr == 0)
    {
        ptbr = (size_t)insertPageWhenInvalid(ptbr);
    }

    // Store page table register
    size_t ptbr_Holder = ptbr;

    // Runs through all LEVELS, until it reaches the bottom level
    while (levelCount > 0)
    {
        levelCount--;

        // Remove any higher levels
        index_Holder = index >> (levelCount * vpnbits);

        // Remove unused bits with a mask
        index_Holder = index_Holder & ((1 << vpnbits) - 1);

        // Move to index of ptbr and shift through by POBITS to the address
        size_t ppn = *((size_t *)ptbr_Holder + index_Holder) >> POBITS;

        // Shift left by POBITS to create space
        size_t page_address = ppn << POBITS;

        // Set page table entry equal to the new location
        size_t pte = ((size_t *)ptbr_Holder)[index_Holder];
        if (!(pte & 1))
        {
            pte = (size_t)insertPageWhenInvalid(pte);

            // Set new added page address's valid bit to 1
            ((size_t *)ptbr_Holder)[index_Holder] = pte | 1;

            // Move page table base register to the newly allocated page
            ptbr_Holder = pte;
        }
        // Handles if there is a valid page
        if ((page_address != 0))
        {
            ptbr_Holder = page_address;
        }
    }
}