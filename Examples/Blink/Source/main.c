#include "PJ_RPI.h"
#include <stdio.h>

int main()
{
	if(map_peripheral(&gpio) == -1)
	{
       	 	printf("Failed to map the physical GPIO registers into the virtual memory space.\n");
        	return -1;
    	}

	// Define gpio 17 as output
	printf("Defining GPIO 17 as output");
	INP_GPIO(17);
	OUT_GPIO(17);

	while(1)
	{
		// Toggle 17 (blink a led!)
		printf("ON!!");
		GPIO_SET = 1 << 17;
		sleep(1);

		printf("OFF!!");
		GPIO_CLR = 1 << 17;
		sleep(1);
	}

	return 0;

}
