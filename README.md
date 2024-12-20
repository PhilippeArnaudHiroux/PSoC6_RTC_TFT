# PSoC6_RTC_TFT
In this project a clock is made that you can set using the capsense sensor.</br>
[Video](https://youtu.be/5wDyNYDAxGc)

## What has been used
### Material
* [PSoC 6 wifi-bt pioneer kit](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-062-wifi-bt/)
* [CY8CKIT-028-TFT display shield](https://www.infineon.com/cms/en/product/evaluation-boards/cy8ckit-028-tft/)

### Software
* [ModusToolbox](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software/)

### Peripheries
* Capsense sensor
  * Slider
  * Button0
  * Button1
* TFT-display
* Ambient light sensor
* RTC
  * RTC general
  * RTC alarm interupt
* Timer
    * Timer general
    * Timer interrupt

## How does it work
In the photo below you see the start screen afther programming the PSoC.
![TFT_Start_Screen](Documents/Photo/TFT_Start_Screen.jpg)

By sliding from the left to the right side of the capsence slider, you select the next time element (eg: From year to month, from month to day). By sliding from the right to the left side of the capsence slider, you can select the previous time element (eg: From month to year, from day to month)
![Capsense flowchart](Documents/Flowchart/Capsense_Flowchart.jpg)

WIth the left capsence button (button 0) you can decrease your time element, and with the right capsence button (button 1) you can increase your time element.

<details>
  <summary>Flowchart of configuring year</summary>

![Year_Flowchart](Documents/Flowchart/Year_Flowchart.jpg)
</details>
<details>
<summary>Flowchart of configuring month</summary>

![Year_Flowchart](Documents/Flowchart/Month_Flowchart.jpg)
</details>
<details>
<summary>Flowchart of configuring day</summary>

![Year_Flowchart](Documents/Flowchart/Day_Flowchart.jpg)
</details>
<details>
<summary>Flowchart of configuring hour</summary>

![Year_Flowchart](Documents/Flowchart/Hour_Flowchart.jpg)
</details>
<details>
<summary>Flowchart of configuring minute</summary>

![Year_Flowchart](Documents/Flowchart/Minute_Flowchart.jpg)
</details>
<details>
<summary>Flowchart of configuring second</summary>

![Year_Flowchart](Documents/Flowchart/Second_Flowchart.jpg)
</details>

The values on the TFT-display will be updated and shown.
![TFT_Set_Time](Documents/Photo/TFT_Set_Time.jpg)

When you configured the right time, you can push the user button (sw2). By presing this button the RTC will set himself with this time. And the time will be displayed on the TFT-shield.

Every second a timer will generate an interrupt to update the current time on the TFT-shield. First I wanted to use a RTC interrupt for it, but the TFT-shield does not wanted to work in a RTC interrupt.
![TFT_Time](Documents/Photo/TFT_Time.jpg)

With the ambient light sensor you can check the light intensity in the room. When the sensor measure a light intesity that is to low (eg: During the night) it will change the color of the text on the TFT-shield from white to red. If the light intesity is hight enough, the text will change again from red to white.
![TFT_Light_Sensor](Documents/Photo/TFT_Light_Sensor.jpg)

## Experience
* The TFT-Shield does not want to work in a RTC interrupt, but will work in a timer interrupt. But the RTC interrupt does want to execute the GUI_Clear() function.
* When there is text displayed on the TFT-shield, and you overwrite this text with a shorter text then the last characters of the longer text will be still displayed.
* When a new day start it is better to clear the whole TFT-shield. Otherwise it is possible that text from the previous day is still displayed.

## Source
ModusToolbox documentation
* [HAL](https://infineon.github.io/mtb-hal-cat1/html/group__group__hal__rtc.html)
* [Ambient light sensor](https://github.com/Infineon/sensor-light)
* [Capsense](https://infineon.github.io/capsense/capsense_api_reference_manual/html/group__group__capsense__high__level.html)
