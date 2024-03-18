#include "mlpt.h"
#include "config.h"
#include <assert.h>
#include <stdalign.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#define MAX (size_t)(-1)

alignas(4096)
  static size_t testing_page_table[512];  

size_t ptbr = 0;

size_t translate(size_t va){

    if(ptbr == 0){
        return MAX;
    }


    size_t offset = va & 0xfff;
    size_t index = va >> POBITS;
    size_t PPN = *((size_t *)ptbr + index) >> POBITS;
    size_t PPNshifter = PPN << POBITS;
    size_t PA = PPNshifter | offset;
    //make sure page table entry is not null

    // if the pte doesnt exist, then return max
    size_t PTE = ((size_t*)ptbr)[index];
    if (!(PTE & 1)){
        return MAX;
    }

    if((PA != 0)){
        return PA;
    }
    else
    return MAX;



}

void page_allocate(size_t va){

}









// static void set_testing_ptbr(void) {
//       ptbr = (size_t) &testing_page_table[0];

//       }


//   int main(int argc, char *argv[]) {
//       alignas(4096)
//      static size_t page_of_data[512];

//       set_testing_ptbr();

//       alignas(4096)
//       static char data_for_page_3[4096];

//       size_t address_of_data_for_page_3_as_integer = (size_t) &data_for_page_3[0];
//       size_t physical_page_number_of_data_for_page_3 = address_of_data_for_page_3_as_integer >> 12;
//       size_t page_table_entry_for_page_3 = (
//           (physical_page_number_of_data_for_page_3 << 12)
//           |
//           1
//           );


//       testing_page_table[3] = page_table_entry_for_page_3;

//       size_t data = (size_t) &data_for_page_3[0x44];


//       printf("%zx\n", translate(0x3045));
//       printf("%zx\n", data);

//       assert(translate(0x3044) == data);


//       return 0;

//       }
