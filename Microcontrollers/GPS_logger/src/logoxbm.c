#include <Arduino.h>

/* empty battery icon 25x18 */
uint8_t empty_battery_icon_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xf8, 0xff, 0x0f, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x10, 0x00,
   0xc8, 0x00, 0x30, 0x00, 0xc8, 0x00, 0x30, 0x00, 0xc8, 0x00, 0x30, 0x00,
   0xc8, 0x00, 0x30, 0x00, 0xc8, 0x00, 0x30, 0x00, 0xc8, 0x00, 0x30, 0x00,
   0x08, 0x00, 0x10, 0x00, 0x08, 0x00, 0x08, 0x00, 0xf8, 0xff, 0x0f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* half battery icon 25x18 */
uint8_t half_battery_icon_bits[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xf8, 0xff, 0x0f, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x10, 0x00,
0xc8, 0x1c, 0x30, 0x00, 0xc8, 0x1c, 0x30, 0x00, 0xc8, 0x1c, 0x30, 0x00,
0xc8, 0x1c, 0x30, 0x00, 0xc8, 0x1c, 0x30, 0x00, 0xc8, 0x1c, 0x30, 0x00,
0x08, 0x00, 0x10, 0x00, 0x08, 0x00, 0x08, 0x00, 0xf8, 0xff, 0x0f, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* full battery icon 25x18 */
uint8_t full_battery_icon_bits[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xf8, 0xff, 0x0f, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x10, 0x00,
0xc8, 0x9c, 0x33, 0x00, 0xc8, 0x9c, 0x33, 0x00, 0xc8, 0x9c, 0x33, 0x00,
0xc8, 0x9c, 0x33, 0x00, 0xc8, 0x9c, 0x33, 0x00, 0xc8, 0x9c, 0x33, 0x00,
0x08, 0x00, 0x10, 0x00, 0x08, 0x00, 0x08, 0x00, 0xf8, 0xff, 0x0f, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* gps icon 3 23x23*/
uint8_t sat_icon_small_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x48, 0x00, 0x00,
   0x84, 0x00, 0x00, 0x06, 0x01, 0x00, 0x04, 0x03, 0x00, 0x18, 0x13, 0x00,
   0xf0, 0x3b, 0x00, 0xc0, 0x3d, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x5e, 0x00,
   0x00, 0xee, 0x07, 0x40, 0x60, 0x0c, 0xd0, 0x60, 0x18, 0x90, 0x41, 0x30,
   0x24, 0xc3, 0x10, 0xcc, 0x00, 0x09, 0x98, 0x01, 0x0e, 0x70, 0x00, 0x04,
   0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* fox2 icon large 125x125 */
uint8_t fox_icon_large_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0,
   0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x03, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf8, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf7,
   0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x7d, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0xe7, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc7, 0x1f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x7f, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x87,
   0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0xfe, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0x0f, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0xf0, 0x07, 0x00, 0x00,
   0x00, 0x00, 0xfc, 0x03, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0xc0, 0xff, 0xff, 0xff,
   0xff, 0xff, 0x7f, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0xfe, 0xff, 0xff,
   0xff, 0xff, 0x0f, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0x80, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x20, 0x00, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x07, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00,
   0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07, 0xe0, 0x0f, 0x00, 0x00,
   0x00, 0x00, 0xfe, 0x01, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x07,
   0xe0, 0x3f, 0x00, 0x00, 0x00, 0x80, 0xff, 0x00, 0x7c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x0f, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0xc0, 0x7f, 0x00,
   0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0xff, 0x01, 0x00,
   0x00, 0xe0, 0x3f, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f,
   0x00, 0xfe, 0x03, 0x00, 0x00, 0xf8, 0x0f, 0x00, 0x3f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x3f, 0x00, 0xfc, 0x07, 0x00, 0x00, 0xfc, 0x07, 0x00,
   0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0xf0, 0x0f, 0x00,
   0x00, 0xfe, 0x01, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0xfc, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xfe, 0x00, 0x80, 0x07, 0x00, 0x00, 0x78, 0x00, 0xe0,
   0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x03, 0x00,
   0x00, 0x18, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
   0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0x00, 0x00,
   0x00, 0x00, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0x00, 0xfe, 0x0f, 0x00, 0x7f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f, 0x00, 0xff,
   0x1f, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x7f, 0x00, 0xff, 0x1f, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0xff, 0x1f, 0xe0, 0x0f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0xfe,
   0x0f, 0xf0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xf8, 0x03, 0xf0, 0x01, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x07, 0xf0, 0x01, 0xfc, 0x01, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0xf0,
   0x01, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0x1f, 0xf0, 0x01, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0xf0, 0x01, 0x7f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0,
   0x81, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x3f, 0xf0, 0x81, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xf0, 0xc1, 0x0f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0xf0,
   0xc1, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xfc, 0xf0, 0xe1, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xf1, 0xf1, 0x03, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff,
   0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff,
   0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/* sat icon third wavefront 60x60 */
uint8_t sat_icon_three_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00 };

/* sat icon second wavefront 60x60 */
uint8_t sat_icon_two_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00 };

