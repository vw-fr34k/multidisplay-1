/*
    Copyright 2009-10 Stephan Martin, Dominik Gummel

    This file is part of Multidisplay.

    Multidisplay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Multidisplay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Multidisplay.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wiring.h"

#include "MultidisplayDefines.h"
#include "LCDController.h"
#include "LCDScreen1.h"
#include "LCDScreen2.h"
#include "LCDScreen3.h"
#include "LCDScreen4.h"
#include "LCDScreen5.h"
#include "LCDScreen6.h"
#include "LCDScreen7.h"
#include "LCDScreen8.h"
#include "LCDScreen9.h"
#include "LCDScreen10.h"

const uint8_t	LCDController::ystart[] = { 0x80, 0xC0, 0x94, 0xD4 };

LCDController::LCDController() {
}

void  LCDController::myconstructor() {
	activeScreen = 2;
	brightness = 2;
	lcdp = new LCD4Bit(4);
	cbuf = (char*) malloc (sizeof(char) * LCD_BUFSIZE);

	lcdp->init();

    for ( int i = 0 ; i < 20 ; i++ )
            scopeInt[i]=0;

    myScreens[0] = new LCDScreen1();
    myScreens[1] = new LCDScreen2();
    myScreens[2] = new LCDScreen3();
    myScreens[3] = new LCDScreen4();
    myScreens[4] = new LCDScreen5();
    myScreens[5] = new LCDScreen6();
    myScreens[6] = new LCDScreen7();
    myScreens[7] = new LCDScreen8();
    myScreens[8] = new LCDScreen9();
    myScreens[9] = new LCDScreen10();
}

void LCDController::init() {
	myScreens[activeScreen]->init();
}

LCDScreen* LCDController::getLCDScreen ( uint8_t num ) {
	if ( num > SCREENCOUNT )
		return NULL;
	return myScreens[num];
}

void LCDController::draw() {
	myScreens[activeScreen]->draw();
}

//The LCD brightness, what could be cycled through
const uint8_t LCDController::LCDBrightnessD[]= {
   50,125,255        //Change this for other settings depending on what you like
 };

void LCDController::toggleBrightness() {
	if (brightness == 3)
		brightness = 0;
	else
		brightness++;
	setBrightness ( brightness );
}

void LCDController::setBrightness(uint8_t bright)  {
	brightness = constrain(bright,0,2);
    analogWrite(LCDBRIGHTPIN,pgm_read_word(&LCDBrightnessD[brightness]));
}

void LCDController::toggleScreen () {
	// 9 screens atm -> 0..8!
	if ( activeScreen == SCREENCOUNT-1 )
		activeScreen = 0;
	else
		activeScreen++;
	init();
}


//TODO convert into one 2D array!
//const prog_uchar LCDController::bn12[]={255,1,255,0, 1,255,254,0, 1,3,255,0, 1,3,255,0, 255,2,2,0, 255,3,1,0, 255,3,1,0, 1,1,255,0, 255,3,255,0, 255,3,255,0};
char LCDController::bn12[]={255,1,255,0, 1,255,254,0, 1,3,255,0, 1,3,255,0, 255,2,2,0, 255,3,1,0, 255,3,1,0, 1,1,255,0, 255,3,255,0, 255,3,255,0};
char LCDController::bn22[]={255,2,255,0, 2,255,2,0,   255,2,2,0, 2,2,255,0, 254,255,254,0, 2,2,255,0,   255,2,255,0, 254,255,254,0, 255,2,255,0, 254,254,255,0};
// Array index into parts of big numbers. Numbers consist of 12 custom characters in 4 lines
//              			0               1            2              3                4             5             6              7              8              9
char LCDController::bn14[]={1,2,3,0,       2,3,254,0,     1,2,3,0,       1,2,3,0,       2,254,254,0,   2,2,2,0,       1,2,3,0,       2,2,2,0,       1,2,3,0,       1,2,3,0};
char LCDController::bn24[]={255,254,255,0, 254,255,254,0, 1,2,255,0,     254,2,255,0,   255,2,2,0,     255,2,2,0,     255,2,3,0,     254,2,255,0,   255,2,255,0,   255,254,255,0};
char LCDController::bn34[]={255,254,255,0, 254,255,254,0, 255,254,254,0, 254,254,255,0, 254,255,254,0, 254,254,255,0, 255,254,255,0, 254,255,254,0, 255,254,255,0, 4,6,255,0};
char LCDController::bn44[]={4,6,5,0,       6,6,6,0,       4,6,6,0,       4,6,5,0,       254,6,254,0,   6,6,5,0,       4,6,5,0,       254,6,254,0,   4,6,5,0,       254,254,6,0};




//255 is the "Full Block" Code, and 254 is blank
const uint8_t LCDController::bigfont2data1[] = {31,31,31,0,0,0,0,0};    //Small Block on top
const uint8_t LCDController::bigfont2data2[] = {0,0,0,0,0,31,31,31};    //Small Block on bottom
const uint8_t LCDController::bigfont2data3[] = {31,31,31,0,0,0,31,31};  //Small Block on top and bottom
// const prog_uint8_t bigfont2data4[] = {28,30,31,31,31,31,30,28};  //Full Block roundet right
// const prog_uint8_t bigfont2data5[] = {7,15,31,31,31,31,15,7};  //Full Block roundet left
const uint8_t LCDController::bigfont2data6[] = {0,0,0,14,14,14,12,8};  //Dot, for Decimal.

//Bigfont Code
//
//BIG parts from http://opengauge.googlecode.com/svn/trunk/mpguino/mpguino.pde
void LCDController::cgramBigFont2() {
	lcdUploadUdef5x8_P (1,bigfont2data1);
	lcdUploadUdef5x8_P (2,bigfont2data2);
	lcdUploadUdef5x8_P (3,bigfont2data3);
	// LcdUploadUdef5x8_P (4,data4);
	// LcdUploadUdef5x8_P (5,data5);
	lcdUploadUdef5x8_P (6,bigfont2data6);
}
//-------------------------------------------------------------------------------------------------------

//255 is the "Full Block" Code, and 254 is blank
const uint8_t LCDController::bigfont4data1[] = {0,0,0,0,3,15,15,31};    //Small Block on bottom left
const uint8_t LCDController::bigfont4data2[] = {0,0,0,0,31,31,31,31};    //Small Block on bottom right
const uint8_t LCDController::bigfont4data3[] = {0,0,0,0,24,30,30,31};  //Small Block on bottom full
const uint8_t LCDController::bigfont4data4[] = {31,15,15,3,0,0,0,0};  //Small Block on top left
const uint8_t LCDController::bigfont4data5[] = {31,30,30,24,0,0,0,0};  //Small Block on top right
const uint8_t LCDController::bigfont4data6[] = {31,31,31,31,0,0,0,0};  //Small Block on top full
const uint8_t LCDController::bigfont4data7[] = {14,14,14,14,12,8,0,0};  //Dot, for Decimal.

void LCDController::cgramBigFont4() {

	lcdUploadUdef5x8_P (1,bigfont4data1);
	lcdUploadUdef5x8_P (2,bigfont4data2);
	lcdUploadUdef5x8_P (3,bigfont4data3);
	lcdUploadUdef5x8_P (4,bigfont4data4);
	lcdUploadUdef5x8_P (5,bigfont4data5);
	lcdUploadUdef5x8_P (6,bigfont4data6);
	lcdUploadUdef5x8_P (7,bigfont4data7);
}

/**
 * prints a 2 line number
 * @param digit
 * @param x_offset 1 unit is one lcd char, not one big char!
 * @param y_offset lcd lines!
 * TODO do we need boundary check?
 */
