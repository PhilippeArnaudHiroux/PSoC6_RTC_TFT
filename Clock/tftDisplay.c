#include "cyhal.h"
#include "cy8ckit_028_tft.h"
#include "GUI.h"
#include "cy_retarget_io.h"

struct tm current_date_time = {0};
//x = 320 en y = 240

#define	HOUR_MINUTE_X  	22
#define	HOUR_MINUTE_Y  	0
#define	SECOND_X		174
#define SECOND_Y  		100
#define WEEKDAY_X  		22
#define WEEKDAY_Y		100
#define DAY_MONTH_X		22
#define DAY_MONTH_Y		130
#define YEAR_X			22
#define YEAR_Y			160

char hourMinuteBuf[8];
char secondBuf[4];
char weekdayBuf[10];
char dayMonthBuf[10];
char yearBuf[8];

uint8_t lightData;

void printfTFT(cyhal_rtc_t *rtc_ptr)
{
	cyhal_rtc_read(rtc_ptr, &current_date_time);

	//Create hour and minute
	GUI_SetFont(&GUI_FontD80);
	strftime(hourMinuteBuf, sizeof(hourMinuteBuf), "%H:%M", &current_date_time);
	GUI_DispStringAt(hourMinuteBuf, HOUR_MINUTE_X, HOUR_MINUTE_Y);

	//Create second
	GUI_SetFont(&GUI_FontD80);
	strftime(secondBuf, sizeof(secondBuf), "%S", &current_date_time);
	GUI_DispStringAt(secondBuf, SECOND_X, SECOND_Y);

	//Create weekday
	GUI_SetFont(&GUI_Font32B_ASCII);
	strftime(weekdayBuf, sizeof(weekdayBuf), "%A", &current_date_time);
	GUI_DispStringAt(weekdayBuf, WEEKDAY_X, WEEKDAY_Y);

	//Create day and month
	GUI_SetFont(&GUI_Font32B_ASCII);
	strftime(dayMonthBuf, sizeof(dayMonthBuf), "%d %b", &current_date_time);
	GUI_DispStringAt(dayMonthBuf, DAY_MONTH_X, DAY_MONTH_Y);

	//Create year
	GUI_SetFont(&GUI_FontD32);
	strftime(yearBuf, sizeof(yearBuf), "%Y", &current_date_time);
	GUI_DispStringAt(yearBuf, YEAR_X, YEAR_Y);
}

void tftColor(mtb_light_sensor_t *lightObj_ptr)
{
	uint8_t lightData;
	lightData = mtb_light_sensor_light_level(lightObj_ptr); //Get light level
	if(lightData < 10)
	{
		GUI_SetColor(GUI_DARKRED);
	}
	else
	{
		GUI_SetColor(GUI_WHITE);
	}
	printf("test = %d\r\n", lightData);
}
