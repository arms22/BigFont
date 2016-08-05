/*
  Warning: PLEASE DO NOT REMOVE THIS COMMENT WHEN REDISTRIBUTING!
  This is the second official release of the Phi-big-font library version 20110617
  Released for Phi-1 and Phi-2 shields for Arduino
  User is encouraged to obtain a Phi shield or is solely responsible for converting it to work on other shields or configurations.
  
  Programmed by Dr. John Liu
  Revision: 06/17/2011
  Free software for educational and personal uses.
  No warrantee!
  Commercial use without authorization is prohibited.
  Find details of the Phi-1 shield, Phi-2 shield, and Phi-big-font or contact Dr. Liu at
  http://liudr.wordpress.com/phi-1-shield/
  http://liudr.wordpress.com/phi-2-shield/
  http://liudr.wordpress.com/phi_big_font/
  All rights reserved.
  
  Updates:
  The array names for programmable characters on the LCD has been changed to aviod conflict with other libraries. 

  modified by arms22.
  Revision: 08/01/2016
*/
#include <stdio.h>
#include "phi_big_font.h"

#if defined(ARDUINO_ARCH_AVR)
  #include <avr/pgmspace.h>
#else
  #ifndef PROGMEM
    #define PROGMEM
  #endif
  #ifndef pgm_read_byte
    #define pgm_read_byte(p) (*(p))
  #endif
  #ifndef pgm_read_word
    #define pgm_read_word(p) (*(p))
  #endif
  #ifndef strcpy_P
  	#define strcpy_P strcpy
  #endif
#endif

static const char pbf_lcd_ch0[]	PROGMEM = {64,64,64,64,64,64,64,64,0};	//0
static const char pbf_lcd_ch1[]	PROGMEM = {64,64,64,64,64,31,31,31,0};	//1
static const char pbf_lcd_ch2[]	PROGMEM = {64,64,64,31,31,64,64,64,0};	//2
static const char pbf_lcd_ch3[]	PROGMEM = {64,64,64,31,31,31,31,31,0};	//3
static const char pbf_lcd_ch4[]	PROGMEM = {31,31,31,64,64,64,64,64,0};	//4
static const char pbf_lcd_ch5[]	PROGMEM = {31,31,31,64,64,31,31,31,0};	//5
static const char pbf_lcd_ch6[]	PROGMEM = {31,31,31,31,31,64,64,0};		//6
static const char pbf_lcd_ch7[]	PROGMEM = {31,31,31,31,31,31,31,31,0};	//7

static const char * const pbf_ch_item[] PROGMEM = {
	pbf_lcd_ch0,
	pbf_lcd_ch1,
	pbf_lcd_ch2,
	pbf_lcd_ch3,
	pbf_lcd_ch4,
	pbf_lcd_ch5,
	pbf_lcd_ch6,
	pbf_lcd_ch7
};

static const char font_35_09[] PROGMEM = {
	1,2,3,7,1,6, 1,3,0,1,7,1, 2,2,3,7,5,5, 2,2,3,5,5,7,
	3,0,3,4,4,7, 3,2,2,5,5,7, 3,2,2,7,5,7, 2,2,3,0,0,7,
	3,2,3,7,5,7, 3,2,3,5,5,7
};

static const char font_35_dot[] = {
    0,0,0,0,3,0, // '.'
};

/*
( 0,3,4,0,6,1,
) 4,3,0,1,6,0,
* 1,3,1,2,7,2,
+ 0,3,0,4,7,4,
, 0,0,0,0,3,0,
- 0,0,0,4,4,4,
. 0,0,0,0,3,0,
/ 0,0,3,3,4,0,
: 0,3,0,0,3,0,
= 1,1,1,2,2,2,
[ 0,1,2,0,6,1,
] 2,1,0,1,6,0,
_ 0,0,0,1,1,1,
> 2,1,0,1,2,4,
< 0,1,2,4,2,1,
? 2,2,3,0,5,0,
*/
static const char font_35_AZ[] PROGMEM = {
	3,2,3,7,4,7, 3,2,3,7,5,3, 3,2,2,7,1,1, 3,2,1,7,1,6,
	3,2,2,7,5,5, 3,2,2,7,4,4, 3,2,2,7,1,7, 3,0,3,7,4,7,
	2,3,2,1,7,1, 2,2,3,3,1,7, 3,0,1,7,4,3, 3,0,0,7,1,1,
	3,1,3,7,4,7, 3,2,3,7,0,7, 3,2,3,7,1,7, 3,2,3,7,4,4,
	3,2,3,4,4,7, 3,2,3,7,4,3, 3,2,2,5,5,7, 2,3,2,0,7,0,
	3,0,3,7,1,7, 3,0,3,6,1,6, 3,0,3,7,6,7, 3,0,3,3,4,3,
	3,0,3,4,7,4, 2,2,3,3,5,1
};

