/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "xil_printf.h"
#include <stdio.h>
#include <stdlib.h>

#define LOOPS	50		/* Use this for faster machines */

/* Compiler dependent options */
#undef	NOENUM			/* Define if compiler has no enum's */
#undef	NOSTRUCTASSIGN		/* Define if compiler can't assign structures */

#ifdef GOOF
char Version[] = "1.0";
#else
char Version[] = "1.1";
#endif

#define	structassign(d, s)	d = s
typedef enum {
	Ident1, Ident2, Ident3, Ident4, Ident5
} Enumeration;

typedef int OneToThirty;
typedef int OneToFifty;
typedef char CapitalLetter;
typedef char String30[31];
typedef int Array1Dim[51];
typedef int Array2Dim[51][51];

struct Record {
	struct Record *PtrComp;
	Enumeration Discr;
	Enumeration EnumComp;
	OneToFifty IntComp;
	char* StringComp;
};

typedef struct Record RecordType;
typedef RecordType * RecordPtr;
typedef int boolean;

#define	TRUE		1
#define	FALSE		0

#ifndef REG
#define	REG
#endif

int Proc0();
int Proc1(REG RecordPtr PtrParIn);
int Proc2(OneToFifty *IntParIO);
int Proc3(RecordPtr * PtrParOut);
int Proc4(void);
int Proc5(void);
int Proc6(REG Enumeration EnumParIn, REG Enumeration *EnumParOut);
int Proc7(OneToFifty IntParI1, OneToFifty IntParI2, OneToFifty *IntParOut);
int Proc8(Array1Dim Array1Par, Array2Dim Array2Par, OneToFifty IntParI1,
		OneToFifty IntParI2);
int proc9(void);
Enumeration Func1(CapitalLetter CharPar1, CapitalLetter CharPar2);
boolean Func2(String30 StrParI1, String30 StrParI2);
boolean Func3(REG Enumeration EnumParIn);
int my_strcmp(const char * pStr1, const char * pStr2);

int test_main() {
	while (1) {
		Proc0();
		proc9();
		////printf("\n\n\n-----------------task2 finish----------------\n\n\n");
	}
}

/*
 * Package 1
 */
int IntGlob;
boolean BoolGlob;
char Char1Glob;
char Char2Glob;
Array1Dim Array1Glob;
Array2Dim Array2Glob;
RecordPtr PtrGlb;
RecordPtr PtrGlbNext;

int Proc0() {
	OneToFifty IntLoc1;
	REG OneToFifty IntLoc2;
	OneToFifty IntLoc3;

	REG char CharIndex;
	Enumeration EnumLoc;
	char* String1Loc;
	char* String2Loc;

	register unsigned int i;
	PtrGlbNext = (RecordPtr) malloc(sizeof(RecordType));
	PtrGlb = (RecordPtr) malloc(sizeof(RecordType));
	PtrGlb->PtrComp = PtrGlbNext;
	PtrGlb->Discr = Ident1;
	PtrGlb->EnumComp = Ident3;
	PtrGlb->IntComp = 40;
	PtrGlb->StringComp = "DHRYSTONE PROGRAM, SOME STRING";
#ifndef	GOOF
	String1Loc = "DHRYSTONE PROGRAM, 1'ST STRING"; /*GOOF*/
#endif
	Array2Glob[8][7] = 10; /* Was missing in published program */

	/*****************
	 -- Start Timer --
	 *****************/
	for (i = 0; i < LOOPS; ++i) {

		Proc5();
		Proc4();
		IntLoc1 = 2;
		IntLoc2 = 3;
		String2Loc = "DHRYSTONE PROGRAM, 2'ND STRING";
		EnumLoc = Ident2;
		BoolGlob = !Func2(String1Loc, String2Loc);
		while (IntLoc1 < IntLoc2) {
			IntLoc3 = 5 * IntLoc1 - IntLoc2;
			Proc7(IntLoc1, IntLoc2, &IntLoc3);
			++IntLoc1;
		}
		Proc8(Array1Glob, Array2Glob, IntLoc1, IntLoc3);
		Proc1(PtrGlb);
		for (CharIndex = 'A'; CharIndex <= Char2Glob; ++CharIndex)
			if (EnumLoc == Func1(CharIndex, 'C'))
				Proc6(Ident1, &EnumLoc);
		IntLoc3 = IntLoc2 * IntLoc1;
		IntLoc2 = IntLoc3 / IntLoc1;
		IntLoc2 = 7 * (IntLoc3 - IntLoc2) - IntLoc1;
		Proc2(&IntLoc1);
	}

	return 0;
}

