#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cycfg_capsense.h"
#include "cycfg.h"
#include "cy_retarget_io.h"

#define CAPSENSE_INTR_PRIORITY 7U

bool scanComplete = false;

void initCapSense(void);										//Declare the function initCapSense
static void capsense_isr(void);									//Declare the function capsense_isr
void endScanCallback(cy_stc_active_scan_sns_t * ptrActiveScan);	//Declare the function endScanCallback
void capsenseSlider(void);										//Declare the fucntion processTouch
void capsenseButtonLeft(void);
void capsenseButtonRight(void);

int i = 0;	//Variable to save the index for the flash memory

void capsense(void)
{
	initCapSense();																//Init capsense
    cyhal_gpio_init(P0_4, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_NONE, false);	//Init SW2
	Cy_CapSense_ScanAllWidgets(&cy_capsense_context);							//Start first scan

    for (;;)
    {
    	if(scanComplete == true)									//Wait for scan to complete
    	{
    		Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);	//Process scan data
    		Cy_CapSense_RunTuner(&cy_capsense_context);				//Update tuner
    		capsenseSlider();											//Update LED according to any touches
    		capsenseButtonLeft();
    		capsenseButtonRight();
			Cy_CapSense_ScanAllWidgets(&cy_capsense_context);		//Start next scan
			scanComplete = false;									//Make scanComplete equal to false
    	}
    	if(cyhal_gpio_read(P0_4) == 0){break;}						//If SW2 is pressed, go out of the for loop
    }
}

void initCapSense(void) //Initializes the CSD HW block for touch sensing
{
	const cy_stc_sysint_t CapSense_interrupt_config = {
			.intrSrc = CYBSP_CSD_IRQ,
			.intrPriority = CAPSENSE_INTR_PRIORITY
	};

	Cy_CapSense_Init(&cy_capsense_context);															//Initialize CSD HW block
	Cy_SysInt_Init(&CapSense_interrupt_config, capsense_isr);										//Initialize CapSense interrupt
	NVIC_ClearPendingIRQ(CapSense_interrupt_config.intrSrc);										//Initialize CapSense interrupt
	NVIC_EnableIRQ(CapSense_interrupt_config.intrSrc);												//Initialize CapSense interrupt
	Cy_CapSense_Enable(&cy_capsense_context);														//Enable CapSense firmware modules
	Cy_CapSense_RegisterCallback(CY_CAPSENSE_END_OF_SCAN_E, endScanCallback, &cy_capsense_context);	//Initialize end of scan callback
}

void capsenseButtonLeft(void)
{
	uint32_t button0_status;					//Vars to hold widget statuses
	static uint32_t button0_status_prev;		//Vars to hold previous widget statuses
	button0_status = Cy_CapSense_IsSensorActive(CY_CAPSENSE_BUTTON0_WDGT_ID, CY_CAPSENSE_BUTTON0_SNS0_ID, &cy_capsense_context);	//Get button0 status

	if((button0_status != 0U) && (button0_status != button0_status_prev))
	{
		printf("left button\r\n");									//Print out the text
	}

	button0_status_prev = button0_status;	//Update previous touch status
}

void capsenseButtonRight(void)
{
	uint32_t button1_status;					//Vars to hold widget statuses
	static uint32_t button1_status_prev;		//Vars to hold previous widget statuses
	button1_status = Cy_CapSense_IsSensorActive(CY_CAPSENSE_BUTTON1_WDGT_ID, CY_CAPSENSE_BUTTON1_SNS0_ID, &cy_capsense_context);	//Get button1 status

	if((button1_status != 0U) && (button1_status != button1_status_prev))
	{
		printf("Right button\r\n");									//Print out the text
	}

	button1_status_prev = button1_status;	//Update previous touch status
}

void capsenseSlider(void) 						//Check if CapSense buttons were pressed and update the LED state accordingly
{
	cy_stc_capsense_touch_t *slider_touch_info;	// Vars to hold widget statuses
	uint16_t slider_pos = 0;					// Vars to hold widget statuses
	uint8_t slider_touch_status;				// Vars to hold widget statuses
	static uint8_t was_touched = 0;				// Vars to track if slider was previously touched

	// Get slider status
	slider_touch_info = Cy_CapSense_GetTouchInfo(CY_CAPSENSE_LINEARSLIDER0_WDGT_ID, &cy_capsense_context);
	slider_touch_status = slider_touch_info->numPosition;
	slider_pos = slider_touch_info->ptrPosition->x;

	if((slider_touch_status != 0U) && (was_touched == 0U))
	{
		float value = (1.0 - ((float)slider_pos / (float)cy_capsense_context.ptrWdConfig[CY_CAPSENSE_LINEARSLIDER0_WDGT_ID].xResolution)) * 100; //Calculate de value of the slider
		printf("Value aanraken: %.0f\r\n", value);
		was_touched = 1;  // Update touch status
	}

	if (slider_touch_status == 0U && was_touched == 1U)
	{
		float value = (1.0 - ((float)slider_pos / (float)cy_capsense_context.ptrWdConfig[CY_CAPSENSE_LINEARSLIDER0_WDGT_ID].xResolution)) * 100; //Calculate de value of the slider
		printf("Value loslaten: %.0f\r\n", value);
		was_touched = 0;  // Update touch status
	}
}

static void capsense_isr(void){Cy_CapSense_InterruptHandler(CYBSP_CSD_HW, &cy_capsense_context);}	//Interrupt Service Routines and Callbacks
void endScanCallback(cy_stc_active_scan_sns_t * ptrActiveScan){scanComplete = true;}				//Make scanComplete equal to true
