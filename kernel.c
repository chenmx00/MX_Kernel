/*
 * kernel.c
 * Ian C.
 */
void main(void) {
  const char *str = "Minxing's Kernel";
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
  while(str[j] != '\0'){
    vidptr[i] = str[i];
    vidptr[i+1] = 0x07;
    ++j;
    i = i + 2;
  }
  return;
}
