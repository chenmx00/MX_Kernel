//monitor.h -- Implements the interface for monitor.h
//Ian C. -- Credit from JamesM.
#include "monitor.h"
u16int *video_memory = (u16int *) 0xB8000;
u8int cursor_x = 0;
u8int cursor_y = 0;  


//Update the harware cursor.
static void move_cursor(){
    //The screen is 80 character wide.
    u16int cursor_location = 80 * cursor_y + cursor_x;
    out_byte(0x3D4, 14); //Tells the VGA controller's port 0X3D4 we are sending the high byte.
    out_byte(0x3D5, cursor_location >> 8);
    out_byte(0x3D4, 15); //Tells the VGA controller's port 0x3D4 we are sending the low byte.
    out_byte(0x3D5, cursor_location);
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

//Write a single character to the screen.
void monitor_put(char c){
    // The background color is black(0), the foreground is white(15)
    u8int backColor = 0;
    u8int foreColor = 15;

    //The attribute byte is made up of two nibbles - The lower being the 
    //foreground color, and the upper the background color. 
    u8int attributeByte = (backColor << 4) | (foreColor & 0x0F);
    u16int attribute = attributeByte << 8;
    u16int blank = 0x20 | (attributeByte << 8); //Writing the space character into the rightmost 8 bits.
    u16int *location;

    //handle a backspace, by moving the cursor back one space.
    if (c == 0x08 && cursor_x){
        location = video_memory + (cursor_y * 80 + cursor_x-1);
        *location = blank;
        cursor_x --;
    }

    //handle a tab by increasing the cursor's X, but only to a point where it is divisible by 8.
    if (c == 0x09 && cursor_x){
        cursor_x = (cursor_x + 8) & ~ (8 - 1);
    }

    //handle carriage return
    else if (c == '\r'){
        cursor_x = 0;
    }

    //handle newline by moving cursor back to left and increasing the row.
    else if (c == '\n'){
        cursor_x = 0;
        cursor_y ++;
    }

    else if(c >= ' '){
        location = video_memory + (cursor_y * 80 + cursor_x);
        *location = c | attribute;
        cursor_x ++;
    }

    if (cursor_x >= 80){
        cursor_x = 0;
        cursor_y ++;
    }
    scroll();
    move_cursor();
}

//Clear the screen while copying spaces to the framebuffer.
void monitor_clear(){
    //make an attribute byte for the default code.
    u8int attributeByte = (0 << 4) | (15 & 0x0F);
    u16int blank = 0x20 | (attributeByte << 8);
    int i;
    for (i = 0; i < 80 * 25; i++){
        video_memory[i] = blank;
    }
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

//Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c){
    int i = 0;
    while (c[i]){
        monitor_put(c[i++]);
    }
}



