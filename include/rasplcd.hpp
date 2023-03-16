#ifndef RASPLCD_H
#define RASPLCD_H

#define VERSION 0.0.1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#if __APPLE__ // Not usable on Apple ... used to test if it compile.
    #include <sys/ioctl.h>
    #define I2C_SLAVE 1
#endif

#if __linux__
	#include <linux/i2c.h>
    #include <linux/i2c-dev.h>
    #include <sys/ioctl.h>
#endif

#include <fcntl.h>

#define I2C_BUS        "/dev/i2c-1" // I2C bus device on a Raspberry Pi 3
#define VERBOSE 1

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0x4  // Enable bit
#define Rw 0x2  // Read/Write bit
#define Rs 0x1  // Register select bit

class LiquidCrystal {
public:
    /**
	 * Constructor
	 *
	 * @param lcd_addr	I2C slave address of the LCD display. Most likely printed on the
	 *					LCD circuit board, or look in the supplied LCD documentation.
	 * @param lcd_cols	Number of columns your LCD display has.
	 * @param lcd_rows	Number of rows your LCD display has.
	 * @param charsize	The size in dots that the display has, use LCD_5x10DOTS or LCD_5x8DOTS.
	 */
	LiquidCrystal(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize = LCD_5x8DOTS);

    void begin();
    void clear();
    void home();
    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void print(char *, uint8_t);
    void noCursor();
    void cursor();
    
    void scrollDisplayLeft();
	void scrollDisplayRight();
	void printLeft();
	void printRight();
	void leftToRight();
	void rightToLeft();
	void shiftIncrement();
	void shiftDecrement();
	void noBacklight();
	void backlight();
	bool getBacklight();
	void autoscroll();
	void noAutoscroll();
	void createChar(uint8_t, uint8_t[]);
	void setCursor(uint8_t, uint8_t);

    inline void blink_on() { blink(); }
	inline void blink_off() { noBlink(); }
	inline void cursor_on() { cursor(); }
	inline void cursor_off() { noCursor(); }

    void setBacklight(uint8_t new_val);				// alias for backlight() and nobacklight()
	void load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()

private:
	void expanderWrite(uint8_t);
	void pulseEnable(uint8_t);
	void send(uint8_t, uint8_t);
	void write4bits(uint8_t);
	void command(uint8_t);
	uint8_t write_rs(uint8_t);
	uint8_t _addr;
	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _cols;
	uint8_t _rows;
	uint8_t _charsize;
	uint8_t _backlightval;
    int _i2c_fd;
};

#endif