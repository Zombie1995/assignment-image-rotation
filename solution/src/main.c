#include "bmp.h"
#include "image.h"
#include "image_rotation.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

int main( int argc, char** argv ) {
    if (argc != 3) {
        printf("Wrong number of parameters\n");
    }
    else {
        printf("Processing\n");

        FILE *in = fopen(argv[1], "rb");
        FILE *out = fopen(argv[2], "wb");
        
        struct image img = {0};
        from_bmp(in, &img);
        struct image rotated_img = rotate(img);
        to_bmp(out, &rotated_img);

        free(img.data);
        free(rotated_img.data);
        fclose(in);
        fclose(out);

        printf("Done\n");
    }

    return 0;
}
