#include "packed_image.h"
#include <stdint.h>

const uint32_t data[] = {0x9f30511c,
	0x8107154,
	0x41c7c204,
	0x145051fc,
	0xd14,
	0x1931b9fe,
	0xf71f71f7,
	0xf1971f71,
	0x1fe31b,
	0x0};

const packed_image_t icon_leave1 = {
    .startByte = 0,
    .startBit = 0,
    .width = 7,
    .height = 7,
    .data = &data};

const packed_image_t icon_leave2 = {
    .startByte = 1,
    .startBit = 17,
    .width = 7,
    .height = 7,
    .data = &data};

const packed_image_t icon_target = {
    .startByte = 3,
    .startBit = 2,
    .width = 7,
    .height = 7,
    .data = &data};

const packed_image_t icon_transit = {
    .startByte = 4,
    .startBit = 19,
    .width = 12,
    .height = 12,
    .data = &data};