void LCDController::printOneNumber2(uint8_t digit, uint8_t x_offset, uint8_t y_offset) {
	// Line 1 of the one digit number
	lcdp->commandWrite( ystart[y_offset] + x_offset); //Line1

	lcdp->print (bn12[digit*3+digit*1]);
	lcdp->print (bn12[digit*3+1+digit*1]);
	lcdp->print (bn12[digit*3+2+digit*1]);

	// Line 2 of the one-digit number
	lcdp->commandWrite(ystart[y_offset+1] + x_offset); // Line 2
	lcdp->print(bn22[digit*3+digit*1]);
	lcdp->print(bn22[digit*3+1+digit*1]);
	lcdp->print(bn22[digit*3+2+digit*1]);
}

//-------------------------------------------------------------------------------------------------------

/**
 * prints a 4 line number
 * @param digit
 * @param x_offset 1 unit is one lcd char, not one big char!
 * @param y_offset lcd lines!
 * TODO do we need boundary check?
 */
void LCDController::printOneNumber4(uint8_t digit, uint8_t x_offset, uint8_t y_offset) {
	//TODO do we need the 4. col in the bnx4 arrays?

	// Line 1 of the one digit number
	lcdp->commandWrite( ystart[y_offset] + x_offset);                  //Line 1
	lcdp->print(bn14[digit*4]);
	lcdp->print(bn14[digit*4+1]);
	lcdp->print(bn14[digit*4+2]);

	// Line 2 of the one-digit number
	lcdp->commandWrite( ystart[y_offset+1] + x_offset);
	lcdp->print(bn24[digit*4]);
	lcdp->print(bn24[digit*4+1]);
	lcdp->print(bn24[digit*4+2]);

	// Line 3 of the one digit number
	lcdp->commandWrite( ystart[y_offset+2] + x_offset);
	lcdp->print(bn34[digit*4]);
	lcdp->print(bn34[digit*4+1]);
	lcdp->print(bn34[digit*4+2]);

	// Line 4 of the one-digit number
	lcdp->commandWrite( ystart[y_offset+3] + x_offset);
	lcdp->print(bn44[digit*4]);
	lcdp->print(bn44[digit*4+1]);
	lcdp->print(bn44[digit*4+2]);
}

