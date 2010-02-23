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

#ifndef LCDSCREEN8_H_
#define LCDSCREEN8_H_

#include "LCDScreen.h"

/**
 * Max Value Screen (This shows the Max LD, RPM or AGT or Oil values)
 * screen8: max boost, rpm, lmm, agt
 */
class LCDScreen8: public LCDScreen {
public:
	LCDScreen8();

	void customInit ();
	void customDraw ();
};

#endif /* LCDSCREEN8_H_ */