/*
 ============================================================================
 Name        : Nyuulyzer.c
 Author      : Korneliusz Osmenda <kormeliuszo@gmail.com>
 Version     :
 Copyright   : GPLv3
 Description : core for sending data to PC
 ============================================================================
 */

#include <avr/io.h>
.macro nops x
.rept \x
nop
.endr
.endm

.section	.text.LogcoreD,"ax",@progbits
.global LogcoreD

#define input() in R19, _SFR_IO_ADDR(PIND)
//#define input() inc R19

LogcoreD:
	cli
	ldi ZL,lo8(Ubuff)
    ldi ZH,hi8(Ubuff)  ;write
    ldi XL,lo8(Ubuff)
    ldi XH,hi8(Ubuff)  ;read
    ldi R19, 0x23
	ldi R23, 0
	ldi R24, 255
	rjmp start

begin4:												;1l
	nops 2											;2
begin2:												;1l
	nops 2											;2
begin:												;1l
	//UEINTX &=~((1<<TXINI)|(1<<FIFOCON));
	lds R21, _SFR_MEM_ADDR(UEINTX)					;2
	andi R21, ~((1<<TXINI)|(1<<FIFOCON))			;1
	sts _SFR_MEM_ADDR(UEINTX), R21					;2

start:
	input()											;1p
	st -Z,R19										;2
	ori ZH,0x04										;1
	nops 2											;2
	//while(!(1<<TXINI & UEINTX))
	1:												;1l
	lds R21, _SFR_MEM_ADDR(UEINTX)					;2
	andi R21, (1<<TXINI)							;1
	brne 2f											;1
	//if (Z==X) return
	cp ZL, XL										;1
	cpc ZH, XH										;1
	brne 3f											;1
end:
	clr R1
	reti
	3:												;1
	nops 2											;2
	input()											;1p
	st -Z,R19										;2
	ori ZH,0x04										;1
	rjmp 1b											;1

	2:												;1
	//if ((X-Z)&&0x0x3ff<150?)
	movw R20, XL									;1
	sub R20, ZL 									;1
	sbc R21, ZH										;1
	breq end										;1
	andi R21, 0x03									;1
	input()											;1p
	st -Z,R19										;2
	ori ZH,0x04										;1
	subi R20, 150									;1
	sbci R21, 0										;1
	brsh 1f											;1
	//rjmp 1f
	//wysyłamy "pusty" pakiet
		ldi R22,64									;1
		3:											;1l
		sts _SFR_MEM_ADDR(UEDATX), R23				;2
		sts _SFR_MEM_ADDR(UEDATX), R20				;2
		sts _SFR_MEM_ADDR(UEDATX), R21				;2
		sts _SFR_MEM_ADDR(UEDATX), R22				;2
		input()										;1p
		st -Z,R19									;2
		ori ZH,0x04									;1
		//for (i=64;i<0;i++)
		dec R22										;1
		breq begin4									;1
		rjmp 3b										;1
	//else
	1:												;1
	//wysyłamy DANE
	sts _SFR_MEM_ADDR(UEDATX), R24					;2  0
	sts _SFR_MEM_ADDR(UEDATX), R20					;2  1
	sts _SFR_MEM_ADDR(UEDATX), R21					;2  2

.rept 36
	ld R0, -X										;2
	input()											;1p
	st -Z,R19										;2
	ori ZH,0x04										;1
	ori XH,0x04										;1
	sts _SFR_MEM_ADDR(UEDATX), R0					;2 /3 10 17 24 31 38 45 52 59 66 73 80 87 94 101 108 115 122 129 136 143 150 157 164 171 178 185 192 199 206 213 220 227 234 241 248
	ld R0, -X										;2
	ori XH,0x04										;1
	sts _SFR_MEM_ADDR(UEDATX), R0					;2 /4 11 18 25 32 39 46 53 60 67 74 81 88 95 102 109 116 123 130 137 144 151 158 165 172 179 186 193 200 207 214 221 228 235 242 249
	ld R0, -X										;2
	ori XH,0x04										;1
	nop												;1
	input()											;1p
	st -Z,R19										;2
	ori ZH,0x04										;1
	sts _SFR_MEM_ADDR(UEDATX), R0					;2 /5 12 19 26 33 40 47 54 61 68 75 82 89 96 103 110 117 124 131 138 145 152 159 166 173 180 187 194 201 208 215 222 229 236 243 250
	ld R0, -X										;2
	ori XH,0x04										;1
	sts _SFR_MEM_ADDR(UEDATX), R0					;2 /6 13 20 27 34 41 48 55 62 69 76 83 90 97 104 111 118 125 132 139 146 153 160 167 174 181 188 195 202 209 216 223 230 237 244 251
	ld R0, -X										;2
	ori XH,0x04										;1
	sts _SFR_MEM_ADDR(UEDATX), R0					;2 /7 14 21 28 35 42 49 56 63 70 77 84 91 98 105 112 119 126 133 140 147 154 161 168 175 182 189 196 203 210 217 224 231 238 245 252
	input()											;1p
	st -Z,R19										;2
	ori ZH,0x04										;1
	ld R0, -X										;2
	ori XH,0x04										;1
	sts _SFR_MEM_ADDR(UEDATX), R0					;2 /8 15 22 29 36 43 50 57 64 71 78 85 92 99 106 113 120 127 134 141 148 155 162 169 176 183 190 197 204 211 218 225 232 239 246 253
	ld R0, -X										;2
	ori XH,0x04										;1
	sts _SFR_MEM_ADDR(UEDATX), R0					;2 /9 16 23 30 37 44 51 58 65 72 79 86 93 100 107 114 121 128 135 142 149 156 163 170 177 184 191 198 205 212 219 226 233 240 247 254
.endr
	ld R0, -X										;2
	input()											;1p
	st -Z,R19										;2
	ori ZH,0x04										;1
	ori XH,0x04										;1
	sts _SFR_MEM_ADDR(UEDATX), R0					;2 /255
	rjmp begin2 									;1

	ret

.section .bss.Ubuff
.org 0x2ff
.space 1024
Ubuff:
;.acomm Ubuff,256*4


