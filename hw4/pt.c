
#include "mlpt.h"
#include "config.h"
#include <assert.h>
#include <stdalign.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define MAX (size_t)(-1)
#define vpnbits (POBITS - 3)

alignas(4096) static size_t testing_page_table[512];
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

    for (int i = 0; count > 0; i++)
    {
        count--;
        pos = index >> (count * vpnbits);
        pos = pos & ((1 << vpnbits) - 1);

        // shift it so it only contains index bits
        size_t PPN = *((size_t *)curPage + pos) >> POBITS;
        // shift it back so that offset section is added
        size_t PPNshifter = PPN << POBITS;
        size_t PTE = ((size_t *)curPage)[pos];
        if (!(PTE & 1))
        {
            return MAX;
        }
        if ((PPNshifter != 0))
        {
            curPage = PPNshifter;
        }

        else
            return MAX;
    }
    printf("translate output is %zx: \n", curPage | offset);
    return curPage | offset;
}

void page_allocate(size_t va)
{

    size_t offset = va & ((1 << POBITS) - 1);
    size_t index = va >> offset;

    size_t pos = index;

    size_t count = LEVELS;

    if (ptbr == 0)
    {
        size_t *pointer;
        ptbr = &pointer;
        posix_memalign(&ptbr, pow(2, POBITS), pow(2, POBITS));
        int val = pow(2, POBITS);
        //memset(ptbr, 0, val);
    }
    size_t curPage = ptbr;

    for (int i = 0; count > 0; i++)
    {
        count--;
        pos = index >> (count * vpnbits);
        pos = pos & ((1 << vpnbits) - 1);

        // shift it so it only contains index bits
        size_t PPN = *((size_t *)curPage + pos) >> POBITS;
        // shift it back so that offset section is added
        size_t PPNshifter = PPN << POBITS;
        size_t PTE = ((size_t *)curPage)[pos];
        if (!(PTE & 1))
        {
            //size_t *pointer;
            posix_memalign(&PTE, pow(2, POBITS), pow(2, POBITS));
            int val = pow(2, POBITS);
            //memset(PTE, 0, val);
            PTE = PTE | 1;
        }
        if ((PPNshifter != 0))
        {
            curPage = PPNshifter;
        }
    }

    size_t PA = curPage | offset;
}

int main() {
    // 0 pages have been allocated
    assert(ptbr == 0);

    page_allocate(0x456789abcdef);
    // 5 pages have been allocated: 4 page tables and 1 data
    assert(ptbr != 0);

    page_allocate(0x456789abcd00);
    // no new pages allocated (still 5)
    
    int *p1 = (int *)translate(0x456789abcd00);
    //*p1 = 0xaabbccdd;
    short *p2 = (short *)translate(0x456789abcd02);
    //printf("%04hx\n", *p2); // prints "aabb\n"

    assert(translate(0x456789ab0000) == 0xFFFFFFFFFFFFFFFF);
    
    page_allocate(0x456789ab0000);
    // 1 new page allocated (now 6; 4 page table, 2 data)

    assert(translate(0x456789ab0000) != 0xFFFFFFFFFFFFFFFF);
    
    page_allocate(0x456780000000);
    // 2 new pages allocated (now 8; 5 page table, 3 data)
}

// static void set_testing_ptbr(void)
// {
//     ptbr = (size_t)&testing_page_table[0];
// }

// int main(int argc, char *argv[])
// {
//     alignas(4096) static size_t page_of_data[512];

//     set_testing_ptbr();

//     alignas(4096) static char data_for_page_3[4096];

//     size_t address_of_data_for_page_3_as_integer = (size_t)&data_for_page_3[0];
//     size_t physical_page_number_of_data_for_page_3 = address_of_data_for_page_3_as_integer >> 12;
//     size_t page_table_entry_for_page_3 = ((physical_page_number_of_data_for_page_3 << 12) |
//                                           1);

//     testing_page_table[3] = page_table_entry_for_page_3;

//     size_t data = (size_t)&data_for_page_3[0x45];

//     printf("%zx\n", translate(0x3045));
//     printf("%zx\n", data);

//     assert(translate(0x3044) == data);

//     return 0;
// }