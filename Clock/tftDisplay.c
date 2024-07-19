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

void printfDate()
{
	char str[10];
	GUI_SetFont(&GUI_Font32B_ASCII);
	GUI_DispStringAt("Year: ", 0, 0);
	GUI_DispStringAt(itoa(2000, str, 10), 150, 0);
	GUI_DispStringAt("Month: ", 0, 30);
	GUI_DispStringAt("January", 150, 30);
	GUI_DispStringAt("Day : ", 0, 60);
	GUI_DispStringAt(itoa(1, str, 10), 150, 60);
	GUI_DispStringAt("Weekday: ", 0, 90);
	GUI_DispStringAt("Monday", 150, 90);
	GUI_DispStringAt("Hour: ", 0, 120);
	GUI_DispStringAt(itoa(0, str, 10), 150, 120);
	GUI_DispStringAt("Minute: ", 0, 150);
	GUI_DispStringAt(itoa(0, str, 10), 150, 150);
	GUI_DispStringAt("Second: ", 0, 180);
	GUI_DispStringAt(itoa(0, str, 10), 150, 180);
}

void printfYear(int value)
{
	char str[10];
	GUI_DispStringAt(itoa(value, str, 10), 150, 0);
}

void printfMonth(int value)
{
	switch(value)
	{
		case 0:
			GUI_DispStringAt("January      ", 150, 30);
			break;
		case 1:
			GUI_DispStringAt("February     ", 150, 30);
			break;
		case 2:
			GUI_DispStringAt("March        ", 150, 30);
			break;
		case 3:
			GUI_DispStringAt("April        ", 150, 30);
			break;
		case 4:
			GUI_DispStringAt("May          ", 150, 30);
			break;
		case 5:
			GUI_DispStringAt("June         ", 150, 30);
			break;
		case 6:
			GUI_DispStringAt("July         ", 150, 30);
			break;
		case 7:
			GUI_DispStringAt("August       ", 150, 30);
			break;
		case 8:
			GUI_DispStringAt("September", 150, 30);
			break;
		case 9:
			GUI_DispStringAt("October      ", 150, 30);
			break;
		case 10:
			GUI_DispStringAt("November     ", 150, 30);
			break;
		case 11:
			GUI_DispStringAt("December   ", 150, 30);
			break;
	}
}

void printfDay(int value)
{
	char str[10];
	itoa(value, str, 10);
	GUI_DispStringAt(strcat(str, "  "), 150, 60);
}

void printfWeekday(int value)
{
	switch(value)
	{
		case 0:
			GUI_DispStringAt("Sunday      ", 150, 90);
			break;
		case 1:
			GUI_DispStringAt("Monday      ", 150, 90);
			break;
		case 2:
			GUI_DispStringAt("Tuesday     ", 150, 90);
			break;
		case 3:
			GUI_DispStringAt("Wednesday", 150, 90);
			break;
		case 4:
			GUI_DispStringAt("Thursday    ", 150, 90);
			break;
		case 5:
			GUI_DispStringAt("Friday      ", 150, 90);
			break;
		case 6:
			GUI_DispStringAt("Saturday    ", 150, 90);
			break;
	}
}

void printfHour(int value)
{
	char str[10];
	itoa(value, str, 10);
	GUI_DispStringAt(strcat(str, "  "), 150, 120);
}

void printfMinute(int value)
{
	char str[10];
	itoa(value, str, 10);
	GUI_DispStringAt(strcat(str, "  "), 150, 150);
}
void printfSecond(int value)
{
	char str[10];
	itoa(value, str, 10);
	GUI_DispStringAt(strcat(str, "  "), 150, 180);
}

void clearTFT()
{
	GUI_Clear();
}
