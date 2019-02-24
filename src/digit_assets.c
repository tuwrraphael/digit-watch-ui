#include "packed_image.h"
#include <stdint.h>

const uint32_t image[] = {0x373fc000, 0xee3ee326, 0xe3ee3ee3, 0xfc637e32, 0x3};

const packed_image_t icon_public_transport = {
    .startByte = 0,
    .startBit = 0,
    .width = 12,
    .height = 12,
    .data = &image};