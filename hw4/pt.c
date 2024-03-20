
#include "mlpt.h"
#include "config.h"
#include <assert.h>
#include <stdalign.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#define MAX (size_t)(-1)
#define vpnbits (POBITS - 3)

alignas(4096) static size_t testing_page_table[512];
size_t ptbr = 0;

size_t translate(size_t va)
{

    if (ptbr == 0)
    {
        return MAX;
    }


    // size_t offset = va & ((1 << POBITS) - 1);
    // size_t index = va >> POBITS;

    // size_t pos = index;

    // size_t count = LEVELS-1;

    // size_t curPage = ptbr;
    // for (int i = 0; i < count; i++)
    // {

    //     pos = index >> (count * vpnbits);
    //     count--;
    //     pos = pos & ((1 << vpnbits) - 1);
    //     // shift it so it only contains index bits
    //     size_t PPN = *((size_t *)curPage + pos) >> POBITS;
    //     // shift it back so that offset section is added
    //     size_t PPNshifter = PPN << POBITS;

    //     size_t PTE = ((size_t *)curPage)[pos];
    //     if (!(PTE & 1))
    //     {
    //         return MAX;
    //     }

    //     if ((PPNshifter != 0))
    //     {
    //         curPage = PPNshifter;
    //     }
    //     else
    //         return MAX;
    // }

size_t offset = va & ((1 << POBITS) -1);
    size_t index = va >> POBITS;
    size_t PPN = *((size_t *)ptbr + index) >> POBITS;
    size_t PPNshifter = PPN << POBITS;
    size_t PA = PPNshifter | offset;
    return PA -1;
        //return curPage | offset;
}


void page_allocate(size_t va)
{



    size_t offset = va & (1 << POBITS) - 1;
    size_t index = va >> offset;

    size_t pos = index;

    size_t count = LEVELS-1;

    size_t curPage = ptbr;
    if (curPage == 0)
    {
        void* pointer = NULL;
        posix_memalign(pointer, 1 << POBITS, 1 << POBITS);
        memset(pointer, 0, POBITS);
        curPage = (size_t)pointer | 1;
    }
    for (int i = 0; i < LEVELS; i++)
    {

        pos = index >> (count * vpnbits);
        count--;
        pos = pos & ((1 << vpnbits) - 1);
        // shift it so it only contains index bits
        size_t PPN = *((size_t *)curPage + pos) >> POBITS;
        // shift it back so that offset section is added
        size_t PPNshifter = PPN << POBITS;

        // make sure page table entry is not null

        // if the pte doesnt exist, then return max
        size_t PTE = ((size_t *)curPage)[pos];
        if (!(PTE & 1))
        {
            void* pointer = NULL;
            posix_memalign(pointer, 1 << POBITS, 1 << POBITS);
            memset(pointer, 0, POBITS);
            curPage = (size_t)pointer | 1;
        }

        if ((PPNshifter != 0))
        {
            curPage = PPNshifter;
        }
        else{
            void* pointer = NULL;
            posix_memalign(pointer, 1 << POBITS, 1 << POBITS);
            memset(pointer, 0, POBITS);
            curPage = (size_t)pointer | 1;
        }
    }
    
        size_t PA = curPage | offset;

}



// int main() {
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





static void set_testing_ptbr(void)
{
    ptbr = (size_t)&testing_page_table[0];
}

int main(int argc, char *argv[])
{
    alignas(4096) static size_t page_of_data[512];

    set_testing_ptbr();

    alignas(4096) static char data_for_page_3[4096];

    size_t address_of_data_for_page_3_as_integer = (size_t)&data_for_page_3[0];
    size_t physical_page_number_of_data_for_page_3 = address_of_data_for_page_3_as_integer >> 12;
    size_t page_table_entry_for_page_3 = ((physical_page_number_of_data_for_page_3 << 12) |
                                          1);

    testing_page_table[3] = page_table_entry_for_page_3;

    size_t data = (size_t)&data_for_page_3[0x44];

    printf("%zx\n", translate(0x3045));
    printf("%zx\n", data);

    assert(translate(0x3044) == data);

    return 0;
}