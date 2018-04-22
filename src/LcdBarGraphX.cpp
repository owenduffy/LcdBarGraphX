/**
 * File: LcdBarGraphX.cpp
 * Description:
 * LcdBarGraphX is an Arduino library for displaying analog values in LCD display, 
 *   which is previously initialized. This library uses LCD library (https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home) for displaying.
 *
 * Author: Balazs Kelemen
 * Contact: prampec+arduino@gmail.com
 * Copyright: 2010 Balazs Kelemen
 * Credits: Hans van Neck
 * Copying permission statement:
    This file is part of LcdBarGraphX.

    LcdBarGraphX is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Arduino.h"
#include "LcdBarGraphX.h"

// -- initializing bar segment characters
// -- filled character
byte LcdBarGraphX::_level0[8] = {
    B10101,
    B10101,
    B10101,
    B10101,
    B10101,
    B10101,
    B10101,
    B10101
};
// -- character with one bar
byte LcdBarGraphX::_level1[8] = {
    B10101,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10101
};
// -- character with two bars
byte LcdBarGraphX::_level2[8] = {
    B10101,
    B10100,
    B10100,
    B10100,
    B10100,
    B10100,
    B10100,
    B10101
};
// -- character with line
byte LcdBarGraphX::_level3[8] = {
    B10101,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B10101
};
// -- constructor
LcdBarGraphX::LcdBarGraphX(LCD* lcd, byte numCols, byte startX, byte startY)
{
    // -- setting fields
    _lcd = lcd;
    _numCols = numCols;
    _startX = startX;
  	_startY = startY;
}

void LcdBarGraphX::begin()
{
    // -- creating characters
    _lcd->createChar(0, this->_level0);
    _lcd->createChar(1, this->_level1);
    _lcd->createChar(2, this->_level2);
    _lcd->createChar(3, this->_level3);
    // -- setting initial values
    this->_prevValue = -1; // -- cached value
//    this->_lastFullChars = 0; // -- cached value
  	this->_initialized = true;
}

// -- the draw function
void LcdBarGraphX::drawValue(int value, int maxValue) {
	if(!this->_initialized) {
		this->begin();
	}
    //limit input
    if(value<0) {value=0;}
    if(value>maxValue) {value=maxValue;}
    // -- calculate full (filled) character count
    byte fullChars = (long)value * _numCols / maxValue;
    // -- calculate partial character bar count
    byte mod = ((long)value * _numCols * 3 / maxValue) % 3;

    // -- if value does not change, do not draw anything
    int normalizedValue = (int)fullChars * 3 + mod;
    if(this->_prevValue != normalizedValue) {
        // -- do not clear the display to eliminate flickers
        _lcd->setCursor(_startX, _startY);
        
        // -- write filled characters
        for(byte i=0; i<fullChars; i++) {
            _lcd->write((byte)0);
        }
        
        // -- write the partial character
        if(mod > 0) {
            _lcd->write(mod); // -- index the right partial character
            ++fullChars;
        }
        
        // -- clear characters left over the previous draw
        for(byte i=fullChars;i<this->_numCols;i++) {
            _lcd->write((byte)3);
        }
        
        // -- save cache
//        this->_lastFullChars = fullChars;
        this->_prevValue = normalizedValue;
    }
    /*
    // debug info
    _lcd->setCursor(0,1);
    _lcd->write('[');
    _lcd->print((int)value);
    _lcd->write(' ');
    _lcd->print(normalizedValue);
    _lcd->write(' ');
    _lcd->print((int)mod);
    _lcd->write(']');
    */
}
