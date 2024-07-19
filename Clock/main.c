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

const int mday = 3, month = 3, year = 2020; // Day of month, month and year
const int hours = 8, minutes = 10, seconds = 0; // Hours, minutes and seconds
const int wday = 4; // Days from Sunday. Sunday is 0, Monday is 1 and so on.
const int dst  = 0; // Daylight Savings. 0 - Disabled, 1 - Enabled

// Setting the tm structure as 08 HRS:10 MIN:10 SEC ; 3rd March 2020; Wednesday ; DST off
struct tm new_date_time =
{
	.tm_sec   = seconds,
	.tm_min   = minutes,
	.tm_hour  = hours,
	.tm_mday  = mday,
	.tm_mon   = month - 1,
	.tm_year  = year - TM_YEAR_BASE,
	.tm_wday  = wday,
	.tm_isdst = dst
};
struct tm current_date_timee = {0};

void printfTFT(cyhal_rtc_t *rtc_ptr);
void tftColor(mtb_light_sensor_t *lightObj_ptr);
void capsense();

const cyhal_timer_cfg_t timer_cfg =
{
	.compare_value = 0,                  // Timer compare value, not used
	.period        = 9999,             // Defines the timer period for 15 seconds
	.direction     = CYHAL_TIMER_DIR_UP, // Timer counts up
	.is_compare    = false,              // Don't use compare mode
	.is_continuous = true,               // Run the timer indefinitely
	.value         = 0                   // Initial value of counter
};

static void isr_timer(void* callback_arg, cyhal_timer_event_t event)
{
	cyhal_rtc_read(&my_rtc_obj, &current_date_timee);
	strftime(buffer, sizeof(buffer), "%c", &current_date_timee);
	printf("%s - TIMER\r\n", buffer);

	printfTFT(&my_rtc_obj);
}

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
