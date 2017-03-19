/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "nrf.h"
#include <stdbool.h>
#include <stdint.h>
#include "bsp.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "math.h"

// Peripheral channel assignments
#define PWM0_GPIOTE_CH      0
#define PWM0_PPI_CH_A       0
#define PWM0_PPI_CH_B       1
#define PWM0_TIMER_CC_NUM   0

// TIMER3 reload value. The PWM frequency equals '16000000 / TIMER_RELOAD'
#define TIMER_RELOAD        1024
// The timer CC register used to reset the timer. Be aware that not all timers in the nRF52 have 6 CC registers.
#define TIMER_RELOAD_CC_NUM 5


// This function initializes timer 3 with the following configuration:
// 24-bit, base frequency 16 MHz, auto clear on COMPARE5 match (CC5 = TIMER_RELOAD)
void timer_init()
{
    NRF_TIMER3->BITMODE                 = TIMER_BITMODE_BITMODE_24Bit << TIMER_BITMODE_BITMODE_Pos;
    NRF_TIMER3->PRESCALER               = 0;
    NRF_TIMER3->SHORTS                  = TIMER_SHORTS_COMPARE0_CLEAR_Msk << TIMER_RELOAD_CC_NUM;
    NRF_TIMER3->MODE                    = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
    NRF_TIMER3->CC[TIMER_RELOAD_CC_NUM] = TIMER_RELOAD;    
}


// Starts TIMER3
void timer_start()
{
    NRF_TIMER3->TASKS_START = 1;
}


// This function sets up TIMER3, the PPI and the GPIOTE modules to configure a single PWM channel
// Timer CC num, PPI channel nums and GPIOTE channel num is defined at the top of this file
void pwm0_init(uint32_t pinselect)
{  
    NRF_GPIOTE->CONFIG[PWM0_GPIOTE_CH] = GPIOTE_CONFIG_MODE_Task << GPIOTE_CONFIG_MODE_Pos | 
                                         GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos | 
                                         pinselect << GPIOTE_CONFIG_PSEL_Pos | 
                                         GPIOTE_CONFIG_OUTINIT_High << GPIOTE_CONFIG_OUTINIT_Pos;

    NRF_PPI->CH[PWM0_PPI_CH_A].EEP = (uint32_t)&NRF_TIMER3->EVENTS_COMPARE[PWM0_TIMER_CC_NUM];
    NRF_PPI->CH[PWM0_PPI_CH_A].TEP = (uint32_t)&NRF_GPIOTE->TASKS_CLR[PWM0_GPIOTE_CH];
    NRF_PPI->CH[PWM0_PPI_CH_B].EEP = (uint32_t)&NRF_TIMER3->EVENTS_COMPARE[TIMER_RELOAD_CC_NUM];
    NRF_PPI->CH[PWM0_PPI_CH_B].TEP = (uint32_t)&NRF_GPIOTE->TASKS_SET[PWM0_GPIOTE_CH];
    
    NRF_PPI->CHENSET               = (1 << PWM0_PPI_CH_A) | (1 << PWM0_PPI_CH_B);
}


// ### ------------------ TASK 1: STEP 1 ------------------
// ### Implement a method for initializing PWM channel 1, for a total of 2 individual PWM channels, and call it 'pwm1_init(uint32_t pinselect)'
// ### Hint: You can copy 'pwm0_init(..)' and use it as a starting point

// ### ----------------------------------------------------


// ### ------------------ TASK 1: STEP 2 ------------------
// ### Using the FORK feature of the PPI try to modify 'pwm1_init(..)' to reduce the number of PPI channels used
// ### (in total it should be sufficient to use 3 PPI channels for 2 PWM outputs).
// ### Hint: The FORK feature is useful when the same event needs to trigger several tasks.
// ### ----------------------------------------------------


// ### ------------------ TASK 1: STEP 6 (optional)--------
// ### Make a generic init function that takes both the PWM channel number and the pinselect as arguments,
// ### to avoid having to implement one function for each channel. The function should support up to 4 PWM channels total.
// ### Hint: Don't start on the optional steps until all the required steps are complete. 

// ### ----------------------------------------------------


// Function for changing the duty cycle on PWM channel 0
void pwm0_set_duty_cycle(uint32_t value)
{
    if(value == 0)
    {
        value = 1;
    }
    else if(value >= TIMER_RELOAD)
    {
        value = TIMER_RELOAD - 1;
    }
    NRF_TIMER3->CC[PWM0_TIMER_CC_NUM] = value;
}


// ### ------------------ TASK 1: STEP 3 ------------------
// ### Implement a method for setting the duty cycle on PWM channel 1, and call it 'pwm1_set_duty_cycle(uint32_t value)'
// ### Hint: You can copy 'pwm0_set_duty_cycle(..)' and use it as a starting point

// ### ----------------------------------------------------


// ### ------------------ TASK 1: STEP 7 (optional) ----------
// ### Make a generic set duty cycle function to support a total of 4 PWM channels.

// ### ----------------------------------------------------


// ### ------------------ TASK 1: STEP 8 (optional) ----------
// ### Find a better workaround for a duty cycle of 0% or 100%, so that it is possible to set the PWM output either constantly high or low.
// ### Implement the workaround in 'pwmN_set_duty_cycle(uint32_t N, uint32_t value)'
// ### -------------------------------------------------------


// Utility function for providing sin values, and converting them to integers.
// input values in the range [0 - input_max] will be converted to 0-360 degrees (0-2*PI).
// output values will be scaled to the range [output_min - output_max].
uint32_t sin_scaled(uint32_t input, uint32_t input_max, uint32_t output_min, uint32_t output_max)
{
    float sin_val = sinf((float)input * 2.0f * 3.141592f / (float)input_max);
    return (uint32_t)(((sin_val + 1.0f) / 2.0f) * (float)(output_max - output_min)) + output_min; 
}


int main(void)
{
    uint32_t counter = 0;
    
    // Initialize the timer
    timer_init();
    
    // Initialize PWM channel 0
    pwm0_init(LED_1);
    
    // ### ------------------ TASK 1: STEP 4 ------------------
    // ### Call the init function implemented in STEP 1, and configure the additional PWM channel on LED_2.

    // ### ----------------------------------------------------
    
    // ### ------------------ TASK 1: STEP 9 (optional) -------
    // ### Call the generic init function implemented in STEP 6, and configure 2 more PWM channels on LED_3 and LED_4.

    // ### ----------------------------------------------------
    
    // Start the timer
    timer_start();

    while (true)
    {
        nrf_delay_us(4000);
        
        // Update the duty cycle of PWM channel 0 and increment the counter.
        pwm0_set_duty_cycle(sin_scaled(counter++, 200, 0, TIMER_RELOAD));
        
        // ### ------------------ TASK 1: STEP 5 ------------------
        // ### Update the duty cycle of PWM channel 1, and add an offset to the counter to make the LED's blink out of phase.

        // ### ----------------------------------------------------
        
        // ### ------------------ TASK 1: STEP 10 (optional) ------
        // ### Update the duty cycle of PWM channel 2 and 3, using the generic functions implemented earlier.
 
        // ### ----------------------------------------------------        
    }
}