static const char font_35_az[] PROGMEM = {
	2,2,1,2,5,7, 3,1,0,7,1,6, 1,2,2,6,1,1, 0,1,3,6,1,7,
	1,2,1,6,5,5, 0,1,2,4,7,4, 1,2,1,1,5,6, 3,1,0,7,0,7,
	0,2,0,0,7,0, 0,0,2,3,1,6, 3,0,1,7,4,3, 3,0,0,6,1,1,
	3,3,1,7,4,7, 3,2,1,7,0,7, 1,2,1,6,1,6, 3,2,1,7,4,0,
	1,2,3,0,4,7, 1,2,2,7,0,0, 1,2,2,1,5,2, 1,3,1,0,6,1,
	3,0,3,6,1,7, 3,0,3,6,1,6, 3,0,3,6,7,7, 3,0,3,3,4,3,
	3,0,3,1,5,6, 2,2,3,3,5,1
};

//static boolean inverted_big_font = false;

static void read_big_char_code(char ch, uint8_t *buf)
{
	const char *from;
	uint8_t i;
	if ( (ch >= '0') && (ch <= '9')) {
		ch = ch - '0';
		from = font_35_09;
	}
    else if ( (ch == ' ') ) {
		for (i=0; i<6; i++){
			buf[i] = 0;
		}
		return;
    }
    else if ( (ch == '.') ) {
		ch = ch - '.';
		from = font_35_dot;
    }
	else if ( (ch >= 'A') && (ch <= 'Z') ) {
		ch = ch - 'A';
		from = font_35_AZ;
	}
	else if ( (ch >= 'a') && (ch <= 'z')) {
		ch = ch - 'a';
		from = font_35_az;
	}
	else {
		i = 0;
		for (; i<3; i++){
			buf[i] = 3;
		}
		for (; i<6; i++){
			buf[i] = 7;
		}
		return;
	}
	uint8_t off = ch * 6;
	for (i=0; i<6; i++){
		buf[i] = pgm_read_byte(from + off + i);
	}
}

static void render_big_char_half(uint8_t *dat, LiquidCrystal *lcd, bool invert)
{
	if (!invert) {
		for (uint8_t i=0; i<3; i++){
			lcd->write(dat[i]);
		}
		lcd->write(' ');
	}else{
		for (uint8_t i=0; i<3; i++){
			lcd->write(7 - dat[i]);
		}
		lcd->write(255);
	}
}

void BigFont::render_big_char(char ch, byte loc_x, byte loc_y)
{
	uint8_t dat[6];
	LiquidCrystal *lcd = this->_lcd;
	bool invert = this->_invert;
	read_big_char_code(ch, dat);
	
	lcd->setCursor(loc_x, loc_y);
	render_big_char_half( &dat[0], lcd, invert);
	
	lcd->setCursor(loc_x, loc_y + 1);
	render_big_char_half( &dat[3], lcd, invert);
}

// void render_big_msg(char msg[], byte loc_x, byte loc_y)
// {
//   byte i=0;
//   while(msg[i])
//   {
//     render_big_char(msg[i],loc_x+i*4, loc_y);
//     i++;
//   }
// }

// void render_big_number(int number, byte loc_x, byte loc_y)
// {
//   char msg[11];
//   sprintf(msg,"%d",number);
//   render_big_msg(msg, loc_x, loc_y);
// }

//Display strings stored in PROGMEM. Provide the string name stored in PROGMEM to be displayed on the LCD's current cursor position.
// void big_msg_lcd(char* msg_line, byte loc_x, byte loc_y)
// {
//   char msg_buffer[17];
//   strcpy_P(msg_buffer,msg_line);
//   msg_buffer[4]=0; 
//   render_big_msg(msg_buffer, loc_x, loc_y);
// }

void BigFont::lcd_clear()
{
  LiquidCrystal *lcd = this->_lcd;
  if (!this->_invert)
  {
  	lcd->clear();
  }
  else
  {
    lcd->setCursor(0,0);
    for (byte i=0;i<80;i++)
    {
      lcd->write(255);
    }
  }
}

// void invert_big_font(boolean inv)
// {
//   inverted_big_font = inv;
// }

void BigFont::init_big_font()
{
  LiquidCrystal *lcd = this->_lcd;
  byte ch_buffer[10]; // This buffer is required for custom characters on the LCD.
  for (int i=0;i<8;i++)
  {
    strcpy_P((char*)ch_buffer,(char*)pgm_read_word(&(pbf_ch_item[i])));
    lcd->createChar(i, ch_buffer);
  }
}

// void display_img(byte msg[])
// {
//   output_lcd->setCursor(0,0);
//   for (byte i=0;i<80;i++)
//   {
//     output_lcd->write(msg[i]);
//   }
// }
