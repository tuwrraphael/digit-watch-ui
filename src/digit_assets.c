#include "packed_image.h"
#include <stdint.h>

const uint32_t data[] = {0x86678300,
	0x12011025,
	0x2112112f,
	0x78586102,
	0x13fe3006,
	0x701f0170,
	0x1d701701,
	0x1f1d71d7,
	0x3fe70,
	0x9f30511c,
	0x8107154,
	0x41c7c204,
	0x145051fc,
	0xd14,
	0x1931b9fe,
	0xf71f71f7,
	0xf1971f71,
	0x1fe31b,
	0x8000000,
	0x9f0f386,
	0xce6bc6fc,
	0x338678,
	0x1};

const packed_image_t icon_alarm = {
    .startByte = 0,
    .startBit = 0,
    .width = 12,
    .height = 12,
    .data = data};

const packed_image_t icon_event = {
    .startByte = 4,
    .startBit = 16,
    .width = 12,
    .height = 12,
    .data = data};

const packed_image_t icon_leave1 = {
    .startByte = 9,
    .startBit = 0,
    .width = 7,
    .height = 7,
    .data = data};

const packed_image_t icon_leave2 = {
    .startByte = 10,
    .startBit = 17,
    .width = 7,
    .height = 7,
    .data = data};

const packed_image_t icon_target = {
    .startByte = 12,
    .startBit = 2,
    .width = 7,
    .height = 7,
    .data = data};

const packed_image_t icon_transit = {
    .startByte = 13,
    .startBit = 19,
    .width = 12,
    .height = 12,
    .data = data};

const packed_image_t icon_walk = {
    .startByte = 18,
    .startBit = 3,
    .width = 12,
    .height = 12,
    .data = data};