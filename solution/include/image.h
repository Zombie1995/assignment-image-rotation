#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

struct pixel { uint8_t r, g, b; };

struct image {
  uint64_t width, height;
  struct pixel* data;
};

#endif