//-------------------------------------------------------------------------------------------------------


void LCDController::blanks(uint8_t c) {
	for ( uint8_t i = 0 ; i < c ; i++ )
		lcdp->print(254);
}


void LCDController::float2string ( char* buffer, float f, int dp ) {
	//TODO implement negative values! do wee need them?
	int v = (int) f;
	double n = f - v;
	n *= dp;
	int ni = (int) n;
	char a[15];
	char b[15];
	itoa(v, &(a[0]), 10);
	itoa(ni, &(b[0]), 10);
	//buffer = a + ',' + b;
	strcpy (buffer, &a[0]);
	strcat (buffer, ".");
	strcat (buffer, &b[0]);
}

void LCDController::printFloat2DP(float f) {
	printFloat (f, "%.2f");
}

void LCDController::printFloat(float f, char* formatstring) {
	float2string ( cbuf, f, 100 );
//	snprintf (cbuf, LCD_BUFSIZE, formatstring, f);
	lcdp->printIn(cbuf);
}

void LCDController::printFloat2DP(uint8_t pos, float f) {
	printFloat (pos, f, "%.2f");
}

void LCDController::printFloat(uint8_t pos, float f, char* formatstring) {
	lcdp->commandWrite(pos);
	printFloat (f, formatstring);
}


const unsigned char LCDController::idata1[] = {28,28,28,30,30,31,31,31};  // 1--- k
const unsigned char LCDController::idata2[] = {7,7,7,15,15,31,31,31};     // -1-- k
const prog_uint8_t LCDController::idata3[] = {31,31,31,28,28,28,28,28};  // --1- k
const prog_uint8_t LCDController::idata4[] = {24,28,30,15,7,7,7,7};      // ---1 k

void LCDController::cgramIntro() {
	lcdUploadUdef5x8_P (1,idata1);
	lcdUploadUdef5x8_P (2,idata2);
	lcdUploadUdef5x8_P (3,idata3);
	lcdUploadUdef5x8_P (4,idata4);
}

const unsigned char LCDController::i2data1[] = {31,29,29,28,28,28,28,28};  // 2--- k
const unsigned char LCDController::i2data2[] = {31,23,23,7,7,7,7,7};       // -2-- k
const unsigned char LCDController::i2data3[] = {28,28,28,28,28,31,31,31};  // --2- k
const unsigned char LCDController::i2data4[] = {7,7,7,7,15,30,28,24};      // ---2 k
void LCDController::cgramIntro2() {

	lcdUploadUdef5x8_P (5,i2data1);
	lcdUploadUdef5x8_P (6,i2data2);
	lcdUploadUdef5x8_P (7,i2data3);
	lcdUploadUdef5x8_P (8,i2data4);
}


