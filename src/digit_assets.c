#include "packed_image.h"
#include <stdint.h>

const uint32_t data[] = {0x17013fe,
	0x1701701f,
	0x71d71d70,
	0xfe701f1d,
	0x511c0003,
	0x71549f30,
	0xc2040810,
	0x51fc41c7,
	0xd141450,
	0xb9fe0000,
	0x71f71931,
	0x1f71f71f,
	0xe31bf197,
	0x1f};

const packed_image_t icon_event = {
    .startByte = 0,
    .startBit = 0,
    .width = 12,
    .height = 12,
    .data = data};

const packed_image_t icon_leave1 = {
    .startByte = 4,
    .startBit = 16,
    .width = 7,
    .height = 7,
    .data = data};

const packed_image_t icon_leave2 = {
    .startByte = 6,
    .startBit = 1,
    .width = 7,
    .height = 7,
    .data = data};

const packed_image_t icon_target = {
    .startByte = 7,
    .startBit = 18,
    .width = 7,
    .height = 7,
    .data = data};

const packed_image_t icon_transit = {
    .startByte = 9,
    .startBit = 3,
    .width = 12,
    .height = 12,
    .data = data};