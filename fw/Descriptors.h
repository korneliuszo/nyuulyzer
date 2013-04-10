/*
 ============================================================================
 Name        : Descriptors.c
 Author      : Korneliusz Osmenda <kormeliuszo@gmail.com>
 Version     :
 Copyright   : GPLv3
 Description : header file for Descriptors.c (Based on RelayBoardDemo)
 ============================================================================
 */

#ifndef _DESCRIPTORS_H_
#define _DESCRIPTORS_H_

	/* Includes: */
		#include <avr/pgmspace.h>
		#include <LUFA/Drivers/USB/USB.h>

	/* Type Defines: */
		/** Type define for the device configuration descriptor structure. This must be defined in the
		 *  application code, as the configuration descriptor contains several sub-descriptors which
		 *  vary between devices, and which describe the device's usage to the host.
		 */
		typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;

			USB_Descriptor_Interface_t            LoggerInterfaceAlt0;
			USB_Descriptor_Interface_t            LoggerInterfaceAlt1;
			USB_Descriptor_Endpoint_t             LoggerEndpoint;
		} USB_Descriptor_Configuration_t;

	/* Macros: */
		/** Endpoint number of the Logger IN endpoint. */
		#define NYUULYZER_EPNUM               (1 | ENDPOINT_DIR_IN)

		/** Size in bytes of the IN endpoint. */
		#define NYUULYZER_EPSIZE              256
		#define EP_TYPE_NYUULYZER             EP_TYPE_BULK

	/* Function Prototypes: */
		uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
		                                    const uint8_t wIndex,
		                                    const void** const DescriptorAddress)
		                                    ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);
#endif