int Proc1(PtrParIn)
	REG RecordPtr PtrParIn; {
#define	NextRecord	(*(PtrParIn->PtrComp))

	structassign(NextRecord, *PtrGlb);
	PtrParIn->IntComp = 5;
	NextRecord.IntComp = PtrParIn->IntComp;
	NextRecord.PtrComp = PtrParIn->PtrComp;
	Proc3(NextRecord.PtrComp);
	if (NextRecord.Discr == Ident1) {
		NextRecord.IntComp = 6;
		Proc6(PtrParIn->EnumComp, &NextRecord.EnumComp);
		NextRecord.PtrComp = PtrGlb->PtrComp;
		Proc7(NextRecord.IntComp, 10, &NextRecord.IntComp);
	} else
		structassign(*PtrParIn, NextRecord);

#undef	NextRecord
	////printf("task2 : Proc0\n");
	return 0;
}

int Proc2(IntParIO)
	OneToFifty *IntParIO; {
	REG OneToFifty IntLoc;
	REG Enumeration EnumLoc;

	IntLoc = *IntParIO + 10;
	for (;;) {
		if (Char1Glob == 'A') {
			--IntLoc;
			*IntParIO = IntLoc - IntGlob;
			EnumLoc = Ident1;
		}
		if (EnumLoc == Ident1)
			break;
	}
	////printf("task2 : Proc2\n");
	return 0;
}

int Proc3(PtrParOut)
	RecordPtr *PtrParOut; {
	if (PtrGlb != NULL )
		*PtrParOut = PtrGlb->PtrComp;
	else
		IntGlob = 100;
	Proc7(10, IntGlob, &PtrGlb->IntComp);
	////printf("task2 : Proc3\n");
	return 0;
}

int Proc4() {
	REG boolean BoolLoc;

	BoolLoc = Char1Glob == 'A';
	BoolLoc |= BoolGlob;
	Char2Glob = 'B';
	//printf("task2 : Proc4\n");
	return 0;
}

int Proc5() {
	Char1Glob = 'A';
	BoolGlob = FALSE;
	//printf("task2 : Proc5\n");
	return 0;
}

int Proc6(EnumParIn, EnumParOut)
	REG Enumeration EnumParIn; REG Enumeration *EnumParOut; {
	*EnumParOut = EnumParIn;
	if (!Func3(EnumParIn))
		*EnumParOut = Ident4;
	switch (EnumParIn) {
	case Ident1:
		*EnumParOut = Ident1;
		break;
	case Ident2:
		if (IntGlob > 100)
			*EnumParOut = Ident1;
		else
			*EnumParOut = Ident4;
		break;
	case Ident3:
		*EnumParOut = Ident2;
		break;
	case Ident4:
		break;
	case Ident5:
		*EnumParOut = Ident3;
	}
	//printf("task2 : Proc6\n");
	return 0;
}

int Proc7(IntParI1, IntParI2, IntParOut)
	OneToFifty IntParI1;OneToFifty IntParI2;OneToFifty *IntParOut; {
	REG OneToFifty IntLoc;

	IntLoc = IntParI1 + 2;
	*IntParOut = IntParI2 + IntLoc;
	//printf("task2 : Proc7\n");
	return 0;
}

int Proc8(Array1Par, Array2Par, IntParI1, IntParI2)
	Array1Dim Array1Par;Array2Dim Array2Par;OneToFifty IntParI1;OneToFifty IntParI2; {
	REG OneToFifty IntLoc;
	REG OneToFifty IntIndex;

	IntLoc = IntParI1 + 5;
	Array1Par[IntLoc] = IntParI2;
	Array1Par[IntLoc + 1] = Array1Par[IntLoc];
	Array1Par[IntLoc + 30] = IntLoc;
	for (IntIndex = IntLoc; IntIndex <= (IntLoc + 1); ++IntIndex)
		Array2Par[IntLoc][IntIndex] = IntLoc;
	++Array2Par[IntLoc][IntLoc - 1];
	Array2Par[IntLoc + 20][IntLoc] = Array1Par[IntLoc];
	IntGlob = 5;
	//printf("task2 : Proc8\n");
	return 0;
}

