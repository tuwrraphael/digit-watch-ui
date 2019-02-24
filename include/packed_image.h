#ifndef PACKED_IMAGE
#define PACKED_IMAGE
#include <stdint.h>
typedef struct
{
    uint8_t startByte;
    uint8_t startBit;
    uint8_t width;
    uint8_t height;
    uint32_t *data;
} packed_image_t;
#endif