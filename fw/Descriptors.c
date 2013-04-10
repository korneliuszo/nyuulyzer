/*
 ============================================================================
 Name        : Descriptors.c
 Author      : Korneliusz Osmenda <kormeliuszo@gmail.com>
 Version     :
 Copyright   : GPLv3
 Description : Descriptors declaration (Based on RelayBoardDemo)
 ============================================================================
 */

#include "Descriptors.h"

/** Device descriptor structure. This descriptor, located in FLASH memory, describes the overall
 *  device characteristics, including the supported USB version, control endpoint size and the
 *  number of device configurations. The descriptor is read out by the USB host when the enumeration
 *  process begins.
 */
const USB_Descriptor_Device_t PROGMEM Logger_DeviceDescriptor =
{
	.Header                 = {.Size = sizeof(USB_Descriptor_Device_t), .Type = DTYPE_Device},

	.USBSpecification       = VERSION_BCD(02.00),
	.Class                  = USB_CSCP_VendorSpecificClass,
	.SubClass               = USB_CSCP_NoDeviceSubclass,
	.Protocol               = USB_CSCP_NoDeviceProtocol,

	.Endpoint0Size          = FIXED_CONTROL_ENDPOINT_SIZE,

	.VendorID               = 0x03EB,
	.ProductID              = 0x2040,
	.ReleaseNumber          = VERSION_BCD(12.40),

	.ManufacturerStrIndex   = 0x01,
	.ProductStrIndex        = 0x02,
	.SerialNumStrIndex      = 0x03,

	.NumberOfConfigurations = FIXED_NUM_CONFIGURATIONS
};

/** Configuration descriptor structure. This descriptor, located in FLASH memory, describes the usage
 *  of the device in one of its supported configurations, including information about any device interfaces
 *  and endpoints. The descriptor is read out by the USB host during the enumeration process when selecting
 *  a configuration so that the host may correctly communicate with the USB device.
 */
const USB_Descriptor_Configuration_t PROGMEM Logger_ConfigurationDescriptor =
{
	.Config =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Configuration_Header_t), .Type = DTYPE_Configuration},

			.TotalConfigurationSize = sizeof(USB_Descriptor_Configuration_t),
			.TotalInterfaces        = 1,

			.ConfigurationNumber    = 1,
			.ConfigurationStrIndex  = NO_DESCRIPTOR,

			.ConfigAttributes       = USB_CONFIG_ATTR_RESERVED,

			.MaxPowerConsumption    = USB_CONFIG_POWER_MA(500)
		},


	.LoggerInterfaceAlt0 =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = 0,
			.AlternateSetting       = 0,

			.TotalEndpoints         = 0,

			.Class                  = USB_CSCP_VendorSpecificClass,
			.SubClass               = 0x00,
			.Protocol               = 0x00,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},
	.LoggerInterfaceAlt1 =
		{
			.Header                 = {.Size = sizeof(USB_Descriptor_Interface_t), .Type = DTYPE_Interface},

			.InterfaceNumber        = 0,
			.AlternateSetting       = 1,

			.TotalEndpoints         = 1,

			.Class                  = USB_CSCP_VendorSpecificClass,
			.SubClass               = 0x00,
			.Protocol               = 0x00,

			.InterfaceStrIndex      = NO_DESCRIPTOR
		},
    .LoggerEndpoint =
        {
			.Header                 = {.Size = sizeof(USB_Descriptor_Endpoint_t), .Type = DTYPE_Endpoint},

			.EndpointAddress        = (NYUULYZER_EPNUM),
			.Attributes             = (EP_TYPE_NYUULYZER),
			.EndpointSize           = NYUULYZER_EPSIZE,
			.PollingIntervalMS      = 1
        },
};

/** Language descriptor structure. This descriptor, located in FLASH memory, is returned when the host requests
 *  the string descriptor with index 0 (the first index). It is actually an array of 16-bit integers, which indicate
 *  via the language ID table available at USB.org what languages the device supports for its string descriptors.
 */
const USB_Descriptor_String_t PROGMEM Logger_LanguageString =
{
	.Header                 = {.Size = USB_STRING_LEN(1), .Type = DTYPE_String},

	.UnicodeString          = {LANGUAGE_ID_ENG}
};

/** Manufacturer descriptor string. This is a Unicode string containing the manufacturer's details in human readable
 *  form, and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t PROGMEM Logger_ManufacturerString =
{
	.Header                 = {.Size = USB_STRING_LEN(18), .Type = DTYPE_String},

	.UnicodeString          = L"Korneliusz Osmenda"
};

/** Product descriptor string. This is a Unicode string containing the product's details in human readable form,
 *  and is read out upon request by the host when the appropriate string ID is requested, listed in the Device
 *  Descriptor.
 */
const USB_Descriptor_String_t PROGMEM Logger_ProductString =
{
	.Header                 = {.Size = USB_STRING_LEN(9), .Type = DTYPE_String},

	.UnicodeString          = L"Nyuulyzer"
};

/** Serial number string. This is a Unicode string containing the device's unique serial number, expressed as a
 *  series of uppercase hexadecimal digits.
 */
const USB_Descriptor_String_t PROGMEM Logger_SerialString =
{
	.Header                 = {.Size = USB_STRING_LEN(5), .Type = DTYPE_String},

	.UnicodeString          = L"00001"
};

/** This function is called by the library when in device mode, and must be overridden (see library "USB Descriptors"
 *  documentation) by the application code so that the address and size of a requested descriptor can be given
 *  to the USB library. When the device receives a Get Descriptor request on the control endpoint, this function
 *  is called so that the descriptor details can be passed back and the appropriate descriptor sent back to the
 *  USB host.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void** const DescriptorAddress)
{
	const uint8_t  DescriptorType   = (wValue >> 8);
	const uint8_t  DescriptorNumber = (wValue & 0xFF);

	const void* Address = NULL;
	uint16_t    Size    = NO_DESCRIPTOR;

	switch (DescriptorType)
	{
		case DTYPE_Device:
			Address = &Logger_DeviceDescriptor;
			Size    = sizeof(USB_Descriptor_Device_t);
			break;
		case DTYPE_Configuration:
			Address = &Logger_ConfigurationDescriptor;
			Size    = sizeof(USB_Descriptor_Configuration_t);
			break;
		case DTYPE_String:
			switch (DescriptorNumber)
			{
				case 0x00:
					Address = &Logger_LanguageString;
					Size    = pgm_read_byte(&Logger_LanguageString.Header.Size);
					break;
				case 0x01:
					Address = &Logger_ManufacturerString;
					Size    = pgm_read_byte(&Logger_ManufacturerString.Header.Size);
					break;
				case 0x02:
					Address = &Logger_ProductString;
					Size    = pgm_read_byte(&Logger_ProductString.Header.Size);
					break;
				case 0x03:
					Address = &Logger_SerialString;
					Size    = pgm_read_byte(&Logger_SerialString.Header.Size);
					break;
			}

			break;
	}

	*DescriptorAddress = Address;
	return Size;
}

