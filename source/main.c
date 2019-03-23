#include <stdio.h>
#include <string.h>
#include <3ds.h>

// Get current Cartridge Slot status
bool get_slot_status() {
    bool status;
    FSUSER_CardSlotGetCardIFPowerStatus(&status);
    return status;
}

// Switch cartridge slot status
void switch_slot_status(bool status) {
    bool newstatus;
    if (status) {
        FSUSER_CardSlotPowerOff(&newstatus);
    } else {
        FSUSER_CardSlotPowerOn(&newstatus);
    }
    // Doesn't seem to return the new power status correctly?
}

int main(int argc, char** argv)
{
    // Initialize services
    gfxInitDefault();
    fsInit();
  
    // Init console for text output
    consoleInit(GFX_TOP, NULL);
    consoleClear();
    
    // Begin Code here
    printf(CONSOLE_GREEN "3DS Power Off Cartridge Slot" CONSOLE_RESET "\n");
    printf("Press A to switch cartridge status.\n");
    printf("Press START to exit.\n");
    
    bool status = get_slot_status();

    // Main loop
    while (aptMainLoop())
    {
        printf("\x1b[5;0H");
        if (status) {
            printf("Cartridge Slot is:" CONSOLE_BLUE " ENABLED " CONSOLE_RESET "\n");
        } else {
            printf("Cartridge Slot is:" CONSOLE_RED " DISABLED" CONSOLE_RESET "\n");
        }
        
        hidScanInput();
        u32 kDown = hidKeysDown();
        
        if (kDown & KEY_START) {
            break; // break in order to return to hbmenu
        }
        
        if (kDown & KEY_A) {
            switch_slot_status(status);
            status = get_slot_status();
        }

        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    // Exit services
    fsExit();
    gfxExit();
    return 0;
}
