/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2020 nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/* Website: http://www.nuvoton.com                                                                         */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : June/21/2020                                                                                   */
/***********************************************************************************************************/

/************************************************************************************************************/
/*  File Function: MS51 Timer 2 capture function demo                                                       */
//***********************************************************************************************************/
#include "MS51_8K.H"

/************************************************************************************************************/
/*    Capture interrupt subroutine                                                                   */
/************************************************************************************************************/
void Capture_ISR (void) interrupt 12
{
    _push_(SFRS);
  
    clr_CAPCON0_CAPF0;                  /* clear capture0 interrupt flag  */
    clr_T2CON_TF2;

    _pop_(SFRS);
}
/************************************************************************************************************/
/*    Main function                                                                                         */
/************************************************************************************************************/
void main (void)
{
    ALL_GPIO_QUASI_MODE;
    P00_QUASI_MODE;
    P00 = 1;

    TIMER2_Compare_Capture_Mode;
    IC3_P00_CAP3_BOTHEDGE_CAPTURE;
      
    set_EIE_ECAP;                        //Enable Capture interrupt
    set_T2CON_TR2;                       //Triger Timer2
    ENABLE_GLOBAL_INTERRUPT;

    while(1);
}
