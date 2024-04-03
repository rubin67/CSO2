
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

#define NUM_SETS 16
#define WAYS 4

typedef struct
{
    size_t tag;
    size_t pa;
    int valid;
    int lru_counter;
} CacheLine;

CacheLine tlb[NUM_SETS][WAYS];

extern size_t ptbr;

size_t LRU_status = 0;

/** invalidate all cache lines in the TLB */

void tlb_clear()
{
    for (int i = 0; i < NUM_SETS; i++)
    {
        for (int j = 0; j < WAYS; j++)
        {
            tlb[i][j].valid = 0;
        }
    }
}

/**
 * return 0 if this virtual address does not have a valid
 * mapping in the TLB. Otherwise, return its LRU status: 1
 * if it is the most-recently used, 2 if the next-to-most,
 * etc.
 */
int tlb_peek(size_t va)
{
    size_t vpn = va >> POBITS;
    size_t set_index = vpn % NUM_SETS;
    for (int i = 0; i < WAYS; i++)
    {
        if (tlb[set_index][i].valid && tlb[set_index][i].tag == vpn)
        {
            return tlb[set_index][i].lru_counter;
        }
    }
    return 0;
}

/**
 * If this virtual address is in the TLB, return its
 * corresponding physical address. If not, use
 * `translate(va)` to find that address, store the result
 * in the TLB, and return it. In either case, make its
 * cache line the most-recently used in its set.
 *
 * As an exception, if translate(va) returns -1, do not
 * update the TLB: just return -1.
 */
size_t tlb_translate(size_t va)
{
    size_t vpn = va >> POBITS;
    size_t set_index = vpn % NUM_SETS;
    size_t offset = va & (POBITS - 1);

    // Check if the translation is already in the TLB
    for (int i = 0; i < WAYS; i++)
    {
        if (tlb[set_index][i].valid && tlb[set_index][i].tag == vpn)
        {
            // Update LRU
            for (int j = 0; j < WAYS; j++)
            {
                if (tlb[set_index][j].valid && tlb[set_index][j].lru_counter < tlb[set_index][i].lru_counter)
                {
                    tlb[set_index][j].lru_counter++;
                }
            }
            tlb[set_index][i].lru_counter = 0;
            return tlb[set_index][i].pa;
        }
    }

    // If translation not found in TLB, perform translation
    size_t pa = translate(va);
    if (pa == -1)
    {
        return -1;
    }
    // Find the LRU entry
    int lru_index = 0;
    for (int i = 0; i < WAYS; i++)
    {
        if (tlb[set_index][i].valid && tlb[set_index][i].lru_counter == WAYS - 1)
        {
            lru_index = i;
            break;
        }
    }

    // Update TLB entry
    tlb[set_index][lru_index].tag = vpn;
    tlb[set_index][lru_index].pa = pa;
    tlb[set_index][lru_index].valid = 1;

    // Update LRU counters
    for (int i = 0; i < WAYS; i++)
    {
        if (tlb[set_index][i].valid)
        {
            tlb[set_index][i].lru_counter++;
        }
    }
    tlb[set_index][lru_index].lru_counter = 0;

    return va;
}

// size_t translate(size_t va)
// {

//     // Remove offset bits
//     size_t index = va >> POBITS;

//     // Store value of offset
//     size_t offset = va & ((1 << POBITS) - 1);

//     // Stores index
//     size_t index_Holder = index;

//     // Stores levels
//     size_t levelCount = LEVELS;

//     // Returns -1 is invalid page
//     if (ptbr == 0)
//     {
//         return MAX;
//     }

//     // Store page table register
//     size_t ptbr_Holder = ptbr;

//     // Runs through all LEVELS, until it reaches the bottom level
//     while (levelCount > 0)
//     {
//         levelCount--;

//         // Remove any higher levels
//         index_Holder = index >> (levelCount * vpnbits);

//         // Remove unused bits with a mask
//         index_Holder = index_Holder & ((1 << vpnbits) - 1);

//         // Move to index of ptbr and shift through by POBITS to the address
//         size_t ppn = *((size_t *)ptbr_Holder + index_Holder) >> POBITS;

//         // Shift left by POBITS to create space
//         size_t page_address = ppn << POBITS;

//         // Set page table entry equal to the new location
//         size_t pte = ((size_t *)ptbr_Holder)[index_Holder];
//         if (!(pte & 1))
//         {
//             return MAX;
//         }
//         if ((page_address != 0))
//         {
//             ptbr_Holder = page_address;
//         }
//         else
//         {
//             return MAX;
//         }
//     }
//     printf("translate output is %zx: \n", ptbr_Holder | offset);
//     return ptbr_Holder | offset;
// }