/* sat icon first wavefront 60x60 */
uint8_t sat_icon_one_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00 };

/* sat icon with no wavefronts 60x60 */
uint8_t sat_icon_zero_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x33, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xc0, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xe0, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00,
   0x07, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00,
   0x1c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x00, 0x00,
   0x00, 0x00, 0x60, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x18,
   0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x18, 0x04, 0x00, 0x00, 0x00, 0x00,
   0x06, 0x3c, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x1e, 0x3f, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0x0f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x87, 0x7f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xe0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x87, 0x1f,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0xe3, 0xf9, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf1, 0x80, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x60, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x0e,
   0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0xc0,
   0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0x01, 0x70, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x38, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x07, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
   0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x0e, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x30, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00 };

/* sat icon full wavefront 60x60 */
uint8_t sat_icon_full_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x33, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xc0, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xe0, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00,
   0x07, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00,
   0x1c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x0c, 0x00, 0x00,
   0x00, 0x00, 0x60, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x18,
   0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x18, 0x04, 0x00, 0x00, 0x00, 0x00,
   0x06, 0x3c, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x1e, 0x3f, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0x0f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x87, 0x7f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xe0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x87, 0x1f,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0xc3, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xe0,
   0xe3, 0xf9, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf1, 0x80, 0x01, 0x00, 0x00,
   0x70, 0x00, 0x60, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x60, 0x00, 0x0e,
   0x00, 0x00, 0xc3, 0x01, 0x60, 0x00, 0x18, 0x00, 0x00, 0x87, 0x03, 0xc0,
   0x00, 0x30, 0x00, 0x10, 0x06, 0x07, 0xc0, 0x00, 0x60, 0x00, 0x38, 0x0c,
   0x1e, 0xc0, 0x01, 0x70, 0x00, 0x70, 0x1c, 0x3c, 0x80, 0x03, 0x38, 0x00,
   0x70, 0x38, 0x10, 0x00, 0x07, 0x1c, 0x00, 0xe0, 0xf0, 0x00, 0x00, 0x0c,
   0x1c, 0x00, 0xc0, 0xc1, 0x01, 0x00, 0x18, 0x0e, 0x00, 0x80, 0x83, 0x07,
   0x00, 0x30, 0x07, 0x00, 0x00, 0x07, 0x0e, 0x00, 0xe0, 0x03, 0x00, 0x00,
   0x1e, 0x04, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00 };


