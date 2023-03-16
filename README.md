# Raspberry Library for LiquidCrystal displays with I2C PCF8574 adapter

A library for driving LiquidCrystal displays (LCD) by using the I2C bus and an PCF8574 I2C adapter.

There are modules that can be soldered or stacked to the display that offers an I2C interface for communication instead of the 8+ digital lines that are used to send data to the display.

Most of these modules use the wiring that is supported by this library's default configuration. 

## How to install ?

```bash
git clone https://github.com/aiRCS-Lab/rasplcd.git
cd rasplcd
mkdir build && cd build
cmake ..
make
sudo make install
```

Don't forget to update your library folder with `sudo ldconfig`.

## How to use ?

Simply add the library to your CMakeLists.txt and include the header in your project.

```cmake
add_library(rasplcd SHARED IMPORTED)
set_property(TARGET rasplcd PROPERTY IMPORTED_LOCATION "/usr/lib/librasplcd.so")

# it is an application
add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} rasplcd)
```

Then include `rasplcd` :

```cpp
#include <rasplcd.hpp>
```

## Methods

### Examples

```cpp
LiquidCrystal lcd(0x27, 16, 2); // addr, col, row

int main(int argc, char *argv[]) {
    lcd.begin();
    lcd.noCursor();

    char str[17] = "Je suis un test!";
    lcd.print(str, 16);
    
    return 0;
}
```

---
&copy; 2023 aiRCS Lab SAS