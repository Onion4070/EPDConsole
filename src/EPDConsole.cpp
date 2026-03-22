#include "EPDConsole.h"
#include <Arduino.h>

EPDConsole::EPDConsole(FrameBuffer& fb): fb(fb) {
    cursorX = cursorY = 0;
    marginX = 0;

    for (int i = 0; i < ROWS; i++) {
        dirty_lines[i] = false;
    }
}

EPDConsole::~EPDConsole() {

}

void EPDConsole::setCursor(int x, int y) {
    cursorX = x;
    cursorY = cursorY;
    marginX = x;
}

void EPDConsole::setMargin(int x) {
    marginX = x;
}

void EPDConsole::advanceCursor() {
    cursorX += CHAR_W;

    // はみ出したら改行
    if (cursorX + CHAR_W >= EPD_WIDTH) {
        cursorX = marginX;
        cursorY += CHAR_H;
    }
}

void EPDConsole::print(const char c) {
    if (c == '\n') {
        cursorX = marginX;
        cursorY += CHAR_H;
        return;
    }
    fb.drawChar(cursorX, cursorY, c);
    dirty_lines[cursorY / CHAR_H] = true;
    advanceCursor();
}

void EPDConsole::print(const char* s) {
    while (*s) print(*s++);
}

void EPDConsole::println(const char c) {
    print(c);
    print('\n');
    flush();
}

void EPDConsole::println(const char* s) {
    print(s);
    print('\n');
    flush();
}

void EPDConsole::println() {
    print('\n');
    flush();
}

void EPDConsole::flush(int mode) {
    EPD_Init_Part();
    for (int row = 0; row < ROWS; row++) {
        if (!dirty_lines[row]) continue;
        int y = row * CHAR_H;
        const unsigned char* line_ptr = fb.buf + y * BYTES_PER_ROW;
        EPD_Dis_Part(
            0,          // x_start
            y,          // y_start
            line_ptr,   // Data
            CHAR_H,     // Height
            EPD_WIDTH,  // Width
            mode        // mode (first: 0, other: 1)
        );

        dirty_lines[row] = false;
    }
}