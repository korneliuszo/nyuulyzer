/*
 ============================================================================
 Name        : loggerapp.c
 Author      : Korneliusz Osmenda <kormeliuszo@gmail.com>
 Version     :
 Copyright   : GPLv3
 Description : Application to comunicate with firmware
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <string.h>

FILE *dbgfd;
int num;

libusb_device_handle* finddevice(libusb_context* ctx) {

	libusb_device_handle* handle = 0;
	// discover devices
	libusb_device **list;
	ssize_t cnt = libusb_get_device_list(ctx, &list);
	ssize_t i = 0;

	for (i = 0; i < cnt; i++) {
		libusb_device *device = list[i];
		struct libusb_device_descriptor desc;
		libusb_get_device_descriptor(device, &desc);
		fprintf(dbgfd, "usb device found ");
		fprintf(dbgfd, "VID: 0x%04x PID: 0x%04x\n", desc.idVendor,
				desc.idProduct);
		if (desc.idVendor == 0x03eb && desc.idProduct == 0x2040) {
			fprintf(dbgfd, "** Test  VID/PID Found **\n");
			libusb_open(device, &handle);
			if (!handle)
				continue;
			unsigned char buff[30], buff2[20];
			libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, buff,
					sizeof(buff));
			fprintf(dbgfd, "Manufacturer: %s\n", buff);
			libusb_get_string_descriptor_ascii(handle, desc.iProduct, buff2,
					sizeof(buff2));
			fprintf(dbgfd, "Product: %s\n", buff2);
			if (!strcmp(buff, "Korneliusz Osmenda") && !strcmp(buff2, "Nyuulyzer")) {
				fprintf(dbgfd, "IS Nyuulyzer\n");
				break;
			}
			libusb_close(handle);
			handle = 0;
		}

	}

	libusb_free_device_list(list, 1);

	return handle;
}
static void capture_callback(struct libusb_transfer *transfer) {

	if (transfer->actual_length == 0) {
			fprintf(dbgfd,"packet with zero len\n");
		num--;
		return;
	}
	int j;
	if (transfer->actual_length==1024)
	{
		if (transfer->buffer[0]==0xff)
			fwrite(&(transfer->buffer[3]), sizeof(char), 256-3, stdout);
		if (transfer->buffer[256]==0xff)
			fwrite(&(transfer->buffer[256+3]), sizeof(char), 256-3, stdout);
		if (transfer->buffer[512]==0xff)
			fwrite(&(transfer->buffer[512+3]), sizeof(char), 256-3, stdout);
		if (transfer->buffer[768]==0xff)
			fwrite(&(transfer->buffer[768+3]), sizeof(char), 256-3, stdout);
		fflush(stdout);
	}
	if (transfer->status == LIBUSB_TRANSFER_COMPLETED)
		libusb_submit_transfer(transfer);
	else
		num--;
}

int main(void) {

	libusb_device_handle* dev;
	libusb_context* ctx;

	dbgfd = stderr;

	libusb_init(&ctx);
	libusb_set_debug(ctx, 2);

	if (!(dev = finddevice(ctx)))
		exit(-1);

	int ret;
	ret=libusb_set_configuration(dev, 1);
	ret=libusb_claim_interface(dev, 0);
	ret=libusb_set_interface_alt_setting(dev, 0, 1);

#define TRANS_NUM 50
	struct libusb_transfer* trans[TRANS_NUM];
	int i;
	for (i = 0; i < TRANS_NUM; i++) {
		trans[i] = libusb_alloc_transfer(0);
		libusb_fill_bulk_transfer(trans[i], dev, 0x81, malloc(1024), 1024,
				capture_callback, 0, 500);

	}
	char a[] = { 0, 1 };

	for (i = 0; i < TRANS_NUM; i++)
		libusb_submit_transfer(trans[i]);

	num = TRANS_NUM;

	while (num)
		libusb_handle_events(ctx);

	for (i = 0; i < TRANS_NUM; i++) {
		libusb_free_transfer(trans[i]);
		trans[i] = 0;
	}

	libusb_close(dev);
	libusb_exit(ctx);
	return EXIT_SUCCESS;
}
