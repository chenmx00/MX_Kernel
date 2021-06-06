/*
 * kernel.c
 * Ian C.
 */
void kmain(void) {
  const char *str1 = "Kernel version 0.0.1";
  const char *str2 = "Minxing's first kernel";
  char *vidptr = (char*) 0xb8000; //beginning of video mem
  unsigned int i = 0;
  unsigned int j = 0;

  /** this loop clears the screen(framebuffer)
   * there are 25 lines of 80 columns; each element takes 2bytes */

  while(j < 80 * 25 * 2){
    vidptr[j] = ' ';
    vidptr[j+1] = 0x07;
    j = j + 2;
  }

  j = 0;
  
  /* this loop writes the string to video memory
   */
  while(str1[j] != '\0'){
    vidptr[i] = str1[j];
    vidptr[i+1] = 0x00 + (i) % 15 + 1;
    ++j;
    i = i + 2;
  }
  
  j = 0;
  i = 160;
  while(str2[j] != '\0'){
    vidptr[i] = str2[j];
    vidptr[i+1] = 0x00 + (i) % 15 + 1;
    ++j;
    i = i + 2;
  }
  return;
}