/* large circle icon 80x80 */
uint8_t circle2_large_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8,
   0xff, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0x03,
   0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x00,
   0x00, 0x00, 0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00,
   0xfe, 0x7f, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0x0f,
   0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x80, 0xff, 0x03, 0x00, 0x00,
   0xc0, 0xff, 0x01, 0x00, 0x00, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
   0x07, 0x00, 0x00, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f, 0x00,
   0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0xfc,
   0x07, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x3f, 0x00, 0x00, 0xfc, 0x03, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x00,
   0x00, 0x80, 0x7f, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0x00, 0x80, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x01,
   0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0xc0, 0x1f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0xe0, 0x1f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0x07, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0x07, 0xf0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xe0, 0x0f, 0xf0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f,
   0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0xf8, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0xf8, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x80, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f,
   0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x7e, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e,
   0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x3f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0xfc, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc,
   0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x7f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x7e, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x7e, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e,
   0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xfe, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x3f, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x3f, 0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f,
   0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0xf8, 0x03,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0xf0, 0x07, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xe0, 0x0f, 0xf0, 0x07, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0x0f, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xf0, 0x07, 0xe0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x07,
   0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0x80, 0x3f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x01, 0x80, 0x7f, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xfe, 0x01, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xff, 0x00, 0x00, 0xfe, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80,
   0x7f, 0x00, 0x00, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00,
   0x00, 0xfc, 0x07, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x3f, 0x00, 0x00, 0xf8,
   0x1f, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0xf0, 0x3f, 0x00,
   0x00, 0x00, 0x00, 0xfc, 0x0f, 0x00, 0x00, 0xe0, 0xff, 0x00, 0x00, 0x00,
   0x00, 0xff, 0x07, 0x00, 0x00, 0x80, 0xff, 0x03, 0x00, 0x00, 0xc0, 0xff,
   0x01, 0x00, 0x00, 0x00, 0xff, 0x0f, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00,
   0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x00, 0x00, 0x00,
   0xf8, 0xff, 0x1f, 0xf8, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff,
   0xff, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff,
   0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xf0, 0x0f, 0x00, 0x00, 0x00, 0x00 };


/* circle icon 40x40 */
uint8_t circle2_bits[] = {
   0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0xe0, 0xff, 0x07, 0x00, 0x00, 0xf8,
   0xff, 0x1f, 0x00, 0x00, 0xfe, 0x00, 0x7f, 0x00, 0x00, 0x1f, 0x00, 0xf8,
   0x00, 0x80, 0x07, 0x00, 0xe0, 0x01, 0xc0, 0x03, 0x00, 0xc0, 0x03, 0xe0,
   0x01, 0x00, 0x80, 0x07, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0x78, 0x00, 0x00,
   0x00, 0x1e, 0x38, 0x00, 0x00, 0x00, 0x1c, 0x1c, 0x00, 0x00, 0x00, 0x38,
   0x1c, 0x00, 0x00, 0x00, 0x38, 0x0e, 0x00, 0x00, 0x00, 0x70, 0x0e, 0x00,
   0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x00, 0x70, 0x06, 0x00, 0x00, 0x00,
   0x60, 0x07, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0xe0, 0x07,
   0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00,
   0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0xe0, 0x06, 0x00, 0x00, 0x00, 0x60,
   0x0e, 0x00, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x00, 0x70, 0x0e, 0x00,
   0x00, 0x00, 0x70, 0x1c, 0x00, 0x00, 0x00, 0x38, 0x1c, 0x00, 0x00, 0x00,
   0x38, 0x38, 0x00, 0x00, 0x00, 0x1c, 0x78, 0x00, 0x00, 0x00, 0x1e, 0xf0,
   0x00, 0x00, 0x00, 0x0f, 0xe0, 0x01, 0x00, 0x80, 0x07, 0xc0, 0x03, 0x00,
   0xc0, 0x03, 0x80, 0x07, 0x00, 0xe0, 0x01, 0x00, 0x1f, 0x00, 0xf8, 0x00,
   0x00, 0xfe, 0x00, 0x7f, 0x00, 0x00, 0xf8, 0xff, 0x1f, 0x00, 0x00, 0xe0,
   0xff, 0x07, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00 };


