#ifndef _RFIDCODES
#define _RFIDCODES

#include "testApp.h"

// 4 familias:

// L - game art				- L1 .. L6
// P - arte publico			- P1 .. P6
// G - arte y genero		- G1 .. G6
// N - arte y naturaleza	- N1 .. N6

// ++

// 1 joker					- J0


string rfidMarks[RFID_CARDS_NUM]	= {"L1","L2","L3","L4","L5","L6","P1","P2","P3","P4","P5","P6",
									   "G1","G2","G3","G4","G5","G6","N1","N2","N3","N4","N5","N6",
									   "J0"};

int rfidCodes[RFID_CARDS_NUM]		= {809580099,808993848,808993586,808994369,808991045,808993857,
									   808990771,808994360,808993849,808993604,808994098,808993090,
									   808994105,808994373,809055027,809055026,808993588,809055029,
									   808990009,808993859,808993860,808993847,809054256,808994097,
									   808993846};


//--------------------------------------------------------------
bool testApp::codeToMark(string *s,int code){
	for(int i=0;i<RFID_CARDS_NUM;i++){
		if(rfidCodes[i] == code){
			*s = rfidMarks[i];
			return true;
			break;
		}
	}
	return false;
}

#endif
