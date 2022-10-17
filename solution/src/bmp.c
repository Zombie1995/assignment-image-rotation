#include "bmp.h"
#include "image.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t get_padding(uint32_t width) {
    return (4 - width * 3 % 4) % 4;
}

static uint32_t get_image_size(
    uint32_t width, 
    uint32_t height, 
    uint32_t padding) {
    return (3 * width + padding) * height;
}

static void pixel_data_to_pixels(
    struct pixel *pixels, 
    uint8_t const *pixel_data, 
    uint32_t width, 
    uint32_t height, 
    uint32_t padding) {
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            uint32_t pixel_index = index1D(j, i, width); 
            pixels[pixel_index].r = pixel_data[3 * pixel_index + 0 + i * padding];
            pixels[pixel_index].g = pixel_data[3 * pixel_index + 1 + i * padding];
            pixels[pixel_index].b = pixel_data[3 * pixel_index + 2 + i * padding];
        }
    }
}

static void pixels_to_pixel_data(
    struct pixel const *pixels, 
    uint8_t *pixel_data, 
    uint32_t width, 
    uint32_t height, 
    uint32_t padding) {
    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < width; j++) {
            uint32_t pixel_index = index1D(j, i, width); 
            pixel_data[3 * pixel_index + 0 + i * padding] = pixels[pixel_index].r;
            pixel_data[3 * pixel_index + 1 + i * padding] = pixels[pixel_index].g;
            pixel_data[3 * pixel_index + 2 + i * padding] = pixels[pixel_index].b;
        }
    }
}

static struct bmp_header new_bmp_header(
    uint32_t biSizeImage, 
    uint32_t width, 
    uint32_t height) {
    struct bmp_header header = {0};
    header.bfType = 0x4D42;
    header.bfileSize = sizeof(struct bmp_header) + biSizeImage;
    header.bfReserved = 0;
    header.bOffBits = sizeof(struct bmp_header);
    header.biSize = 40;
    header.biWidth = width;
    header.biHeight = height;
    header.biPlanes = 1;
    header.biBitCount = 24;
    header.biCompression = 0;
    header.biSizeImage = biSizeImage;
    header.biXPelsPerMeter = 0;
    header.biYPelsPerMeter = 0;
    header.biClrUsed = 0;
    header.biClrImportant = 0;
    return header;
}

enum read_status from_bmp(FILE *in, struct image *img) {
    struct bmp_header header;
    fread(&header, sizeof(struct bmp_header), 1, in);
    
    uint32_t padding = get_padding(header.biWidth);
    uint32_t biSizeImage = get_image_size(
        header.biWidth, 
        header.biHeight,
        padding);
    struct pixel *pixels = malloc(
        sizeof(struct pixel) * 
        header.biWidth * 
        header.biHeight);
    uint8_t *pixel_data = malloc(sizeof(uint8_t) * biSizeImage);
    fseek(in, header.bOffBits, SEEK_SET);
    // fread(pixel_data, header.biSizeImage, 1, in); // Sometimes header.biSizeImage = 0
    fread(pixel_data, biSizeImage, 1, in);

    pixel_data_to_pixels(
        pixels, 
        pixel_data, 
        header.biWidth, 
        header.biHeight, 
        padding);

    img->width = header.biWidth;
    img->height = header.biHeight;
    img->data = pixels;

    free(pixel_data);
    rewind(in);

    return READ_OK;
}

enum write_status to_bmp(FILE *out, struct image const *img) {
    uint32_t padding = get_padding(img->width);
    uint32_t biSizeImage = get_image_size(
        img->width, 
        img->height,
        padding);
    struct bmp_header header = new_bmp_header(
        biSizeImage, 
        img->width, 
        img->height);
    fwrite(&header, sizeof(struct bmp_header), 1, out);

    uint8_t *pixel_data = malloc(sizeof(uint8_t) * biSizeImage);
    pixels_to_pixel_data(
        img->data, 
        pixel_data, 
        img->width, 
        img->height, 
        padding);
    fwrite(pixel_data, biSizeImage, 1, out);

    free(pixel_data);
    rewind(out);

    return WRITE_OK;
}
