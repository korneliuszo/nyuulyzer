/*
 ============================================================================
 Name        : Nyuulyzer.h
 Author      : Korneliusz Osmenda <kormeliuszo@gmail.com>
 Version     :
 Copyright   : GPLv3
Description : Header file for Nyuulyzer.c (Based on RelayBoardDemo)
 ============================================================================
 */


#ifndef _NYUULYZER_H_
#define _NYUULYZER_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>

		#include "Descriptors.h"

		#include <LUFA/Version.h>
		#include <LUFA/Drivers/USB/USB.h>

	/* Macros: */

	/* Function Prototypes: */
		void SetupHardware(void);

		void EVENT_USB_Device_ControlRequest(void);

#endif

