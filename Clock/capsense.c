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
void processTouch(void);										//Declare the fucntion processTouch

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
    		processTouch();											//Update LED according to any touches
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

void processTouch(void) 						//Check if CapSense buttons were pressed and update the LED state accordingly
{
	uint32_t button0_status;					//Vars to hold widget statuses
	uint32_t button1_status;					//Vars to hold widget statuses
	cy_stc_capsense_touch_t *slider_touch_info;	//Vars to hold widget statuses
	uint16_t slider_pos;						//Vars to hold widget statuses
	uint8_t slider_touch_status;				//Vars to hold widget statuses
	static uint32_t button0_status_prev;		//Vars to hold previous widget statuses
	static uint32_t button1_status_prev;		//Vars to hold previous widget statuses
	static uint16_t slider_pos_prev;			//Vars to hold previous widget statuses

	button0_status = Cy_CapSense_IsSensorActive(CY_CAPSENSE_BUTTON0_WDGT_ID, CY_CAPSENSE_BUTTON0_SNS0_ID, &cy_capsense_context);	//Get button0 status
	button1_status = Cy_CapSense_IsSensorActive(CY_CAPSENSE_BUTTON1_WDGT_ID, CY_CAPSENSE_BUTTON1_SNS0_ID, &cy_capsense_context);	//Get button1 status

	slider_touch_info = Cy_CapSense_GetTouchInfo(CY_CAPSENSE_LINEARSLIDER0_WDGT_ID, &cy_capsense_context);	//Get slider status
	slider_touch_status = slider_touch_info->numPosition;													//Get slider status
	slider_pos = slider_touch_info->ptrPosition->x;															//Get slider status

	if((button0_status != 0U) && (button0_status != button0_status_prev)){	//If button 0 of capsense has been pressed
		printf("One meter forward\r\n");									//Print out the text
		i++;																//Do i + 1
		//robot_flash_read();
	}

	if((button1_status != 0U) && (button1_status != button1_status_prev)){	//If button 1 of capsense has been pressed
		printf("One meter backward\r\n");									//Print out the text
		i++;																//Do i + 1
		//robot_flash_read();
	}

	// Print value when slider is pressed
	if(slider_touch_status != 0U && slider_pos != slider_pos_prev) {
		float value = (1.0 - ((float)slider_pos / (float)cy_capsense_context.ptrWdConfig[0].xResolution)) * 100;
		printf("Slider pressed, value: %.2f\n", value);
		slider_pos_prev = slider_pos; // Update previous position
	}

	// Print message when slider is released
	if(slider_touch_status == 0U && slider_pos_prev != 0) {
		printf("Slider released\n");
		slider_pos_prev = 0; // Reset previous position
	}

	button0_status_prev = button0_status;	//Update previous touch status
	button1_status_prev = button1_status;	//Update previous touch status
	slider_pos_prev = slider_pos;			//Update previous touch status
}

static void capsense_isr(void){Cy_CapSense_InterruptHandler(CYBSP_CSD_HW, &cy_capsense_context);}	//Interrupt Service Routines and Callbacks
void endScanCallback(cy_stc_active_scan_sns_t * ptrActiveScan){scanComplete = true;}				//Make scanComplete equal to true
