#include "image.h"
#include "image_rotation.h"
#include "util.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct image rotate( struct image const source ) {
    uint64_t src_width = source.width;
    uint64_t src_height = source.height;
    struct pixel *src_pixels = source.data;
    struct pixel *pixels = malloc(sizeof(struct pixel) * src_width * src_height);

    for (uint64_t i = 0; i < src_height; i++) {
        for (uint64_t j = 0; j < src_width; j++) {
            pixels[index1D(src_height - 1 - i, j, src_height)] = src_pixels[index1D(j, i, src_width)];
        }
    }
    struct image res = {.width = src_height, .height = src_width, .data = pixels};
    return res;
}