Enumeration Func1(CapitalLetter CharPar1, CapitalLetter CharPar2) {
	REG CapitalLetter CharLoc1;
	REG CapitalLetter CharLoc2;

	CharLoc1 = CharPar1;
	CharLoc2 = CharLoc1;
	if (CharLoc2 != CharPar2)
		return (Ident1);
	else
		return (Ident2);
}

boolean Func2(StrParI1, StrParI2)
	String30 StrParI1;String30 StrParI2; {
	REG OneToThirty IntLoc;
	REG CapitalLetter CharLoc;

	IntLoc = 1;
	while (IntLoc <= 1)
		if (Func1(StrParI1[IntLoc], StrParI2[IntLoc + 1]) == Ident1) {
			CharLoc = 'A';
			++IntLoc;
		}
	if (CharLoc >= 'W' && CharLoc <= 'Z')
		IntLoc = 7;
	if (CharLoc == 'X')
		return (TRUE);
	else {
		if (my_strcmp(StrParI1, StrParI2) > 0) {
			IntLoc += 7;
			return (TRUE);
		} else
			return (FALSE);
	}
}

boolean Func3(EnumParIn)
	REG Enumeration EnumParIn; {
	REG Enumeration EnumLoc;

	EnumLoc = EnumParIn;
	if (EnumLoc == Ident3)
		return (TRUE);
	return (FALSE);
}

int my_strcmp(const char * pStr1, const char * pStr2) {
	if (pStr1 == NULL || pStr2 == NULL )
		return -1;

	for (; (*pStr1) != '\0' || (*pStr2) != '\0'; pStr1++, pStr2++) {
		if ((*pStr1) > (*pStr2))
			return 1;
		if ((*pStr1) < (*pStr2))
			return -1;
	}

	return 0;
}
/* ---------- */

int proc9()
{
	unsigned int *arr;
	unsigned int value;
	int iternum, chunknum, linenum;

	arr = (unsigned int *) 0x0;
	iternum = 8000;

	while(iternum){

		for(chunknum=0; chunknum<4; chunknum++){
			for(linenum=0; linenum<256; linenum++){
				value = *(arr + (chunknum*2048) + (linenum*8));
			}
	//	//printf("%d\n", chunknum);
		}

		iternum --;
	}

	return 0;
}


void pmu_setting(void)
{
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 2\t\n" :: "r"(0x8000003f)); // update PMCNTENCLR to disable PMCCNTR and all 6 configurable PMNs
	// A-2.select events to count
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x01)); // update PMSELR to select PMN (0)
	__asm__ volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0x10)); // branch miss
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x02)); // update PMSELR to select PMN (1)
	__asm__ volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0x4)); // data access
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x03)); // update PMSELR to select PMN (2)
	__asm__ volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0x11)); // cycle count
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x04)); // update PMSELR to select PMN (2)
	__asm__ volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0x3)); // data cache miss
	// A-3.set cycle counter tick rate
	// A-4.reset performance counters
	__asm__ volatile ("MCR p15, 0, %0, c9, c13, 0\t\n" :: "r"(0x00000000)); // reset PMCCNTR
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x01));		// update PMSELR to select PMN (0)
	__asm__ volatile ("MCR p15, 0, %0, c9, c13, 2\t\n" :: "r"(0x00000000)); // reset PMN (0)
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x02));		// update PMSELR to select PMN (1)
	__asm__ volatile ("MCR p15, 0, %0, c9, c13, 2\t\n" :: "r"(0x00000000)); // reset PMN (1)
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x03));		// update PMSELR to select PMN (2)
	__asm__ volatile ("MCR p15, 0, %0, c9, c13, 2\t\n" :: "r"(0x00000000)); // reset PMN (2)
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x04));		// update PMSELR to select PMN (2)
	__asm__ volatile ("MCR p15, 0, %0, c9, c13, 2\t\n" :: "r"(0x00000000)); // reset PMN (2)
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 3\t\n" :: "r"(0x8000003f));	// update PMOVSR to clear overflow status of PMCCNTR and all 6 configurable PMNs
	// A-5.enable performance counters
	__asm__ volatile ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x8000003f)); // update PMCNTENSET to enable PMCCNTR and PMN 0, 1, 2
}