void LCDController::lcdShowIntro(int delayValue) {

	//Must load this in 2 steps, somehow its not possible to load 8 chars at once...
	cgramIntro();
	cgramIntro2();

	//Show the Intro:
	lcdp->commandWrite(0x80+2);                  //Line1
	lcdp->print(1);
	lcdp->print(2);
	lcdp->commandWrite(0x80+10);                  //Line1
	lcdp->print(3);
	lcdp->print(4);

	lcdp->commandWrite(0xC0+2); 			//Line2
	lcdp->print(5);
	lcdp->print(6);
	lcdp->printIn_P ( PSTR("ulti") );
	lcdp->commandWrite(0xC0+10); 			//Line2
	lcdp->print(7);
	lcdp->print(8);
//	lcdp->printIn ( "isplay" );
	lcdp->printIn_P ( PSTR("isplay") );

	lcdp->commandWrite(0xD4+1);                  //Line=4
	lcdp->printIn_P ( PSTR("www.designer2k2.at") );
//	lcdp->printIn ( "www.designer2k2.at" );

	//Set the LCD brightness: (This turns on the Light, looks nice)
	setBrightness(2);

	//Flash the Shiftlight:

	analogWrite(RPMSHIFTLIGHTPIN,RPM_HIGH_BRIGHT);
	delay(50);
	analogWrite(RPMSHIFTLIGHTPIN,RPM_LOW_BRIGHT);
	delay(50);
	analogWrite(RPMSHIFTLIGHTPIN,RPM_NO_BRIGHT);


	delay(delayValue);                      //Short delay, for the Intro screen :)

}

/**
 * Upload a user-defined character stored in program mem to character generator RAM of LCD (5x8)
 * 8 5x8 custom chars are available
 * \param character index of the character you want to define (there's eight user definable characters)
 * \param data pointer into program memory (!!) to an array of eight bytes that defines the character.
 *
 * To print a user defined character you just write the index to the LCD. E.g. user defined character 1 is printed by writing the byte 1. That's it.
 *
 * @see http://www.quinapalus.com/hd44780udg.html for character generator.
 * @see http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1231533425/1#1
 *
 */
void LCDController::lcdUploadUdef5x8_P(uint8_t charIndex, const uint8_t *data) {
	charIndex = charIndex << 3;
	for (uint8_t i = 0; i < 8; i++) {
		lcdp->commandWrite(0x40 | charIndex | i);
		lcdp->print( pgm_read_byte (data+i) );
	}
}

void LCDController::lcdUploadUdef5x8(uint8_t charIndex, const uint8_t *data) {
	charIndex = charIndex << 3;
	for (uint8_t i = 0; i < 8; i++) {
		lcdp->commandWrite(0x40 | charIndex | i);
		lcdp->print( data[i] );
	}
}


const uint8_t LCDController::dotdata1[] = {31,0,0,0,0,0,0,31};        //only 2 dots
const uint8_t LCDController::dotdata2[] = {31,0,0,14,0,0,0,31};       //the 2 dots with a minus
const uint8_t LCDController::dotdata3[] = {31,0,4,14,4,0,0,31};       //the 2 dots with a plus
const uint8_t LCDController::dotdata4[] = {31,14,31,31,31,31,14,31};  //thats a block with little rounded edges

/**
 * uploads dots to cgram
 * ATTENTION: this arent dots but rather 2 bars (at top and bottom) plus minus, plus etc
 */
void LCDController::cgramDot() {

	lcdUploadUdef5x8_P (5,dotdata1);
	lcdUploadUdef5x8_P (6,dotdata2);
	lcdUploadUdef5x8_P (7,dotdata3);
	lcdUploadUdef5x8_P (8,dotdata4);
}
//-------------------------------------------------------------------------------------------------------

const uint8_t LCDController::bardata1[] = {16,16,16,16,16,16,16,16};  //1 line
const uint8_t LCDController::bardata2[] = {24,24,24,24,24,24,24,24};  //2 line
const uint8_t LCDController::bardata3[] = {28,28,28,28,28,28,28,28};  //3 line
const uint8_t LCDController::bardata4[] = {30,30,30,30,30,30,30,30};  //4 line

/**
 * uploads bar chars to cgram
 */
void LCDController::cgramBar() {
	lcdUploadUdef5x8_P (1,bardata1);
	lcdUploadUdef5x8_P (2,bardata2);
	lcdUploadUdef5x8_P (3,bardata3);
	lcdUploadUdef5x8_P (4,bardata4);
}


/**
 * draws a bar to the lcd
 * @param Digits number of chars the bar has on the lcd screen
 * @param Value the actual value. 0 <= value <= 10 x Digits ?!?
 */
