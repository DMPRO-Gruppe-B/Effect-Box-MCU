#ifndef SRC_LCD_H_
#define SRC_LCD_H_

// TODO: Remove these and include them properly in textdisplay.h
#define TEXTDISPLAY_ESC_SEQ_CURSOR_HOME_VT100                                  \
  "\033[H" /**< Move cursor to upper left corner. */
#define TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE                                 \
  "\033A" /**< Move cursor up one line. */
#define TEXTDISPLAY_ESC_SEQ_CURSOR_DOWN_ONE_LINE                               \
  "\033B" /**< Move cursor down one line. */

#define EFFECTS 3

#define CURSOR_LINE ">%-12.12s%3d%5s\n"
#define LINE " %-12.12s%3d%5s\n"
#define CURSOR ">\n"
#define NO_CURSOR " \n"

// Effects
#define DELAY "delay"
#define COMPRESSOR "compressor"
#define BITCRUSHER "bitcrusher"

// Units
#define PERCENTAGE "%"
#define MILLISECONDS "ms"
#define BITS "bits"

void LCD_InitialRender();
void LCD_NavigateUp();
void LCD_NavigateDown();
void LCD_IncrementValue();
void LCD_DecrementValue();

#endif /* SRC_LCD_H_ */
