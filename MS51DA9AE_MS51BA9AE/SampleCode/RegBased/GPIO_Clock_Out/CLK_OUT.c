/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2020 nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : June/21/2020
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: MS51 Clock out from CLKO pin P0.1 demo.
//***********************************************************************************************************
#include "MS51_8K.H"



void main(void)
{
  
 /* Note
       MCU power on system clock is HIRC (16.0000MHz), if need change Fsys source pleaes copy hange system closk source demo
*/

    P01_PUSHPULL_MODE;                    /* P01 CLKO pin setting push pull mode to check output. */
    set_CKCON_CLOEN;                                  //System Clock Output Enable
/* Note */ 
/* Each time modify the HIRC must do re-power on process to confirm HIRC modified. */

#if 1
/* 16.6MHz HIRC clock out demo */
    MODIFY_HIRC(HIRC_166);
    CKDIV = 80;
#else 
/* 24MHz HIRC clock out demo */
    MODIFY_HIRC(HIRC_24);
    CKDIV = 120;
#endif
/*Please measure P1.1 (CLKO) pin for system clock output, it should as system clock;    */
    while(1);

}

