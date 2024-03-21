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

size_t translate(size_t va) {
    size_t offset = va & ((1 << POBITS) - 1);
    size_t index = va >> POBITS;
    size_t pos = index;

    size_t count = LEVELS;
    size_t curPage = ptbr;
    if (curPage == 0) {
        return MAX;
    }

    while (count > 0) {
        count--;
        pos = index >> (count * vpnbits);
        pos = pos & ((1 << vpnbits) - 1);

        // Shift it so it only contains index bits
        size_t ppn = *((size_t *)curPage + pos) >> POBITS;
        // Shift it back so that offset section is added
        size_t page_address = ppn << POBITS;
        size_t pte = ((size_t *)curPage)[pos];
        if (!(pte & 1)) {
            return MAX;
        }
        if ((page_address != 0)) {
            curPage = page_address;
        } else {
            return MAX;
        }
    }
    printf("translate output is %zx: \n", curPage | offset);
    return curPage | offset;
}

int countPages = 0;

size_t *insertPageWhenInvalid(size_t ptr) {
    size_t *pointer;
    int size = pow(2, POBITS);
    posix_memalign((void **)&pointer, size, size);
    countPages++;
    memset((void *)pointer, 0, size);
    return pointer;
}

void page_allocate(size_t va) {
    // Step 1: Remove offset bits
    size_t index = va >> POBITS;

    size_t index_Holder = index;
    size_t count = LEVELS;

    if (ptbr == 0) {
        ptbr = (size_t)insertPageWhenInvalid(ptbr);
    }
    size_t ptbr_Holder = ptbr;

    while (count > 0) {
        count--;
        // Step 2: Remove any higher levels
        index_Holder = index >> (count * vpnbits);

        // Step 3: Remove unused bits with a mask
        index_Holder = index_Holder & ((1 << vpnbits) - 1);
        // Step 4: Move to index of ptbr and shift through by POBITS to the address

        size_t ppn = *((size_t *)ptbr_Holder + index_Holder) >> POBITS;
        // Step 5: Shift left by POBITS to create space for it
        size_t page_address = ppn << POBITS;
        // Step 6: Move to
        size_t pte = ((size_t *)ptbr_Holder)[index_Holder];
        if (!(pte & 1)) {
            pte = (size_t)insertPageWhenInvalid(pte);
            // Make the valid bit to 1
            ((size_t *)ptbr_Holder)[index_Holder] = pte | 1;
            ptbr_Holder = pte;
        }
        if ((page_address != 0)) {
            ptbr_Holder = page_address;
        }
    }

    // size_t pa = ptbr_Holder | offset;
}