/* small circle icon 30x30 */
uint8_t circle2small_bits[] = {
   0x00, 0xf0, 0x03, 0x00, 0x00, 0xfe, 0x1f, 0x00, 0x80, 0x0f, 0x7c, 0x00,
   0xc0, 0x03, 0xf0, 0x00, 0xe0, 0x00, 0xc0, 0x01, 0x70, 0x00, 0x80, 0x03,
   0x38, 0x00, 0x00, 0x07, 0x1c, 0x00, 0x00, 0x0e, 0x0c, 0x00, 0x00, 0x0c,
   0x0e, 0x00, 0x00, 0x1c, 0x06, 0x00, 0x00, 0x18, 0x06, 0x00, 0x00, 0x18,
   0x03, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00, 0x30,
   0x03, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00, 0x30, 0x03, 0x00, 0x00, 0x30,
   0x06, 0x00, 0x00, 0x18, 0x06, 0x00, 0x00, 0x18, 0x0e, 0x00, 0x00, 0x1c,
   0x0c, 0x00, 0x00, 0x0c, 0x1c, 0x00, 0x00, 0x0e, 0x38, 0x00, 0x00, 0x07,
   0x70, 0x00, 0x80, 0x03, 0xe0, 0x00, 0xc0, 0x01, 0xc0, 0x03, 0xf0, 0x00,
   0x80, 0x0f, 0x7c, 0x00, 0x00, 0xfe, 0x1f, 0x00, 0x00, 0xf0, 0x03, 0x00 };

/* altitude icon 59x40 */
uint8_t altitude2_icon_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x3b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x80, 0x33, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x78, 0xc0, 0x71, 0xc0, 0x01, 0x00, 0x00, 0x00, 0xf8, 0xc0,
   0xe0, 0xe0, 0x03, 0x00, 0x00, 0x00, 0xdc, 0xe1, 0xe0, 0xf0, 0x07, 0x00,
   0x00, 0x00, 0x8e, 0x63, 0x40, 0xf8, 0x0f, 0x00, 0x00, 0x00, 0x06, 0x73,
   0x00, 0xfc, 0x1f, 0x00, 0x00, 0x00, 0x07, 0x37, 0x00, 0xde, 0x3d, 0x00,
   0x00, 0x80, 0x03, 0x3e, 0x00, 0xcf, 0x79, 0x00, 0x00, 0x80, 0x01, 0x1c,
   0x00, 0xc7, 0x71, 0x00, 0x00, 0xc0, 0x01, 0x1c, 0x00, 0xc3, 0x61, 0x00,
   0x00, 0xc0, 0x00, 0x08, 0x00, 0xc0, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00,
   0x00, 0xc0, 0x01, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00,
   0x00, 0x30, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x38, 0x00, 0x00,
   0x00, 0xc0, 0x01, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00,
   0x00, 0x1c, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x0c, 0x00, 0x00,
   0x00, 0xc0, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00,
   0x00, 0x07, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x00, 0x03, 0x00, 0x00,
   0x00, 0xc0, 0x01, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00,
   0x80, 0x01, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0xc0, 0x00, 0x00, 0x00,
   0x00, 0xc0, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00,
   0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x70, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x70, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00,
   0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x1c, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe0, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x01,
   0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x03, 0x06, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07 };


/* temperature icon esterno */
uint8_t temperature_icon_esterno_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0xcc, 0x00, 0x84, 0x00,
   0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00,
   0x84, 0x00, 0x84, 0x00, 0x86, 0x01, 0x03, 0x03, 0x01, 0x02, 0x01, 0x02,
   0x01, 0x02, 0x01, 0x02, 0x03, 0x03, 0xfe, 0x01, 0x78, 0x00 };

/* temperature icon interno */
uint8_t temperature_icon_interno_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00,
   0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x30, 0x00, 0x78, 0x00, 0x78, 0x00,
   0x78, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