void LCDController::drawBar(uint8_t digits, uint8_t value) {

	uint8_t Curs = value/10;
	for (uint8_t i=0; i < digits; i++) {
		if ( i<Curs ) {
			lcdp->print(255);
		} else {

			if(i==Curs) {
				//in val ist 84 und in val5 die 8
				//ich brauch die 4 /2 als int
				int ChrW = (value - Curs*10)/2;

				if(ChrW==0) {
					lcdp->printIn(" ");
				} else {
					lcdp->print(ChrW);
				}
			} else {
				lcdp->printIn(" ");
			}
		}
	}
}

const uint8_t LCDController::vbardata1[] = {0,0,0,0,0,0,0,31};              //1 row
const uint8_t LCDController::vbardata2[] = {0,0,0,0,0,0,31,31};             //2 rows
const uint8_t LCDController::vbardata3[] = {0,0,0,0,0,31,31,31};            //3 rows
const uint8_t LCDController::vbardata4[] = {0,0,0,0,31,31,31,31};           //4 rows
const uint8_t LCDController::vbardata5[] = {0,0,0,31,31,31,31,31};          //5 rows
const uint8_t LCDController::vbardata6[] = {0,0,31,31,31,31,31,31};         //6 rows
const uint8_t LCDController::vbardata7[] = {0,31,31,31,31,31,31,31};        //7 rows

/**
 * uploads vertical bars to cgram
 */
void LCDController::cgramVertBar() {
	lcdUploadUdef5x8_P (1,vbardata1);
	lcdUploadUdef5x8_P (2,vbardata2);
	lcdUploadUdef5x8_P (3,vbardata3);
	lcdUploadUdef5x8_P (4,vbardata4);
	lcdUploadUdef5x8_P (5,vbardata5);
	lcdUploadUdef5x8_P (6,vbardata6);
	lcdUploadUdef5x8_P (7,vbardata7);
}
//-------------------------------------------------------------------------------------------------------

/**
 * draws a vertical bar
 * @param val must be between 0 and 32 (4x8)
 * @param pos must be between 0 and 20 (on a 20 char disp)
 */
void LCDController::drawVertBar(uint8_t val, uint8_t pos) {

	//whats the modulo with 8 ?
	uint8_t mod = val % 8;
	//This is directly the code for the top character!
	//But at what pos it must be printed?

	uint8_t printPos = (val - mod) / 8;

	//But if Mod = 0, the LCD nothing is 254.

	if(mod == 0)
		mod=254;

//	for ( uint8_t i = 3 ; i > 0 ; i-- ) {
//		//blank
//		uint8_t  draw = 254;
//		if (printPos == i)
//			draw = mod;
//		else if ( i < printPos )
//			draw = 255;
//		//now draw it!
//		lcdp->commandWrite( ystart[i] + pos);
//		lcdp->print( draw );
//	}

	//now lets make a switch case depending on the PrintPos.
	switch(printPos){
	case 0:
		//so there is only one character /3 blanks and the character in the lowest position
		lcdp->commandWrite(0x80+pos);              //Line 1
		lcdp->print(254); //Blank
		lcdp->commandWrite(0xC0+pos);              //Line 2
		lcdp->print(254); //Blank
		lcdp->commandWrite(0x94+pos);              //Line 3
		lcdp->print(254); //Blank
		lcdp->commandWrite(0xD4+pos);              //Line 4
		lcdp->print(mod); //The Custom Character presenting the value
		break;
	case 1:
		//so there is only one character /1 fulls and the character in the 2nd position
		lcdp->commandWrite(0x80+pos);              //Line 1
		lcdp->print(254); //Blank
		lcdp->commandWrite(0xC0+pos);              //Line 2
		lcdp->print(254); //Blank
		lcdp->commandWrite(0x94+pos);              //Line 3
		lcdp->print(mod); //The Custom Character presenting the value
		lcdp->commandWrite(0xD4+pos);              //Line 4
		lcdp->print(255); //Full
		break;
	case 2:
		//so there is only one character /2 fulls and the character in the 3rd position
		lcdp->commandWrite(0x80+pos);              //Line 1
		lcdp->print(254); //Blank
		lcdp->commandWrite(0xC0+pos);              //Line 2
		lcdp->print(mod); //The Custom Character presenting the value
		lcdp->commandWrite(0x94+pos);              //Line 3
		lcdp->print(255); //Full
		lcdp->commandWrite(0xD4+pos);              //Line 4
		lcdp->print(255); //Full
		break;
	case 3:
		//so there is only one character /3 fulls and the character in the highest position
		lcdp->commandWrite(0x80+pos);              //Line 1
		lcdp->print(mod); //The Custom Character presenting the value
		lcdp->commandWrite(0xC0+pos);              //Line 2
		lcdp->print(255); //Full
		lcdp->commandWrite(0x94+pos);              //Line 3
		lcdp->print(255); //Full
		lcdp->commandWrite(0xD4+pos);              //Line 4
		lcdp->print(255); //Full
		break;
	case 4:
		//easy, eveything is full!
		lcdp->commandWrite(0x80+pos);              //Line 1
		lcdp->print(255); //Full
		lcdp->commandWrite(0xC0+pos);              //Line 2
		lcdp->print(255); //Full
		lcdp->commandWrite(0x94+pos);              //Line 3
		lcdp->print(255); //Full
		lcdp->commandWrite(0xD4+pos);              //Line 4
		lcdp->print(255); //Full
		break;
	}

	//thats it, easy :)

}



