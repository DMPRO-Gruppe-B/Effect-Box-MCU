#include "em_device.h"
#include "em_chip.h"
#include <stdio.h>

#include "hal-config.h"

#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  DISPLAY_Init();

  /* Retarget stdio to a text display. */
  if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
	  while (1);
  }

  /* Output text on Memory LCD */
  printf("\n");
  printf(" Effect Box IV\n");
  printf(" Gruppe B\n");
  printf(" Signature Edition");

  /* Infinite loop */
  while (1) {
  }
}
