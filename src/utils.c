#include <stdlib.h>
#include <time.h>
#include "utils.h"

void seed(uint32_t value)
{
    unsigned int seed_val = (unsigned int)time(NULL);
    if (value) seed_val = (unsigned int) value;
    srand(seed_val); 
       
    for (int i = 0; i < 10; i++) {
        rand();
    } // skipping initialization correlation

}