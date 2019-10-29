#include "em_device.h"
#include "em_chip.h"

#include "hal-config.h"

#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  DISPLAY_Init();

  /* Infinite loop */
  while (1) {
  }
}
