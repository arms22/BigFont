/*
  BigFont.cpp - A library that displays large characters on the LCD.
  Copyright 2011,2016(c) arms22. All right reserved.
  
  License: Refer to the phi_big_font.h/cpp file.
*/

#include "BigFont.h"

BigFont::BigFont()
{
	_cur_col = _cur_row = 0;
	_invert = false;
}

BigFont::~BigFont()
{
}

void BigFont::attach(LiquidCrystal *lcd)
{
    _lcd = lcd;
	_invert = false;
	init_big_font();
}

void BigFont::clear()
{
	lcd_clear();
	home();
}

void BigFont::home()
{
	_cur_col = _cur_row = 0;
}

void BigFont::setInvert(bool yes)
{
	_invert = yes;
}

void BigFont::setCursor(uint8_t col, uint8_t row)
{
	_cur_col = col;
	_cur_row = row;
}

size_t BigFont::write(uint8_t ch)
{
	render_big_char(ch, _cur_col, _cur_row);
	_cur_col += 4;
    return 1;
}
