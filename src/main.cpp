#include <SPI.h>
// EPD
#include "Display_EPD_W21.h"
#include "Display_EPD_W21_spi.h"

#include "FrameBuffer.h"
#include "EPDConsole.h"

void setup() {
    pinMode(A14, INPUT);   // BUSY
    pinMode(A15, OUTPUT);  // RES
    pinMode(A16, OUTPUT);  // DC
    pinMode(A17, OUTPUT);  // CS
    // SPI
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    SPI.begin();

    Serial.begin(115200);
}

// Tips//
/*
1.Flickering is normal when EPD is performing a full screen update to clear
ghosting from the previous image so to ensure better clarity and legibility for
the new image. 2.There will be no flicker when EPD performs a partial refresh.
3.Please make sue that EPD enters sleep mode when refresh is completed and
always leave the sleep mode command. Otherwise, this may result in a reduced
lifespan of EPD. 4.Please refrain from inserting EPD to the FPC socket or
unplugging it when the MCU is being powered to prevent potential damage.)
5.Re-initialization is required for every full screen update.
6.When porting the program, set the BUSY pin to input mode and other pins to
output mode.
*/
FrameBuffer fb;
EPDConsole console(fb);
void loop() {
    EPD_Init();
    EPD_SetRAMValue_BaseMap(fb.buf);

    console.print("Waiting for players");
    console.flush();

    for (int i = 0; i < 5; i++) {
        console.print('.');
        console.flush();
        delay(100);
    }

    console.println(" Done.");
    console.println("EPD Console test");
    console.println("Hello World");

    const char* src = 
    "#include <iostream>\n\nint main() {\n    std::cout << \"Hello World\" << std::endl;\n}";
    console.println(src);
    delay(3000);

    EPD_Init();
    EPD_Display(fb.buf);
    EPD_DeepSleep();

    while(1);
}

//////////////////////////////////END//////////////////////////////////////////////////
