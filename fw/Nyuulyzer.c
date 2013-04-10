/*
 ============================================================================
 Name        : Nyuulyzer.c
 Author      : Korneliusz Osmenda <kormeliuszo@gmail.com>
 Version     :
 Copyright   : GPLv3
 Description : Main file for AVR (Based on RelayBoardDemo)
 ============================================================================
 */



#include "Nyuulyzer.h"

uint8_t startflag;

void LogcoreD(void);


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */

int main(void)
{
  SetupHardware();

  startflag=0;

  sei();
  for (;;)
    {
      USB_USBTask();
      if (startflag)
        {
          PORTB=0x50; //Set LED ON
          Endpoint_SelectEndpoint(NYUULYZER_EPNUM);
          startflag=0;
		  cli();

		  //wait some time for PC side
          UDINT=~(1<<SOFI);
          while(!(UDINT & (1<<SOFI)));
		  UDINT=~(1<<SOFI);
          _delay_us(200);
		  while(!(UDINT & (1<<SOFI)));
          UDINT=~(1<<SOFI);
          _delay_us(200);
          while(!(UDINT & (1<<SOFI)));
		  _delay_us(200);

          LogcoreD(); //Go to asm

          PORTB=0x00; //Set LED OFF
        }
    }
}

/** Configures the board hardware and chip peripherals for the project's functionality. */
void SetupHardware(void)
{
  /* Disable watchdog if enabled by bootloader/fuses */
  MCUSR &= ~(1 << WDRF);
  wdt_disable();
  DDRB=0x70;
  PORTB=0x00;
  /* Disable clock division */
  clock_prescale_set(clock_div_1);

  /* Hardware Initialization */
  USB_Init();

}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
  switch (USB_ControlRequest.bRequest)
    {
    case REQ_SetInterface:
      /* Set Interface is not handled by the library, as its function is application-specific */
      if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_STANDARD | REQREC_INTERFACE))
        {
          Endpoint_ClearSETUP();
          Endpoint_ClearStatusStage();

          startflag = ((USB_ControlRequest.wValue) != 0);
        }

      break;
    }
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
  bool ConfigSuccess = true;

  ConfigSuccess &= Endpoint_ConfigureEndpoint(NYUULYZER_EPNUM,ENDPOINT_DIR_IN,NYUULYZER_EPSIZE,2);
}
