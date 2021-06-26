//monitor.h -- Implements the interface for monitor.h
//Ian C. -- Credit from JamesM.
#include "monitor.h"
u16int *video_memory = (u16int *) 0xB0000;
u8int cursor_x = 0;
u8int cursor_y = 0;  


//Update the harware cursor.
static void move_cursor(){
    //The screen is 80 character wide.
    u16int cursor_location = 80 * cursor_y + cursor_x;
    outb(0x3D4, 14); //Tells the VGA controller's port 0X3D4 we are sending the high byte.
    outb(0x3D4, cursor_location >> 8);
    outb(0x3D4, 15); //Tells the VGA controller's port 0x3D4 we are sending the low byte.
    outb(0x3D4, cursor_location);
}

//Scroll the text on the screen by one line.
static void scroll(){
    //Get a space character with the default color attributes.
    u8int attributeByte = (0 << 4) | (15 & 0x0F); // Writing black into the background buffer(left most 4 bits.) and white into the foreground buffer(mid 4 - 8 bits).
    u16int blank = 0x20 | (attributeByte << 8); //Writing the space character into the rightmost 8 bits.

    if(cursor_y >= 25){
        int i;
        for (i = 0; i < 24 * 80; i++){
            video_memory[i] = video_memory[i+80];
        }
        for (i = 24 * 80; i < 25*80; i++){
            video_memory[i] = blank;
        }

        cursor_y = 24;
    } 
}

