# Timer, GPIOTE, PPI Hands On
------

**Scope:** The point of this hands-on is to learn how to use the PPI, Timer and GPIOTE peripherals. 
These peripherals will be used to implement a multi channel PWM feature that doesn't rely on a dedicated PWM module or software bitbanging, just the flexibility of the task/event system and the PPI module. 

The picture below shows how the CC registers of a timer can be used to control several separate PWM channels through the PPI controller, as will be done in this example:


![](https://github.com/NordicPlayground/nrf52-timer-gpiote-ppi-hands-on/blob/master/pics/scope.png)

## Task 1: Add an additional PWM channel

1.  Implement a method for initializing PWM channel 1, for a total of 2 individual PWM channels, and call it *pwm1_init(uint32_t pinselect)*

	Hint: You can copy *pwm0_init(..)* and use it as a starting point.

	Hint2: Remember to duplicate and change the *PWM0_GPIOTE_CH*, *PWM0_PPI_CH_A*, *PWM0_PPI_CH_B* and *PWM0_TIMER_CC_NUM* defines. Otherwise PWM channel 1 will conflict with PWM channel 0.


2.  Using the FORK feature of the PPI try to modify the *pwm1_init(..)* function you implemented in step 1 to reduce the number of PPI channels used (in total it should be sufficient to use 3 PPI channels for 2 PWM outputs).
	
	Hint: The FORK feature is useful when the same event needs to trigger several tasks.


3.  Implement a method for setting the duty cycle on PWM channel 1, and call it *pwm1_set_duty_cycle(uint32_t value)*

	Hint: You can copy *pwm0_set_duty_cycle(..)* and use it as a starting point

4.  Call the init function implemented in STEP 1, and configure the additional PWM channel on LED_2.
 
5.  Update the duty cycle of PWM channel 1, and add an offset to the counter to make the LED's blink out of phase.

	### Optional: Add generic PWM functions for a total of 4 PWM channels

6.  Make a generic init function that takes both the PWM channel number and the pinselect as arguments, to avoid having to implement one function for each channel. The function should support up to 4 PWM channels total.

	Hint: Don't start on the optional steps until all the required steps are complete. 


7.  Make a generic set duty cycle function to support a total of 4 PWM channels.

8.  Find a better workaround for a duty cycle of 0% or 100%, so that it is possible to set the PWM output either constantly high or low.
	Implement the workaround in *pwmN_set_duty_cycle(uint32_t N, uint32_t value)*

    
9.	Call the generic init function implemented in STEP 6, and configure 2 more PWM channels on LED_3 and LED_4.

10.	Update the duty cycle of PWM channel 2 and 3, using the generic functions implemented earlier.