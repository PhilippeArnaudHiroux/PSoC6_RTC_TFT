#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "cy8ckit_028_tft.h"
#include "GUI.h"

int main(void)
{
    cybsp_init();
    __enable_irq();

	cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
    cy8ckit_028_tft_init (NULL, NULL, NULL, NULL);
    
    /* Initialize the emwin library */
	GUI_Init();
	GUI_SetFont(&GUI_FontD80);


	//print headers
	GUI_DispString("12:45");
	//GUI_DispStringAt("Accel: X:       Y:       Z:", 0, 10);
	//GUI_DispStringAt("Gyro:  X:       Y:       Z:", 0, 20);

    for(;;)
    {
    }
}
