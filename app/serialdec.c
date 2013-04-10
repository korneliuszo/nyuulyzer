/*
 ============================================================================
 Name        : serialdec.c
 Author      : Korneliusz Osmenda <kormeliuszo@gmail.com>
 Version     :
 Copyright   : GPLv3
 Description : Quick & Dirty UART decoder
 ============================================================================
 */

#include <stdlib.h>
#include <stdio.h>

enum state_e
{
	WAITING,
	READING,
	STOPING
};

void discardseek(struct _IO_FILE * fd, int val)
{
	while(--val && getc(fd)!=EOF);
}

int main(int argc, char** argv)
{
	if (argc<2)
	{
		printf("Usage: serialdec <pin> <speed>\n");
		return 1;
	}
	unsigned char port=(1<<atoi(argv[1]));
	int speed = 1000000UL/atoi(argv[2]);

	while (!feof(stdin))
	{
		char cur= fgetc(stdin);
		if (cur&port)
			break;
	}
	enum state_e state=WAITING;
	unsigned char bits;
	unsigned char read;

	while (!feof(stdin))
	{
		unsigned char cur= fgetc(stdin);
		switch (state)
		{
			case WAITING:
			if (!(cur&port))
			{
				state=READING;
				bits=8;
				read=0;
				discardseek(stdin,speed*3/2-2);
			}
			break;
			case READING:
			read=(read>>1);
			if (cur&port)
				read|=0x80;
			discardseek(stdin,speed-1);
			if(--bits)
				break;
			state=STOPING;
			//fprintf(stderr,"%i", read);
			putc(read,stdout);
			break;
			case STOPING:
			if (cur&port)
				state=WAITING;
			break;
		}
	}
	return 0;
}
