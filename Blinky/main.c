//*****************************************************************************
//
//   main.c
//   Murtuza Quaizar    ---------------
//   Date: April 24th, 2015 -----------
//   Bare Metal Programming*** (IAR default starup code not included);
//
//   Development Board: TI Stellaris LauchPad LM4f120 Evaluation Kit
//   Microprocessor: LM4D120H5QR
//   Integrated Development Environment (IDE): IAR WorkBench 7.4
//
//   Blink LED from PORTF at Timer0 Precesison
//
//   There is No Copyright.  No rights reserved.
//
//   The following main source code, incorporates all the code (minus the
//   startup code), needed to blink an LED light from PORTF at an interval
//   set by the hardware Timer0, for the LM4F120H5QR Microcontroller.
//   An ARM Cortex M4F Mircoprocessor Architecture variant.
//
//   I want to emphasize, how important it is that no other external files
//   including header files were used, to make this happen.
//
//   There is alot of code already written, even in sample codes, that uses
//   other source code for drivers of PORT pins, and Timers.  This leads
//   a new developer of the architecture to wonder, "great it blinks the LED,
//   but HOW did it do it, and more importantly, HOW can i do it?"
//
//   Not only is the code provided, but instructions copied from the
//   documentation of the user manual "tm4c123gh6pm.pdf" is provided as well
//   for guidance.  Definitions (ie #defines) were copied from the
//   lm4f120h5qr.h header file, and should be referenced on an as needed basis.
//
//   It is interesting to point out that the microcontroller reference manual
//   and its corresponding header file should always be a *match*.
//   At its best, the reference manual should behave as an APPENDIX to the
//   to the target pacific header file (in this case the lmf120h5qr).  And
//   at its best the header file should have its name definitions (#defines)
//   clear enough, and obvious enough to reference back to the manual.
//*****************************************************************************

#define SYSCTL_RCGCTIMER_R      (*((volatile unsigned long *)0x400FE604))
#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))



#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_RIS_R            (*((volatile unsigned long *)0x4003001C))
#define TIMER0_ICR_R            (*((volatile unsigned long *)0x40030024))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))


void main()
{
  //10.3 GPIO (PORTF)
  //---------------------------------------------------------------------//
  //1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
  SYSCTL_RCGCGPIO_R = 0xFF;
  //2) Set the direction of the GPIO port pins by programming the GPIODIR
  GPIO_PORTF_DIR_R |= (1 <<1U);
  GPIO_PORTF_DIR_R |= (1 <<2U);
  GPIO_PORTF_DIR_R |= (1 <<3U);
  //3) Configure the GPIOAFSEL register to program each bit as GPIO;

  //4) Set the drive strength for each pin in GPIODR2R, GPIODR4R, and GPIODR8R

  //5)Program each pad in the port to have eitehr pull up or pull down
   // through GPIOPUR, GPIOPDR, GPIODR register

  //6) Enable GPIO pin as digital IO in GPIODEN
  GPIO_PORTF_DEN_R |= (1 <<1U);
  GPIO_PORTF_DEN_R |= (1 <<2U);
  GPIO_PORTF_DEN_R |= (1 <<3U);

  //7) Program the GPIOIS, GPIOIBE, GPIOEV, and GPIOIM

  //turn on LED;
  GPIO_PORTF_DATA_R |= (1 <<1U);
  //---------------------------------------------------------------------//
  //11.4 TIMERS (Timer0)
  SYSCTL_RCGCTIMER_R = 0x01;
  //11.4.1 One/Shot Periodic Timer Mode
  //1) Clear the TnEN bit in the GPTMCTL register
  TIMER0_CTL_R &=  ~(0x01);
  //2) Write the GPTMCFG Configuration Register with a value of 0x0000.0000.
  TIMER0_CFG_R  = 0x00000000;
  /*3 Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
      a. Write a value of 0x1 for One-Shot mode.
      b. Write a value of 0x2 for Periodic mode
  */
  TIMER0_TAMR_R = 0x02;
 // TIMER0_TBMR_R = 0x02;
  //4 -----skip----
  //5 Load the start value into the GPTM Timer n Interval GPTMTnILR)
  TIMER0_TAILR_R = 0x00FFFFFF;
 // TIMER0_TBILR_R = 0xFFFFFFFF;
  //6 For interrupts, set bits in the GPTM Interrupt Mask Register (GPTMIMR)

  //7 Set the TnEN bit in GPTMCTL to enable the timer and start counting.
  TIMER0_CTL_R |=  (0x01);

  //8 Poll the GPTMRIS register or wait for the interrupt
  //  Clear status flags by writing a 1 to the Register GPTMICR

  while(1)
  {
    if((TIMER0_RIS_R & 0x01) == 0x01)
    {
      TIMER0_ICR_R |= 0x01; //clear Timer A Time Out.
      GPIO_PORTF_DATA_R ^= (1 <<1U); //toggle led

    }
  }
}
