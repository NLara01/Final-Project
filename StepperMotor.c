// HEADER FILES //
#include<stdint.h>
#include<stdbool.h>
#include"inc/hw_memmap.h"
#include"inc/hw_types.h"
#include"driverlib/debug.h"
#include"driverlib/sysctl.h"
#include"driverlib/adc.h"
#include"inc/hw_types.h"
#include"driverlib/gpio.h"
#include"driverlib/pwm.h"
#include"driverlib/pin_map.h"
#include"inc/hw_gpio.h"
#include"driverlib/rom.h"uint32_t ui32ADC0Value[1];                                    // TO STORE THE VALUE IN VARIABLE ui32ADC0Value FOR EVERY SAMPLING
int main(void){
SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);      // SET SYSTEM CLOCK AT 40MHZ
SysCtlPWMClockSet(SYSCTL_PWMDIV_64);                                                   //SET PWM CLOCK AT SYSTEM CLOCK DIVIDED BY 64
SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);                                            //ENABLE PWM1 MODULE
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);                                            //ENABLE ADC0 MODULE
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);                                           //ENABLE GPIO FOR ADC0 MODULE
GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_3);                                            //CONFIGURE PE3 AS AN0
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);                                           //ENABLE GPIO FOR PWM1 MODULE
GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);                                           //CONFIGURE PD0 AS PWM OUTPUT
GPIOPinConfigure(0x00030005);                                                          //SET PD0 AS M1PWM0
PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);                              //SET PWM GENERATOR WITH MODEOF OPERATION AS COUNTING
PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0,4095);                                            //SET THE PERIOD OF PWM GENERATOR
PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);                                        //ENABLE BIT0 OUTPUT
PWMGenEnable(PWM1_BASE, PWM_GEN_0);                                                    //ENABLE PWM GENERATOR
ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);                          // ADC0 MODULE, TRIGGER IS PROCESSOR EVENT, SEQUENCER 0 IS CONFIGURED
ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH0);                                // ADC0 MODULE, SEQUENCER 0 , FOR 1 SAMPLING, INPUT IS FROM CHANNEL 0 PE3
ADCSequenceEnable(ADC0_BASE, 1);                                                       // ENABLE THE SEQUENCE 1 FOR ADC0

while(1){
ADCIntClear(ADC0_BASE, 1);                                                             // CLEAR INTERRUPT FLAG FOR ADC0, SEQUENCER 1
ADCProcessorTrigger(ADC0_BASE, 1);                                                     // TRIGGER IS GIVEN FOR ADC0 MODULE, SEQUENCER1

// STORE THE CONVERTED VALUE FOR ALL DIFFERENT SAMPLING IN ARRAY ui32ADC0Value
ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui32ADC0Value[0]);                              // SET THE PULSE WIDTH
}
}
