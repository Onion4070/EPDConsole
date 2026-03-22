#include "FrameBuffer.h"

#include <string.h>

FrameBuffer::FrameBuffer() {
    clear();
}

FrameBuffer::~FrameBuffer() {

}

// 白: 0xFF, 黒: 0x00
void FrameBuffer::fill(uint8_t color) {
    memset(buf, color, sizeof(buf));
}

void FrameBuffer::clear() {
    fill(0xFF);
}

void FrameBuffer::setPixel(int x, int y, bool black) {
    if (x < 0 || x >= EPD_WIDTH || y < 0 || y >= EPD_HEIGHT) return;
    int idx = y * (EPD_WIDTH / 8) + x / 8;
    uint8_t bit = 0x80 >> (x % 8);
    if (black)
        buf[idx] &= ~bit;  // 0=黒
    else
        buf[idx] |= bit;  // 1=白
}

void FrameBuffer::drawChar(int x, int y, char c) {
    if (c < 0x20 || c > 0x7E) return;
    const uint8_t* glyph = font5x7[c - 0x20];
    for (int col = 0; col < 5; col++) {
        uint8_t bits = pgm_read_byte(&glyph[col]);
        for (int row = 0; row < 7; row++) {
            bool black = (bits >> row) & 1;
            setPixel(x + col, y + row, black);
        }
    }
}