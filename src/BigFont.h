/*
  BigFont.h - A library that displays large characters on the LCD.
  Copyright 2011,2016(c) arms22. All right reserved.

  License: Refer to the phi_big_font.h/cpp file.
*/

#ifndef BigFont_H_
#define BigFont_H_

#include <Arduino.h>
#include <LiquidCrystal.h>

class BigFont : public Print
{
public:
	BigFont();
	~BigFont();
	void attach(LiquidCrystal *lcd);
	void clear();
	void home();
	void setInvert(bool yes);
	void setCursor(uint8_t col, uint8_t row);
	virtual size_t write(uint8_t);
private:
	void lcd_clear();
	void render_big_char(char ch, byte loc_x, byte loc_y);
	void init_big_font();
    LiquidCrystal *_lcd;
	uint8_t _cur_col;
	uint8_t _cur_row;
	bool _invert;
};

#endif
