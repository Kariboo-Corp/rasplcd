#include <rasplcd.hpp>

LiquidCrystal::LiquidCrystal(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows, uint8_t charsize)
{
	_addr = lcd_addr;
	_cols = lcd_cols;
	_rows = lcd_rows;
	_charsize = charsize;
	_backlightval = LCD_BACKLIGHT;
}

void LiquidCrystal::begin()
{
    _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS; // set the display characteristics

    if (_rows > 1) {
        _displayfunction |= LCD_2LINE; 
    }

    if ((_charsize != 0) && (_rows == 1)) {
		_displayfunction |= LCD_5x10DOTS;
	}

    usleep(50000); // let the lcd catch his breath

    expanderWrite(_backlightval); // reset expander and turn backlight off
    sleep(1); // let lcd catch command

    	write4bits(0x03 << 4);
	usleep(4500); // wait min 4.1ms

	// second try
	write4bits(0x03 << 4);
	usleep(4500); // wait min 4.1ms

	// third go!
	write4bits(0x03 << 4);
	usleep(150);

	// finally, set to 4-bit interface
	write4bits(0x02 << 4);

	// set # lines, font size, etc.
	command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();

	// clear it off
	clear();

	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	// set the entry mode
	command(LCD_ENTRYMODESET | _displaymode);

	home();
}

void LiquidCrystal::clear()
{
    command(LCD_CLEARDISPLAY);
    usleep(2000);
}

void LiquidCrystal::print(char * _string, uint8_t _len = 16)
{
    for (uint8_t i = 0; i < _len; i++)
	{
		if (_string[i] == '\0') return;
		write_rs(((int)(_string[i])));
	}
	
}

void LiquidCrystal::print(std::string _string)
{

	char* c = &*_string.begin();

	for (uint8_t i = 0; i < _cols; i++)
	{
		if (c[i] == '\0') return;
		write_rs(((int)(c[i])));
	}
}

void LiquidCrystal::home()
{
    command(LCD_RETURNHOME);
    usleep(2000);
}

void LiquidCrystal::setCursor(uint8_t col, uint8_t row)
{
    int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	
    if (row > _rows) {
		row = _rows-1;    // we count rows starting w/0
	}
	
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LiquidCrystal::noDisplay()
{
    _displaycontrol &= ~LCD_DISPLAYOFF;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal::display()
{
    _displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal::noCursor()
{
    _displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal::cursor()
{
    _displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal::noBlink()
{
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal::blink()
{
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void LiquidCrystal::scrollDisplayLeft(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LiquidCrystal::scrollDisplayRight(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void LiquidCrystal::leftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal::rightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal::autoscroll(void)
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal::noAutoscroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

void LiquidCrystal::createChar(uint8_t location, uint8_t charmap[])
{
	location &= 0x7; // we only have 8 locations 0-7
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write_rs(charmap[i]);
	}
}

void LiquidCrystal::noBacklight(void)
{
	_backlightval=LCD_NOBACKLIGHT;
	expanderWrite(0);
}

void LiquidCrystal::backlight(void)
{
	_backlightval=LCD_BACKLIGHT;
	expanderWrite(0);
}
bool LiquidCrystal::getBacklight()
{
  return _backlightval == LCD_BACKLIGHT;
}

/**
 * Private functions
*/

void LiquidCrystal::expanderWrite(uint8_t _data)
{
    int fd;

    if ((fd = open(I2C_BUS, O_RDWR)) < 0) {
        if (VERBOSE) printf("Error failed to open I2C bus [%s].\n", I2C_BUS);
        exit(-1);
    }

    if (ioctl(fd, I2C_SLAVE, _addr) < 0) {
        if (VERBOSE) printf("Error failed to set I2C address [%i].\n", _addr);
        exit(-1);
    }

    _i2c_fd = fd;

    unsigned char byte[1];
    byte[0] = ((int)(_data) | _backlightval);

    write(_i2c_fd, byte, 1);
    close(_i2c_fd);
}

void LiquidCrystal::pulseEnable(uint8_t _data)
{
	expanderWrite(_data | En);	// En high
	usleep(1);		// enable pulse must be >450ns

	expanderWrite(_data & ~En);	// En low
	usleep(50);		// commands need > 37us to settle
}

void LiquidCrystal::write4bits(uint8_t value)
{
	expanderWrite(value);
	pulseEnable(value);
}

void LiquidCrystal::send(uint8_t value, uint8_t mode)
{
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;

	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
}

void LiquidCrystal::command(uint8_t value) {
	send(value, 0);
}

uint8_t LiquidCrystal::write_rs(uint8_t value) {
	send(value, Rs);
	return 1;
}