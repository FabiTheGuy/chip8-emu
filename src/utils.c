#include "utils.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


byte utils_random_byte()
{
    static bool is_seeded = false;

    if (!is_seeded)
    {
        srand(time(NULL));
        is_seeded = true;
    }

    return (byte) rand(); 
}
