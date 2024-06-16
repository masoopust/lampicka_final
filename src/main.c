#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
#include "uart1.h"
#include "adc_helper.h"
#include "daughterboard.h"




void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    init_milis();
    init_uart1();

    //na pinech vypneme vstupní buffer
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL14, DISABLE); // vypíná schmittův kl. obvof na tomto kanále(brání zákmitu)
    ADC2_SchmittTriggerConfig(ADC2_SCHMITTTRIG_CHANNEL15, DISABLE);
    //nastavíme clock pro ADC2 (16Hz / 4 = 4 MHz)
    ADC2_PrescalerConfig(ADC2_PRESSEL_FCPU_D4); // bude dělit 4, výsledek musí být < 5
    //Zvolím zarovnání výsledku
    ADC2_AlignConfig(ADC2_ALIGN_RIGHT);
    //nastavíme multiplexer na **některý** kanál
    ADC2_Select_Channel(ADC2_CHANNEL_14);
    //rozběhnu ADC
    ADC2_Cmd(ENABLE); //příkaz pro zapnutí periferie
    //počkám až se rozběhne +-(7us)
    ADC2_Startup_Wait();
}


int main(void)
{
  
    uint32_t time = 0;
    uint16_t vref, vtemp, temp;
    init();

    while (1) {
        if (milis() - time > 333 ) {

            time = milis();
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);
            ADC_get(CHANNEL_VTEMP);

            vtemp = ADC_get(CHANNEL_VTEMP) *5000L / 1023;       //L je pretypovani na long
            vref = (uint32_t)ADC_get(CHANNEL_VREF)*5000 / 1023;
            temp = (10*vtemp-4000+97)/195;                     // vše *10 abych byl v celých číslech
            printf("%u mV %u mV\n T = %u ˚C\n", vtemp, vref, temp);
        }  
        
    }
}
 

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