void LCDController::scopeMode() {

	//but work the screen from left side
	for(int i=0; i<=18; i++){

		//Print the Buffer
		drawVertBar(scopeInt[i],i);

		//shift Data to the left
		scopeInt[i] = scopeInt[i+1];
	}
}

void LCDController::printBigNum (uint16_t value, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type) {
//	snprintf(cbuf, LCD_BUFSIZE, "%d", value);
	itoa (value, cbuf, 10);
	printBigNum ( cbuf, length, x_offset, y_offset, type );
}

void LCDController::printBigNum (double value, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type) {
//	snprintf(cbuf, LCD_BUFSIZE, "%.2f", value);
	float2string ( cbuf, value, 100 );
	printBigNum ( cbuf, length, x_offset, y_offset, type );
}

/**
* displays a digit Number over 2 or 4 lines
* right aligned, it clears all in front starting at x_offset
* @param str string to be printed
* @param length
* @param x_offset
* @param y_offset
* @param type LCD_BIGFONT_2 or LCD_BIGFONT_4 (2 or 4 rows)
*/
void LCDController::printBigNum (char* str, uint8_t length, uint8_t x_offset, uint8_t y_offset, uint8_t type) {
	//blank 254
	//point 7
	//comma 5

	//set to pos where decimal point should be, 0 if no
	uint8_t dp_pos = 99;


	//string length
	uint8_t stringlen = 0;
	//length of the string on the lcd screen (one char needs 3 or 4 positions on screen!)
	uint8_t screenlen = 0;

	//compute array of uint8_t out of the char array
	uint8_t value_i[21];
    char c[2];
    c[1] = '\0';
    for ( uint8_t i = 0 ; i < strlen(str) ; i++ ) {
    	c[0] = str[i];
    	if (c[0]=='.' || c[0]==';' || c[0]==',') {
    		value_i[i]=c[0];
    		dp_pos = i;
   			stringlen++;
    	} else {
    		// 0-9 in ascii table
    		if ( c[0] >= 48 && c[0] <= 57 ) {
    			value_i[i] = atoi(&c[0]);
   				stringlen++;
   				screenlen += 4;
    		}
    	}
	 }

    //clear digits in front
    if ( length - screenlen > 0 ) {
    	for ( uint8_t l = 0 ; l < type ; l++ ) {
    		lcdp->commandWrite( ystart[l + y_offset] + x_offset);
    		blanks( length - stringlen );
    	}
    }

    //now set x_offset to x_offset + blanks!
    if ( length - screenlen > 0 )
    	x_offset += length - screenlen;

	for ( uint8_t i = 0 ; i < stringlen ; i++ ) {
		if ( i == dp_pos ) {
			//TODO print bigger decimal point!
			lcdp->print(5);
		} else {
			if ( type == LCD_BIGFONT_2 ) {
				printOneNumber2(value_i[i], x_offset, y_offset );
				x_offset += 3;
//				x_offset += 4;
			} else {
				printOneNumber4(value_i[i], x_offset, y_offset );
				x_offset += 4;
			}
		}
	}
}

/**
 * prints a integer value to postion pos
 */
void LCDController::printInt (uint8_t pos, int value) {
//	snprintf (cbuf, LCD_BUFSIZE, "%d", value);
	itoa (value, cbuf, 10);
	printString (pos, cbuf);
}
/**
 * print a string to position pos
 */
void LCDController::printString (uint8_t pos, char* str) {
	if (pos >= 0x80)
		lcdp->commandWrite (pos);
	lcdp->printIn (str);
}
