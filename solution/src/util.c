#include "util.h"
#include <stdint.h>

uint32_t index1D(uint32_t x, uint32_t y, uint32_t width) {
    return x + y * width;
}
