#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "cy8ckit_028_tft.h"
#include "GUI.h"

#define TM_YEAR_BASE 		1900u

//Objects
cyhal_rtc_t 		my_rtc_obj;
cyhal_timer_t 		timer_obj;
mtb_light_sensor_t 	*lightObj;

char buffer[80];
struct tm new_date_time = {0};
const int dst  = 0; // Daylight Savings. 0 - Disabled, 1 - Enabled

void printfTFT(cyhal_rtc_t *rtc_ptr);
void tftColor(mtb_light_sensor_t *lightObj_ptr);
void capsense();
const int getYear();
const int getMonth();
const int getDay();
const int getHour();
const int getMinute();
const int getSecond();

const cyhal_timer_cfg_t timer_cfg =
{
	.compare_value = 0,                  // Timer compare value, not used
	.period        = 9999,             // Defines the timer period for 15 seconds
	.direction     = CYHAL_TIMER_DIR_UP, // Timer counts up
	.is_compare    = false,              // Don't use compare mode
	.is_continuous = true,               // Run the timer indefinitely
	.value         = 0                   // Initial value of counter
};

static void isr_timer(void* callback_arg, cyhal_timer_event_t event){printfTFT(&my_rtc_obj);}

int main(void)
{
    cybsp_init();
    __enable_irq();

    //Init UART
	cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);
	printf("\x1b[2J\x1b[;H");
	printf("Clock\r\n");

	//Init tft-shield
    cy8ckit_028_tft_init (NULL, NULL, NULL, NULL);
    GUI_Init();
    lightObj = cy8ckit_028_tft_get_light_sensor();

    capsense();

    //Init RTC
    cyhal_rtc_init(&my_rtc_obj);
    new_date_time.tm_year = getYear() - TM_YEAR_BASE;
    new_date_time.tm_mon = getMonth();
    new_date_time.tm_mday = getDay();
    new_date_time.tm_hour = getHour();
    new_date_time.tm_min = getMinute();
    new_date_time.tm_sec = getSecond();
    new_date_time.tm_isdst = dst;

    cyhal_rtc_write(&my_rtc_obj, &new_date_time);

    //Init timer
    cyhal_timer_init(&timer_obj, NC, NULL);
	cyhal_timer_configure(&timer_obj, &timer_cfg);
	cyhal_timer_set_frequency(&timer_obj, 10000);
	cyhal_timer_register_callback(&timer_obj, isr_timer, NULL);
	cyhal_timer_enable_event(&timer_obj, CYHAL_TIMER_IRQ_TERMINAL_COUNT, 3, true);
	cyhal_timer_start(&timer_obj);

    for(;;)
    {
    	tftColor(lightObj);
    	cyhal_syspm_sleep();
    }
}
