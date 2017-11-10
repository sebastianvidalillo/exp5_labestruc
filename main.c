#include "msp430_version.h"
#include <stdio.h>
#include "display.h"
#include "osc.h"
#include "adc.h"
extern float promedio;
int x;
void main(void)
{
    osc_init_xt2();
    WDTCTL = WDTPW + WDTHOLD;

    display_init();
    
    CCTL0 = CCIE;             	// CCR0 habilita interrupción del timer
    
    CCR0 = 32768-1;
    
    P6DIR = 0x00;
    P6SEL = 0x01;
    P1DIR |= 0x04;
    P1SEL |= 0x04;
      
    TBCCTL0 = CCIE;
    TBCTL = TBSSEL_2 + MC_2;
    TACTL = TASSEL_1 + MC_1;
    TACCR0 = 625-1;
    x = (promedio/4095);
    
    if ((P1IN & 0x01) == 0x01 ) {
      TACCR1 = x*624;   }
    else {
      TACCR1 = (1-x)*624; }
           
    adc_init_single_channel_single_conv_it();
 
    adc_single_channel_single_conv_it(0);
   

    _BIS_SR(GIE);	// modo bajo consumo

    // Interrupciones
}


#define CONV(x)      x			// convertir x en voltaje

// Timer A0 rutina de servicio de interrupción
#pragma vector = TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
        adc_single_channel_single_conv_it(0);
	//int pos = display_get_pos();
	//int d;
	//d = adc_single_channel_single_conv(0);
	//printf("Vol=%1.3f\n", CONV(d));
	//display_set_pos(pos);
}

#pragma vector = TIMERB0_VECTOR
__interrupt void Timer_B (void)
{

}

