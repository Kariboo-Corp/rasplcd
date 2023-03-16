#include <rasplcd.hpp>

LiquidCrystal lcd(0x27, 16, 2);

int main(int argc, char *argv[]) {
    lcd.begin();
    lcd.noCursor();

    char str[17] = "Je suis un test!";
    lcd.print(str, 16);
    
    return 0;
}
