#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"

void capsense(void);

int main(void)
{
    cybsp_init();
    __enable_irq();

    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE); //Init UART
	printf("\x1b[2J\x1b[;H");

	capsense();

    for (;;)
    {
    }
}
