
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
    size_t offset = va & ((1 << POBITS) - 1);
    size_t index = va >> POBITS;
    size_t pos = index;

    size_t count = LEVELS;
    size_t curPage = ptbr;
    if (curPage == 0)
    {
        return MAX;
    }

    while (count > 0)
    {
        count--;
        pos = index >> (count * vpnbits);
        pos = pos & ((1 << vpnbits) - 1);

        // shift it so it only contains index bits
        size_t ppn = *((size_t *)curPage + pos) >> POBITS;
        // shift it back so that offset section is added
        size_t page_address = ppn << POBITS;
        size_t pte = ((size_t *)curPage)[pos];
        if (!(pte & 1))
        {
            return MAX;
        }
        if ((page_address != 0))
        {
            curPage = page_address;
        }

        else
            return MAX;
    }
    printf("translate output is %zx: \n", curPage | offset);
    return curPage | offset;
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

    // step 1: remove offset bits
    size_t index = va >> POBITS;

    size_t index_Holder = index;
    size_t count = LEVELS;

    if (ptbr == 0)
    {
        ptbr = (size_t)insertPageWhenInvalid(ptbr);
    }
    size_t ptbr_Holder = ptbr;

    while (count > 0)
    {
        count--;
        // step 2: remove any higher levels
        index_Holder = index >> (count * vpnbits);

        // step 3: remove unused bits with a mask
        index_Holder = index_Holder & ((1 << vpnbits) - 1);
        // step 4: move to index of ptbr and shift through by POBITS to the address

        size_t ppn = *((size_t *)ptbr_Holder + index_Holder) >> POBITS;
        // step 5: shift left by POBITS to create space for it
        size_t page_address = ppn << POBITS;
        // step 6: move to
        size_t pte = ((size_t *)ptbr_Holder)[index_Holder];
        if (!(pte & 1))
        {

            pte = (size_t)insertPageWhenInvalid(pte);
            // make the valid bit to 1
            ((size_t *)ptbr_Holder)[index_Holder] = pte | 1;
            ptbr_Holder = pte;
        }

        if ((page_address != 0))
        {
            ptbr_Holder = page_address;
        }
    }

    //size_t PA = ptbr_Holder | offset;
}
// int main()
// {
//     // 0 pages have been allocated
//     assert(ptbr == 0);

//     page_allocate(0x456789abcdef);

//     // 5 pages have been allocated: 4 page tables and 1 data
//     assert(ptbr != 0);

//     page_allocate(0x456789abcd00);
//     // no new pages allocated (still 5)

//     int *p1 = (int *)translate(0x456789abcd00);
//     *p1 = 0xaabbccdd;
//     short *p2 = (short *)translate(0x456789abcd02);
//     printf("%04hx\n", *p2); // prints "aabb\n"

//     assert(translate(0x456789ab0000) == 0xFFFFFFFFFFFFFFFF);

//     page_allocate(0x456789ab0000);
//     // 1 new page allocated (now 6; 4 page table, 2 data)

//     assert(translate(0x456789ab0000) != 0xFFFFFFFFFFFFFFFF);

//     page_allocate(0x456780000000);
//     // 2 new pages allocated (now 8; 5 page table, 3 data)
// }